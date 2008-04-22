// TODO: Ausschluss, dass portmap-keyword teil eines worts

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




*/


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include <vector>
#include <list>
#include <map>

#define VERSION "avr-pmgen 0.05"


using namespace std;


/*
enum PinType { NONE = 0, PIN = 1, PORT = 2, DDR = 3 };

struct Port2Mem
{
	char	portChar;
	PinType	pinType;
	int	address;
};

struct Device
{
	Port2Mem  p2m [];
	const char * portRange; 
};

Device device_atmega32 =
{
	{
		{ 'd',	PIN,	0x30 },
		{ 'd',	DDR,	0x31 },
		{ 'd',	PORT,	0x32 },
		{ 'c',	PIN,	0x33 },
		{ 'c',	DDR,	0x34 },
		{ 'c',	PORT,	0x35 },
		{ 'b',	PIN,	0x36 },
		{ 'b',	DDR,	0x37 },
		{ 'b',	PORT,	0x38 },
		{ 'a',	PIN,	0x39 },
		{ 'a',	DDR,	0x3a },
		{ 'a',	PORT,	0x3b },
		{ 0, NONE, 0 }
	},
	"a..d"
};

Device device_at90can128 [] =
{
	{
		{ 'a',	PIN,	0x20 },
		{ 'a',	DDR,	0x21 },
		{ 'a',	PORT,	0x22 },
		{ 'b',	PIN,	0x23 },
		{ 'b',	DDR,	0x24 },
		{ 'b',	PORT,	0x25 },
		{ 'c',	PIN,	0x26 },
		{ 'c',	DDR,	0x27 },
		{ 'c',	PORT,	0x28 },
		{ 'd',	PIN,	0x29 },
		{ 'd',	DDR,	0x2a },
		{ 'd',	PORT,	0x2b },
		{ 'e',	PIN,	0x2c },
		{ 'e',	DDR,	0x2d },
		{ 'e',	PORT,	0x2e },
		{ 'f',	PIN,	0x2f },
		{ 'f',	DDR,	0x30 },
		{ 'f',	PORT,	0x31 },
		{ 0, NONE, 0 }
	},
	"a..f"
};
*/



/*
options { -q -r0 }

*/


struct Options
{
	const char * iFilename;
	const char * oFilename;
	int verboseLevel;
	int outputLength;
	
	int countOfPorts;	// Ports number
	int countOfPins;	// Pins number per port
};

Options options = { "", "", 1, 0, 6, 8 };



namespace structs
{
}






struct Token
{
	enum { unknown, leftBrace, rightBrace, colon, semicolon, minus, number, identifier } type;
	string text;
};


class Tokenizer
{
	int  currentLineNumber;
	bool insidePortmapBlock;
	int  braceDepth;

public:
	Tokenizer() : 
		currentLineNumber(1),
		insidePortmapBlock(false),
		braceDepth(0)
	{
	}

	int getCurLine()
	{
		return currentLineNumber;
	}

	istream & getToken(istream & stream, Token & token);
};

Tokenizer tokenizer;


// returns next Token (inside interesting blocks, outside return content of the portmap definition file litarally)
istream & Tokenizer::getToken(istream & stream, Token & token)
{
	char c;
	token.text = "";
	token.type = Token::unknown;

	if (!insidePortmapBlock)
	{
		bool commentStarted = false;
		while (stream.get(c))
		{
			token.text += c;

			if (c == '/')
			{
				if (!commentStarted)
					commentStarted = true;	// frist comment character found
				else
				{
					// line comment, copy till the end
					while (stream.get(c))
					{
						token.text += c;
						if (c == '\n')
							break;
					}
					commentStarted = false;
				}
				
			}
			else if (c == '*')
			{
				if (commentStarted)
				{
					// block comment, copy till the end
					while (stream.get(c))
					{
						token.text += c;
						if (token.text[token.text.length() - 2] == '*' && token.text[token.text.length() - 1] == '/')
							break;
					}
					commentStarted = false;
				}
			}
			else if (c == '\"')
			{
				// String, copy till the end
				while (stream.get(c))
				{
					token.text += c;
					if (c == '\"')
						break;
				}
				commentStarted = false;
			}
			else if (c == 'p' && token.text.length() >= 7)
			{
				// look for "portmap" keyword
				if (strcmp(token.text.c_str() + token.text.length() - 7, "portmap") == 0)
				{
					insidePortmapBlock = true;
					token.text = token.text.substr(0, token.text.length() - 7);
					break;		// Return long unknown "token"
				}
// 				stream.putback('p');
// 				stream.putback('o');
// 				stream.putback('r');
// 				stream.putback('t');
// 				stream.putback('m');
// 				stream.putback('a');
// 				stream.putback('p');
				stream.putback('!');
				stream.putback('!');
				stream.putback('!');
				stream.putback('!');
				stream.putback('!');
				stream.putback('!');
				stream.putback('!');

				commentStarted = false;
			}
		}
	}
	else	// inside portmap block
	{
		// Eat whitespaces
		while (stream.get(c))
		{
			if (c == '#')
			{
				// Ignore comments
				while (stream.get(c))
					if (c == '\n')
				{
					currentLineNumber++;
					break;
				}
			}
			else if (c == '\n')
				currentLineNumber++;
			if (!isspace(c))
			{
				stream.putback(c);
				break;
			}
		}
	
		if (stream.get(c))
		{
			token.text = c;
			if (c == '{')
			{
				token.type = Token::leftBrace;
				braceDepth++;
			}
			else if (c == '}')
			{
				token.type = Token::rightBrace;
				braceDepth--;
				if (braceDepth <= 0)
					insidePortmapBlock = false;
			}
			else if (c == ':')
				token.type = Token::colon;
			else if (c == ';')
				token.type = Token::semicolon;
			else if (c == '-')
				token.type = Token::minus;
			else if (isdigit(c))
			{
				// number
				token.type = Token::number;
				
				while (stream.get(c))
				{
					if (!isdigit(c) && c != '.')
					{
						stream.putback(c);
						break;
					}
					else
						token.text += c;
				}
			}
			else if (isalpha(c) || c == '_')
			{
				// identifier or keyword
				token.type = Token::identifier;
				
				while (stream.get(c))
				{
					if (!isalnum(c))
					{
						stream.putback(c);
						break;
					}
					else
						token.text += c;
				}
			}
		}
	}

	// Tokenizer debug output
	if (!token.text.empty())
		cout << "Token: ’" << token.text << "’" << endl;
	
	// no stream error if we will return something!
	if (!token.text.empty())
		stream.clear();
	
	return stream;
}





/*
istream & operator>>(istream & stream, Options & opt)
{
	Token t;
	do
	{
		stream >> t;
	}
	while (t.type != Token::rightBrace);
	
	return stream;
}
*/

void libcError(const char * s)
{
	perror(s);
	exit(1);
}

/*
void parseError(string s, int line = 0)
{
	cerr << options.iFilename << ":" << currentLineNumber << ": error: " << s;
	if (line)
		cerr << line;
	cerr << endl;
	exit(1);
}





struct PortMapEntry
{
	string	identifier;
	int	line;
	char	port       // Port character
	int	firstPin;  // first pin number
	int	lastPin;   // last pin number

	bool isUsingPin(char Port, int Pin)
	{
		return Port == port && Pin >= firstPin && Pin <= lastPin;
	}

	int getPinCount()
	{
		return lastPin - firstPin + 1;
	}

};


string toupper(string s)
{
	for (int i = s.length() - 1; i >= 0; i--)
		s[i] = toupper(s[i]);
	return s;
}

void PortMapEntry::generateCpp(ostream & stream, PinType pt)
{
	static const char * pts [] = { "", "pin", "port", "ddr" };

	int pinCount = getPinCount();
	string dataType;
	string pinType = pts[pt];
	
	if (pinCount == 1)
		dataType = "bool";
	else if (pinCount <= 8)
		dataType = "uint8_t";
	else if (pinCount <= 16)
		dataType = "uint16_t";
	else if (pinCount <= 32)
		dataType = "uint32_t";
	else
		dataType = "uint64_t";

	stream << "\tvolatile " << dataType << " " << pinType << identifier << ": " << pinCount << ";\t\t// " << toupper(pinType) << (char)toupper(port) << ", bit " << firstPin;

	if (lastPin != firstPin)
		stream << "-" << lastPin;

	stream << "\n";
}



class VirtualPort
{
	list<const PortMapEntry *> pins;
	string identifier;
	int line;
public:
	VirtualPort(string id) :
		identifier(id),
		line(currentLineNumber)
	{}
		
	void add(const PortMapEntry * p)
	{
		pins.push_back(p);
	}

	int getLine()
	{
		return line;
	}
};




class PortMap
{
	map<string, PortMapEntry *> entries;
	map<string, VirtualPort *> vports;

	string identifier;
	int line;
public:
	Init portmap from stream ("{ ... }")
	PortMap(string id, istream & stream);

	Write portmap into header file
	void generateCpp(ostream & stream);

	Check that pin(s) identifier is unique in this portmap
	void _checkEntryId(string id)
	{
		map<string, PortMapEntry *>::iterator it = entries.find(id);
		if (it != entries.end())
			parseError("identifier ’" + id + "’ already used, see line ", it->second->getLine());
	}

	void _checkPort(const char port)
	{
		if (port < 'a' || port >= 'a' + options.countOfPorts)
		{
			ostringstream s;
			s << "no port ’" << port << "’ available on this chip, valid: a.." << (char)('a' + options.countOfPorts - 1);
			parseError(s.str());
		}
	}

	void _checkPin(int pin)
	{
		if (pin < 0 || pin >= options.countOfPins)
		{
			ostringstream s;
			s << "’" << pin << "’ is no valid pin number on this chip, valid: 0.." << (options.countOfPins - 1);
			parseError(s.str());
		}
	}

	void checkAndAddEntry(string id, char port, int pin1, int pin2)
	{
		Check that identifier is not used yet
		_checkEntryId(id);

		Test if values are valid
		_checkPort(port);
		_checkPin(pin1);
		_checkPin(pin2);
		if (pin1 > pin2)
		{
			ostringstream s;
			s << "in ’" << pin1 << "-" << pin2 << "’: second number smaller than first";
			parseError(s.str());
		}

		Test if pins are already in use
		map<string, PortMapEntry *>::const_iterator it = entries.begin();
		while (it != entries.end())
		{
			for (int i = pin1; i <= pin2; i++)
				if (it->second->isUsingPin(port, i))
				{
					ostringstream s;
					s << "pin ’" << port << i << "’ already used in this portmap, see line " << it->second->getLine();
					parseError(s.str());
				}
			
			it++;
		}

		entries[id] = new PortMapEntry(id, port, pin1, pin2);
	}


	Check that virtual port identifier is unique in this portmap
	void checkVPort(string id)
	{
		map<string, VirtualPort *>::iterator it = vports.find(id);
		if (it != vports.end())
			parseError("identifier ’" + id + "’ already used, see line ", it->second->getLine());
	}

	int getLine()
	{
		return line;
	}
};

PortMap::PortMap(string id, istream & stream)
{
	Token t;
	
	identifier = id;
	line = currentLineNumber;
	
	stream >> t;
	if (t.type != Token::leftBrace)
		parseError("expected {, found ’" + t.text + "’");
	
	Syntax:	<vport-id>{...
	Syntax:	<identifier>: <port-character> <number> ;
	Syntax:	<identifier>: <port-character> <number> - <number> ;
	tokenIdx:         0      1         2           3     4    5     6
	int tokenIdx = 0;
	VirtualPort * currentVPort = 0;
	string identifier;
	char portCharacter = 0;
	int pinNum1 = 0;
	int pinNum2 = 0;
	while (1)
	{
		stream >> t;
		
		if (!stream)
			parseError("missing }");
		
		} (portmap block closed) ?
		if (t.type == Token::rightBrace && !currentVPort && tokenIdx == 0)
			break;
		
		switch (tokenIdx)
		{
			case 0:
				if (t.type == Token::rightBrace && currentVPort != 0)
				{
					} (end of vport block)
					currentVPort = 0;
					break;
				}
				if (t.type != Token::identifier)
					parseError("expected identifier or keyword, found ’" + t.text + "’");
				if (t.text == "vport")
				{
					virtual port / pin group
					stream >> t;
					if (t.type != Token::identifier)
						parseError("expected identifier behind vport, found ’" + t.text + "’");

					checkVPort(t.text);

					currentVPort = new VirtualPort(t.text);
					vports[t.text] = currentVPort;
					
					stream >> t;
					if (t.type != Token::leftBrace)
						parseError("expected { behind vport, found ’" + t.text + "’");
				}
				else
				{
					<identifier>
					identifier = t.text;
					tokenIdx++;
				}
				break;
			case 1:
				:
				if (t.type != Token::colon)
					parseError("expected colon behind ’" + identifier + "’, found ’" + t.text + "’");
				tokenIdx++;
				break;
			case 2:
				<port-character>
				if (t.type != Token::identifier || t.text.length() != 1 || !isalpha(t.text[0]))
					parseError("expected charcter identifying port (a, b, c...), found ’" + t.text + "’");
				portCharacter = tolower(t.text[0]);
				tokenIdx++;
				break;
			case 3:
				<number>
				if (t.type != Token::number)
					parseError("expected integer identifying pin of the port (0, 1, 2...), found ’" + t.text + "’");
				pinNum1 = atoi(t.text.c_str());
				tokenIdx++;
				break;
			case 4:
				if (t.type == Token::semicolon)
				{
					; (end of pin declaration)
					checkAndAddEntry(identifier, portCharacter, pinNum1, pinNum1);
					tokenIdx = 0;
				}
				else if (t.type == Token::minus)
				{
					- (more than one pin...)
					tokenIdx++;
				}
				else
					parseError("expected ; or -, found ’" + t.text + "’");
				break;
			case 5:
				<number>
				if (t.type != Token::number)
					parseError("expected integer identifying pin of the port (0, 1, 2...), found ’" + t.text + "’");
				pinNum2 = atoi(t.text.c_str());
				tokenIdx++;
				break;
			case 6:
				if (t.type == Token::semicolon)
				{
					; (end of pins declaration)
					checkAndAddEntry(identifier, portCharacter, pinNum1, pinNum2);
					tokenIdx = 0;
				}
				else
					parseError("expected semicolon, found ’" + t.text + "’");
				break;
		}
	}
}

void PortMap::generateCpp(ostream & stream)
{
	Generate a map of pins

	int bits = 3 * options.countOfPorts * options.countOfPins;

	PortMapEntry::PinType * pinType = new PortMapEntry::PinType [bits];	// Type of pin: PIN, DDR, PORT ('p')
	PortMapEntry ** pinData = new PortMapEntry* [bits];			// Data of pin
	
	for (int i = 0; i < bits; i++)
	{
		pinType[i] = PortMapEntry::NONE;
		pinData[i] = 0;
	}

	
	Generate map
	map<string, PortMapEntry *>::const_iterator it = entries.begin();
	for (; it != entries.end(); it++)
	{
		Generate 3 entries per pin/pin group (pin, ddr, port)
		PortMapEntry * p = it->second;
		int port = p->getPort() - 'a';
		int pin = p->getFirstPin();

		int pinIdx = port * 3 * options.countOfPins + pin;
		pinType[pinIdx] = PortMapEntry::PIN;
		pinData[pinIdx] = p;
		pinIdx += options.countOfPins;
		pinType[pinIdx] = PortMapEntry::DDR;
		pinData[pinIdx] = p;
		pinIdx += options.countOfPins;
		pinType[pinIdx] = PortMapEntry::PORT;
		pinData[pinIdx] = p;
	}


	stream	<< "struct " << identifier << "\n"
		<< "{\n";

	
	int lastPinSeqNum = 0;		// absolute number of the first pin nothing was written about
	for (int i = 0; i < bits; )
	{
		static const char * pts [] = { "PIN", "DDR", "PORT" };
		char port = 'A' + i / (3 * options.countOfPins);
		int pin = i % options.countOfPins;
		int type = (i / options.countOfPins) % 3;

		switch (options.outputLength)
		{
			case 1:
				Generate readable, but long output
				if (pinType[i] == PortMapEntry::NONE)
				{
					
					stream << "\tvolatile bool : 1;\t\t// " << pts[type] << port << ", bit " << pin << "\n";
					i++;
				}
				else
				{
					pinData[i]->generateCpp(stream, pinType[i]);
					i += pinData[i]->getPinCount();
				}
				break;
			default:
				Generate short output
				if (pinType[i] != PortMapEntry::NONE)
				{
					int d = i - lastPinSeqNum;
					if (d)
					{
						Fill the gap from last named pin to this pin
						stream << "\tvolatile bool : " << d << ";\n";
						lastPinSeqNum = i;
					}

					pinData[i]->generateCpp(stream, pinType[i]);

					lastPinSeqNum += pinData[i]->getPinCount();
					i += pinData[i]->getPinCount();
				}
				else
				{
					i++;
				}
				break;
		}
	}
	
	stream << "};\n\n";
	
	delete [] pinData;
	delete [] pinType;
}



*/
void generate()
{
// 	map<string, PortMap *> portMaps;
	
	{
		// Parse config file
		ifstream iFile(options.iFilename, ifstream::in);

		if (!iFile)
			libcError(options.iFilename);
	
		if (options.verboseLevel)
			clog << "Parsing config file " << options.iFilename << endl;
	
		Token token;
		while (tokenizer.getToken(iFile, token))
		{
		//	cout << "\"" << token.text << "\"" << endl;
			/*if (token.text == "options")
			{
				iFile >> options;
			}
			else if (token.text == "portmap")
			{
				iFile >> token;
				if (token.type != Token::identifier)
					parseError("expected identifier behind portmap, found ’" + token.text + "’");

				Check if identifier is already used
				map<string, PortMap *>::iterator it = portMaps.find(token.text);
				if (it != portMaps.end())
					parseError("identifier ’" + token.text + "’ already used, see line ", it->second->getLine());

				PortMap * pm = new PortMap(token.text, iFile);
				portMaps[token.text] = pm;
			}
			else
			{
				parseError("expected top level keyword, found ’" + token.text + "’");
			}*/
		}
	}

// 	{
// 		Generate header file
// 		ofstream oFile();
// 		ostream & oFile = cout;
// 
// 		oFile	<<	"/* PortMap header for avr-halib\n"
// 				" * generated by " << VERSION << " from " << options.iFilename << "\n"
// 				" * \n"
// 				" * Do not modify this file unless you really know what you are doing!"
// 				" * Change" << options.iFilename << " ...\n"
// 				" * \n"
//  				" */\n\n"
// 				"#include <stdint.h>\n\n";
// 
// 		for (map<string, PortMap *>::iterator it = portMaps.begin(); it != portMaps.end(); it++)
// 			it->second->generateCpp(oFile);
// 
// 		oFile	<<	"\n\n" << flush;
// 	}
}

int main(int argc, char * argv[])
{
	options.iFilename = "test.portmap";
//	options.outputLength = 0;
	
	generate();
	return 0;
}

void printUsage()
{
	
}





