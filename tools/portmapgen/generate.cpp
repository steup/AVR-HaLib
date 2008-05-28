/**
 *	\file	generate.cpp
 *	\brief	Generate c++ code from already parsed portmap definitions
 *	\author	Philipp Werner
 *	\date	20.05.2008
 *
 *	\todo	alignment of coments
 */


#include "portmapgen.h"

#include "uc.h"

#include <string>
#include <cctype>
#include <fstream>
#include <algorithm>
#include <iostream>

namespace generate
{

std::string tolower(std::string s)
{
	for (int i = s.length() - 1; i >= 0; i--)
		s[i] = std::tolower(s[i]);
	return s;
}

std::string capitalize(std::string s)
{
	s[0] = std::toupper(s[0]);
	for (int i = s.length() - 1; i >= 1; i--)
		s[i] = std::tolower(s[i]);
	return s;
}


const char * getDataType(int bits)
{
	assert(bits > 0);
	if (bits == 1)
		return "bool";
	else if (bits <= 8)
		return "uint8_t";
	else if (bits <= 16)
		return "uint16_t";
	else
		return "uint32_t";
}



struct MapEntryData
{
	std::string identifier;
	uc::PinType ptype;
	int addr;
	const PinBlock * pb;
};


void generateMapEntries(std::ostream & stream, MapEntryData ** p, int pCount, std::string indentation = "\t\t\t")
{
	assert(p && pCount >= 0);

	// Sort p by addresses and pins
	for (int o = pCount - 1; o >= 1; o--)
		for (int i = o; i >= 1; i--)
			if (p[i]->addr < p[i-1]->addr || (p[i]->addr == p[i-1]->addr && p[i]->pb->firstPin < p[i-1]->pb->firstPin))
				std::swap(p[i], p[i-1]);

	// All addresses have to be positive (there are valid addresses for all entires)
	assert(p[0]->addr >= 0);

	// Start with address 0, pin 0
	int prevAddress = -1;
	int prevPin = 7;

	// Generate code for entries
	for (int i = 0; i < pCount; i++)
	{
		// generate padding between pins
		const int fullBytePad = p[i]->addr - prevAddress - 1;
		if (fullBytePad > 0)
		{
			if (prevPin < 7)
			{
				stream << indentation << getDataType(8) << " : " << (7 - prevPin) << ";\n";
				prevPin = 7;
			}

			stream << indentation << getDataType(8) << " __pad" << i << " [0x" << std::hex << fullBytePad << std::dec << "];\n";
		}
		const int bitPad = 7 - prevPin + p[i]->pb->firstPin;
		if (bitPad > 0)
			stream << indentation << getDataType(8) << " : " << bitPad << ";\n";

		// generate pin code
		const int pinCount = p[i]->pb->getPinCount();
		stream << indentation << getDataType(pinCount) << " " << p[i]->identifier << " : " << pinCount
		       << ";\t\t// " << uc::getPinTypeString(p[i]->ptype) << (char)toupper(p[i]->pb->port) << " (0x" << std::hex << p[i]->addr << std::dec << "), bit " << p[i]->pb->firstPin;
		if (pinCount > 1)
			stream << "-" << p[i]->pb->lastPin;
		stream << "\n";
		
		prevAddress = p[i]->addr;
		prevPin = p[i]->pb->lastPin;
	}
}



void generatePinBlock(std::ostream & stream, const PinBlock * p, const uc::AvrUC * controller, std::string indentation = "\t\t")
{
	assert(p && controller);

	stream << indentation << "struct\t\t// ";
	if (p->getPinCount() == 1)
		stream << "pin " << p->identifier << ": " << p->port << " " << p->firstPin << ";";
	else
		stream << "pins " << p->identifier << ": " << p->port << " " << p->firstPin << "-" << p->lastPin << ";";
	stream << "\n" << indentation << "{\n";

	MapEntryData dPin = { tolower(uc::getPinTypeString(uc::PIN)), uc::PIN, uc::getAddress(p->port, uc::PIN, controller), p };
	MapEntryData dDdr = { tolower(uc::getPinTypeString(uc::DDR)), uc::DDR, uc::getAddress(p->port, uc::DDR, controller), p };
	MapEntryData dPort = { tolower(uc::getPinTypeString(uc::PORT)), uc::PORT, uc::getAddress(p->port, uc::PORT, controller), p };
	MapEntryData * d [3] = { & dPin, & dDdr, & dPort };

	generateMapEntries(stream, d, 3, indentation + '\t');
	
	stream << indentation << "} " << p->identifier << ";\n";
}


std::string getBitMask(int firstBit, int bitCount)
{
	assert(firstBit >= 0 && bitCount > 0);
	std::string s;
	while (firstBit >= 4)
	{
		s += '0';
		firstBit -= 4;
	}
	while (bitCount > 0)
	{
		// firstBit € [0, 3], bitCount > 0
		char nibble = 0;
		int lastBit = firstBit + bitCount - 1;
		for (int i = 3; i >= 0; i--)
		{
			nibble <<= 1;
			if (i >= firstBit && i <= lastBit)
				nibble |= 1;
		}
		bitCount -= 4 - firstBit;
		firstBit = 0;

		if (nibble <= 9)
			nibble += 48;		// 0..9
		else
			nibble += 97 - 10;	// a..f
		s = nibble + s;
	}
	return "0x" + s;
}

void generateVirtualPort(std::ostream & stream, const VirtualPort * p, const uc::AvrUC * controller)
{
	assert(p && controller);

	stream << "\t\tstruct\t\t// vport " << p->identifier << "\n\t\t{\n";


	{
		// Generate vport entries (non-function)

		stream << "\t\t\tunion\n\t\t\t{\n";
		
		for (std::list<PinBlock *>::const_iterator it = p->pinBlocks.begin(); it != p->pinBlocks.end(); it++)
			generatePinBlock(stream, *it, controller, "\t\t\t\t");

		stream << "\t\t\t};\n";
	}
	
	{
		// Generate vport access functions

		// Count total number of bits
		int bitCount = 0;
		for (std::list<PinBlock *>::const_iterator it = p->pinBlocks.begin(); it != p->pinBlocks.end(); it++)
			bitCount += (*it)->getPinCount();
		if (bitCount > 32)
			bitCount = 32;

		// generate code
		uc::PinType pt = uc::PIN;
		for (int i = 0; i < 3; i++)
		{
			if (i == 1) pt = uc::PORT; else if (i == 2) pt = uc::DDR;

			std::string capPinTypeString = capitalize(uc::getPinTypeString(pt));
			std::string lowPinTypeString = tolower(uc::getPinTypeString(pt));
			std::string dataTypeString = getDataType(bitCount == 1 ? 8 : bitCount);

			// get? function
			{
				stream << "\t\t\t" << dataTypeString << " get" << capPinTypeString
				       << "()\n\t\t\t{\n\t\t\t\tSyncPortmap(*this);\n\t\t\t\treturn ";
				int bit = 0;
				std::list<PinBlock *>::const_iterator it = p->pinBlocks.begin();
				for ( ; it != p->pinBlocks.end() || bit > bitCount; it++)
				{
					const int pinCount = (*it)->getPinCount();
					if (bit != 0)
						stream << " || ";
					stream << "(" << (*it)->identifier << "." << lowPinTypeString;
					if (bit > 0)
						stream << " << " << bit;
					stream << ")";
					bit += pinCount;
				}
				stream << ";\n\t\t\t}\n";
			}
			// set function
			{
				stream << "\t\t\t" << "void set" << capPinTypeString << "(" << dataTypeString << " i)\n\t\t\t{\n";
				int bit = 0;
				std::list<PinBlock *>::const_iterator it = p->pinBlocks.begin();
				for ( ; it != p->pinBlocks.end() || bit > bitCount; it++)
				{
					const int pinCount = (*it)->getPinCount();
					stream << "\t\t\t\t" << (*it)->identifier << "." << lowPinTypeString
					       << " = (i & " << getBitMask(bit, pinCount) << ")";
					if (bit > 0 && pinCount != 1)
						stream << " >> " << bit;
					stream << ";\n";
					bit += pinCount;
				}
				stream << "\t\t\t}\n";
			}
		}
	}

	stream << "\t\t} " << p->identifier << ";\n";
}


void generatePortmap(std::ostream & stream, const Portmap * pm)
{
	assert(pm);

	const uc::AvrUC * controller = pm->targetController;
	if (!controller)
		controller = curPGP->targetController;

	stream << "struct " << pm->identifier << "\t\t// portmap for " << controller->name << "\n{\n";

	if (!pm->properties.empty())
	{
		// Generate properties
		stream << "\tenum { ";

		const int count = pm->properties.size();

		std::list<Property *>::const_iterator it =  pm->properties.begin();
		for (int i = count - 1; i > 0 ; it++, i--)
			stream << (*it)->identifier << " = " << (*it)->value << ", ";

		stream << (*it)->identifier << " = " << (*it)->value << " };\n";
	}

	stream << "\tunion\n\t{\n";
	{
		// Generate top level pins
		for (std::list<PinBlock *>::const_iterator it =  pm->pinBlocks.begin(); it != pm->pinBlocks.end() ; it++)
			generatePinBlock(stream, *it, controller);

		// Generate vports
		for (std::list<VirtualPort *>::const_iterator it =  pm->vports.begin(); it != pm->vports.end() ; it++)
			generateVirtualPort(stream, *it, controller);
	}

	stream << "\t};\n};\n";
}
 

void generate()
{
	std::ostream * oFile;
	std::ofstream dummy;
	
	if (curPGP->oFilename.empty())
	{
		oFile = &std::cout;
		if (curPGP->verboseLevel)
			std::clog << "Writing output to stdout..." << std::endl;
	}
	else
	{
		dummy.open(curPGP->oFilename.c_str());
		oFile = &dummy;
		if (curPGP->verboseLevel)
			std::clog << "Writing output file " << curPGP->oFilename << "..." << std::endl;
	}

	// Merge portmaps and copy sections together using sequence numbers
	std::list<Portmap *>::const_iterator pmIt = curPGP->portmaps.begin();
	std::list<CopySection *>::const_iterator csIt = curPGP->copySections.begin();

	(*oFile) << "// THIS FILE WAS GENERATED FROM FILE \"" << curPGP->iFilename << "\" BY AVR-HALIB PORTMAP GENERATOR " << VERSION
	         << "\n// DO NOT EDIT THIS FILE CAUSE CHANGES MAY BE OVERWRITTEN. CHANGE \"" << curPGP->iFilename << "\" INSTEAD!"
	         << "\n#include \"avr-halib/avr/portmap.h\"\n";

	while (pmIt != curPGP->portmaps.end() && csIt != curPGP->copySections.end())
	{
		Portmap * pm = *pmIt;
		CopySection * cs = *csIt;
		if (pm->sequenceNumber < cs->sequenceNumber)
		{
			generatePortmap(*oFile, pm);
			pmIt++;
		}
		else
		{
			(*oFile) << cs->text;				// Copy section
			csIt++;
		}
	}

	while (pmIt != curPGP->portmaps.end())
	{
		Portmap * pm = *pmIt;
		generatePortmap(*oFile, pm);
		pmIt++;
	}

	while (csIt != curPGP->copySections.end())
	{
		CopySection * cs = *csIt;
		(*oFile) << cs->text;
		csIt++;
	}

}

}	// namespace generate
