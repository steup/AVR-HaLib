// TODO: Ausschluss, dass Portmap-keyword teil eines worts

// groups/virtual ports
// todo: doppelte pinbelegungen, identifier finden
// map<std::string, ...> <- identifier-map statt liste
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

TODO: check bezeichner in gesamter portmap, portmaps mit gleichem bezeichner, aber anderem controller nicht beanstanden
*/
#pragma once

#include "uc.h"

#include <string>
#include <list>

#define VERSION "avr-pmgen 0.05"

// Useful debug output (uncomment to get it)
// #define DEBUG_TOKEN_OUTPUT
// #define DEBUG_PARSE_LEVEL

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

// 	bool isUsingPin(char Port, int Pin)
// 	{
// 		return Port == port && Pin >= firstPin && Pin <= lastPin;
// 	}

	int getPinCount()
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




