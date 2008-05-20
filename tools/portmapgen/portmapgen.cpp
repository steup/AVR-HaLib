/**
 *	\file	portmapgen.cpp
 *	\brief	Main source file (main function, command line parsing, usage, debug output)
 *	\author	Philipp Werner
 *	\date	20.05.2008
 *
 */

/*

Todo:

TODO: portmaps mit gleichem bezeichner, aber anderem controller nicht beanstanden
TODO: Speicher korrekt freigegeben (delete auf Listen-Eintraege in Portmap und PortmapGenerationProcess)
TODO: __pad? private
TODO: Eingabe von cin

Doku: vports auf 32 bit beschränkt

*/


#include "portmapgen.h"

#include "parse.h"
#include "uc.h"
#include "generate.h"

#include <iostream>

#define NAME "portmapgen"
#define VERSION "0.1"
#define INFOSTRING "This is portmap generator for avr hardware abstraction library (halib), version " VERSION ".\n"

PortmapGenerationProcess * curPGP = 0;


void libcError(std::string s)
{
	perror(s.c_str());
	exit(1);
}


#ifdef DEBUG_PRINT_PARSE_RESULT

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

#endif	// DEBUG_PRINT_PARSE_RESULT



void printUsage()
{
	std::cout << INFOSTRING
	          << "\n"
	             "Usage:  " NAME " [OPTION]... [INPUT-FILE]\n"
	             "\n"
	             "Options:\n"
	             "\n"
	             "  -o FILE      Specify output file name. If missing, use stdout.\n"
	             "  -m CTRL      Generate portmap code for microcontroller CTRL for portmaps which don't\n"
	             "               use keyword \'for\'.\n"
	             "  -v           Verbose (say what's done)\n"
	             "\n"
	             "  -l           Show list of CTRLs that can be used for option -m CTRL.\n"
	             "  -h           Show this help.\n"
	             "\n"
	             "\n" << std::endl;
}	


int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		std::cerr << "Missing arguments... try " NAME " -h for help." << std::endl;
		return 1;
	}
	
	// Default portmap generation parameters
	PortmapGenerationProcess pgp;
	pgp.iFilename = "";
	pgp.oFilename = "";
	pgp.verboseLevel = 0;
	pgp.targetController = 0;
	
	// Parse command line params
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-o") == 0)
		{
			// Output file should be next argument
			if (++i >= argc)
			{
				std::cerr << "Missing filename behind \'-o\'!" << std::endl;
				exit(1);
			}
			pgp.oFilename = argv[i];
		}
		else if (strcmp(argv[i], "-m") == 0)
		{
			// Target microcontroller should be next argument
			if (++i >= argc)
			{
				std::cerr << "Missing target microcontroller behind \'-m\'! "
				             "Try \'-l\' to get a list of supported controllers. " << std::endl;
				exit(1);
			}
			pgp.targetController = uc::getTargetController(argv[i]);
			if (!pgp.targetController)
			{
				std::cerr << "Microcontroller target \'" << argv[i] << "\' currently not supported. "
				             "Have a look to uc.cpp to change this... ;-)" << std::endl;
			}
		}
		else if (strcmp(argv[i], "-v") == 0)
		{
			pgp.verboseLevel = 1;
		}
		else if (strcmp(argv[i], "-l") == 0)
		{
			std::cout << "These avr mircocontrollers are currently supported:\n" << uc::getSupportedControllers()
			          << std::endl;
			return 0;
		}
		else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "--usage") == 0)
		{
			printUsage();
			return 0;
		}
		else if (i == argc - 1)
		{
			pgp.iFilename = argv[i];
		}
		else
		{
			std::cerr << "Invalid argument \'" << argv[i] << "\'! Try \'" NAME " -h\' for getting help." << std::endl;
			return 1;
		}
	}

	if (pgp.iFilename.empty())
	{
		std::cerr << "No input! Specify file as last argument!" << std::endl;
		return 1;
	}
	
	curPGP = & pgp;

	if (pgp.verboseLevel)
		std::clog << INFOSTRING << std::endl;

	parse::parse();

#ifdef DEBUG_PRINT_PARSE_RESULT
 	std::cout << curPGP << std::endl;	
#endif

	if (!curPGP->parseErrorNum)
		generate::generate();

	return 0;
}
