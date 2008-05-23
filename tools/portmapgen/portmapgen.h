/**
 *	\file	portmapgen.h
 *	\brief	Main header file (common data structures, #defines for compile time configuration)
 *	\author	Philipp Werner
 *	\date	20.05.2008
 *
 */


#pragma once

// Useful debug output (uncomment to get it)
// #define DEBUG_TOKEN_OUTPUT
// #define DEBUG_PARSE_LEVEL
// #define DEBUG_PRINT_PARSE_RESULT

// Define to disable asserts
// #define NDEBUG

#include <cassert>
#include <string>
#include <list>

#include "uc.h"


#define NAME "portmapgen"
#define VERSION "0.1"


void libcError(std::string s);

struct PinBlock;
struct VirtualPort;
struct Property;
struct Portmap;
struct CopySection;
struct PortmapGenerationProcess;


struct PinBlock
{
	Portmap * parentPortmap;
	std::string	identifier;
	int	line;

	char	port;       // Port character
	int	firstPin;  // first pin number
	int	lastPin;   // last pin number

	int getPinCount() const
	{
		return lastPin - firstPin + 1;
	}
};


struct VirtualPort
{
	Portmap * parentPortmap;

	std::string identifier;
	int line;

	std::list<PinBlock *> pinBlocks;
};


struct Property
{
	std::string identifier;
	int line;
	std::string value;
};



struct Portmap
{
	int sequenceNumber;

	std::string identifier;
	int line;

	const uc::AvrUC * targetController;
	std::list<Property *> properties;
	std::list<PinBlock *> pinBlocks;
	std::list<VirtualPort *> vports;
};


struct CopySection
{
	int sequenceNumber;
	std::string text;
};


struct PortmapGenerationProcess
{
	// Config
	std::string iFilename;
	std::string oFilename;
	int verboseLevel;
	const uc::AvrUC * targetController;

	// Init by parse::parse()
	int parseErrorNum;
	std::list<Portmap *> portmaps;
	std::list<CopySection *> copySections;
};


extern PortmapGenerationProcess * curPGP;




