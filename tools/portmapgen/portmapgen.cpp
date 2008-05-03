// TODO: Ausschluss, dass Portmap-keyword teil eines worts

// groups/virtual ports
// todo: doppelte pinbelegungen, identifier finden
// map<string, ...> <- identifier-map statt liste
// todo: Comments einbetten, die übertragen werden
//very short output, not good for hand-editing
/*

Todo:

Command-Line-Parameter

#pragma once
Architektur-Überprüfung, PINA rausnehmen
Liste der implementierten Comtroller
option: Architektur,
Erweiterung: für jede Portmap Architektur angebbar

TODO: portmaps mit gleichem bezeichner, aber anderem controller nicht beanstanden
TODO: Speicher korrekt freigegeben (listen!)


*/


#include "portmapgen.h"

#include "parse.h"
#include "uc.h"

#include <iostream>

#define VERSION "avr-pmgen 0.05"


PortmapGenerationProcess * curPGP = 0;


void libcError(std::string s)
{
	perror(s.c_str());
	exit(1);
}


void printUsage()
{
	
}






std::ostream & operator<< (std::ostream & s, const PinBlock * p)
{
	if (p->firstPin == p->lastPin)
		s << "pin " << p->identifier << ": " << p->port << " " << p->firstPin << ";";
	else
		s << "pins " << p->identifier << ": " << p->port << " " << p->firstPin << "-" << p->lastPin << ";";
	s << "\t(portmap " << p->parentPortmap->identifier << ", line " << p->line << ")";
	return s;
}

std::ostream & operator<< (std::ostream & s, const Property * p)
{
	s << "property " << p->identifier << " = " << p->value << ";\t(line " << p->line << ")";
	return s;
}

std::ostream & operator<< (std::ostream & s, const VirtualPort * p)
{
	s << "\tvport " << p->identifier << " { \t\t(portmap " << p->parentPortmap->identifier << ",line " << p->line << ")\n";
	
	std::list<PinBlock *>::const_iterator i = p->pinBlocks.begin();
	for (; i != p->pinBlocks.end(); i++)
	{
		s << "\t\t" << *i << "\n";
	}
	s << "\t};" << std::endl;
	return s;
}

std::ostream & operator<< (std::ostream & s, const Portmap * p)
{
	s << "portmap " << p->identifier;
	if (p->targetController)
		s << " for " << p->targetController->name;
	s << " { \t\t(# " << p->sequenceNumber << ",line " << p->line << ")\n";


	{
		std::list<Property *>::const_iterator i = p->properties.begin();
		for (; i != p->properties.end(); i++)
		{
			s << "\t" << *i << "\n";
		}
	}
	{
		std::list<PinBlock *>::const_iterator i = p->pinBlocks.begin();
		for (; i != p->pinBlocks.end(); i++)
		{
			s << "\t" << *i << "\n";
		}
	}
	{
		std::list<VirtualPort *>::const_iterator i = p->vports.begin();
		for (; i != p->vports.end(); i++)
		{
			s << *i << "\n";
		}
	}
	s << "};" << std::endl;
	return s;
}


std::ostream & operator<< (std::ostream & s, const CopySection * p)
{
	s << p->text;
	return s;
}


std::ostream & operator<< (std::ostream & s, const PortmapGenerationProcess * p)
{
	s << "= " << p->iFilename << " -> " << p->oFilename << " for " << p->targetController->name
	  << " (" << p->parseErrorNum << " errors) ======================" << std::endl;

	std::list<Portmap *>::const_iterator pmIt = p->portmaps.begin();
	std::list<CopySection *>::const_iterator csIt = p->copySections.begin();

	while (pmIt != p->portmaps.end() && csIt != p->copySections.end())
	{
		Portmap * pm = *pmIt;
		CopySection * cs = *csIt;
		if (pm->sequenceNumber < cs->sequenceNumber)
		{
			s << pm << "\n";
			pmIt++;
		}
		else
		{
			s << cs;
			csIt++;
		}
	}

	while (pmIt != p->portmaps.end())
	{
		Portmap * pm = *pmIt;
		s << pm << "\n";
		pmIt++;
	}

	while (csIt != p->copySections.end())
	{
		CopySection * cs = *csIt;
		s << cs;
		csIt++;
	}

	return s;
}





int main(int argc, char * argv[])
{
	PortmapGenerationProcess pgp;
	pgp.iFilename = "test.portmap";
	pgp.oFilename = "test.h";
	pgp.verboseLevel = 2;
	pgp.targetController = uc::getTargetController("AT90CAN128");

	if (!pgp.targetController)
		std::cerr << "Microcontroller target not supported" << std::endl;
	
	curPGP = & pgp;

	std::cout << curPGP << std::endl;	

	parse::parse();

	std::cout << curPGP << std::endl;	

	return 0;
}
