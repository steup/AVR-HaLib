/**
 *	\file	parse.cpp
 *	\brief	Parse portmap definitions
 *	\author	Philipp Werner
 *	\date	20.05.2008
 *
 */


#include "parse.h"

#include "portmapgen.h"
#include "uc.h"

#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace parse
{

Tokenizer tokenizer;


std::string getTokenString(TokenType t);


void Tokenizer::putBackToken(Token t)
{
	putBack = new Token;
	*putBack = t;
#ifdef DEBUG_TOKEN_OUTPUT
	std::cout << "Token ’" << t.text << "’ put back!" << std::endl;
#endif
}


// returns next Token (inside interesting blocks, outside return content of the portmap definition file litarally)
std::istream & Tokenizer::getToken(std::istream & stream, Token & token)
{
	if (putBack)
	{
		token = *putBack;
		delete putBack;
		putBack = 0;
#ifdef DEBUG_TOKEN_OUTPUT
		std::cout << "Token returned: ’" << token.text << "’" << std::endl;
#endif
		return stream;
	}

	char c;
	bool commentStarted = false;
	token.text = "";
	token.type = unknown;

	if (!insidePortmapBlock)
	{
		while (stream.get(c))
		{
			token.text += c;

			if (c == '\n')
				currentLineNumber++;
			else if (c == '/')
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
						{
							currentLineNumber++;
							break;
						}
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
						if (c == '\n')
							currentLineNumber++;
						if (token.text[token.text.length() - 2] == '*' && token.text[token.text.length() - 1] == '/')
							break;
					}
					commentStarted = false;
				}
			}
			else if (c == '\"')
			{
				// std::string, copy till the end
				while (stream.get(c))
				{
					token.text += c;
					if (c == '\n')
					{
						currentLineNumber++;
						break;
					}
					if (c == '\"')
						break;
				}
				commentStarted = false;
			}
			else if (c == 'p' && token.text.length() >= 7)
			{
				// look for "portmap" keyword
				if (strncmp(token.text.c_str() + token.text.length() - 7, "portmap", 7) == 0)
				{
					insidePortmapBlock = true;
					stream.unget().unget().unget().unget().unget().unget().unget();
					token.text = token.text.substr(0, token.text.length() - 7);
					break;		// Return long unknown "token"
				}

				commentStarted = false;
			}
		}
	}
	else	// inside portmap block
	{
		// Eat whitespaces and comments
		while (stream.get(c))
		{
			if (c == '/')
			{
				if (!commentStarted)
					commentStarted = true;	// frist comment character found
				else
				{
					// line comment
					while (stream.get(c) && c != '\n')
						;
					currentLineNumber++;
					commentStarted = false;
				}
				
			}
			else if (c == '*')
			{
				if (commentStarted)
				{
					static char lastChar = 0;
					// block comment, copy till the end
					while (stream.get(c))
					{
						if (c == '\n')
							currentLineNumber++;
						if (lastChar == '*' && c == '/')
							break;
						lastChar = c;
					}
					commentStarted = false;
				}
			}
			else if (c == '\n')
				currentLineNumber++;
			else if (!isspace(c))
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
				token.type = leftBrace;
				braceDepth++;
			}
			else if (c == '}')
			{
				token.type = rightBrace;
				braceDepth--;
			}
			else if (c == ';')
			{
				token.type = semicolon;
				if (braceDepth <= 0)
					insidePortmapBlock = false;
			}
			else if (c == ':')
				token.type = colon;
			else if (c == '-')
				token.type = minus;
			else if (c == '=')
				token.type = equals;
			else if (c == ',')
				token.type = comma;
			else if (isdigit(c))
			{
				// number
				token.type = number;
				
				bool isHex = false;
				while (stream.get(c))
				{
					if (!isdigit(c) && c != '.')
					{
						if (isHex && ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')))
						{
							// hex digits are okay
						}
						else if (token.text[0] == '0' && c == 'x')
							isHex = true;
						else
						{
							// no hexadecimal
							stream.putback(c);
							break;
						}
					}
					token.text += c;
				}
			}
			else if (isalpha(c) || c == '_')
			{
				// identifier or keyword
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

				if (token.text == "portmap")
					token.type = portmapKeyword;
				else if (token.text == "pin")
					token.type = pinKeyword;
				else if (token.text == "pins")
					token.type = pinsKeyword;
				else if (token.text == "vport")
					token.type = vportKeyword;
				else if (token.text == "for")
					token.type = forKeyword;
				else if (token.text == "property")
					token.type = propertyKeyword;
				else
					token.type = identifier;
			}
		}
	}

#ifdef DEBUG_TOKEN_OUTPUT
	if (!token.text.empty())
		std::cout << "Token returned: ’" << token.text << "’ (" << getTokenString(token.type) << ")" << std::endl;
#endif
	
	// no stream error if we will return something!
	if (!token.text.empty())
		stream.clear();
	
	return stream;
}


void parseError(std::string s, bool fatal, int line)
{
	curPGP->parseErrorNum++;

	std::cerr << curPGP->iFilename << ":" << tokenizer.getCurLine() << ": error: " << s;
	if (line)
		std::cerr << line;
	std::cerr << std::endl;

	if (curPGP->parseErrorNum >= 10)
	{
		std::cerr << curPGP->iFilename << ": Too many errors... stop." << std::endl;
		exit(1);
	}
	
	if (fatal)
		exit(1);
}

void Tokenizer::getNeededToken(std::istream & stream, Token & token)
{
	if (!getToken(stream, token))
		parseError("unexpeceted end of file", true);
}




void parseErrorCheckToken(std::string shallString, std::string behindString, std::string foundString)
{
	parseError("expected " + shallString + " behind " + behindString + ", found ’" + foundString + "’", false);
}


std::string getTokenString(TokenType t)
{
	static const char * s [] = {
		"unknown",
		"’{’",
		"’}’",
		"’:’",
		"’;’",
		"’-’",
		"’=’",
		"’,’",
		"number",
		"identifier",
		"keyword portmap",
		"keyword pin",
		"keyword pins",
		"keyword vport",
		"keyword for",
		"keyword property",
	};
	return s[t];
}

bool isKeyword(TokenType t)
{
	return t > portmapKeyword;
}


bool checkToken(const Token & token, TokenType shallBe, std::string shallString, std::string behind)
{
	if (token.type != shallBe)
	{
		parseErrorCheckToken(shallString, behind, token.text);
		return false;
	}
	return true;
}

bool checkToken(const Token & token, TokenType shallBe, std::string behind)
{
	return checkToken(token, shallBe, getTokenString(shallBe), behind);
}

bool checkToken(const Token & token, TokenType shallBe, TokenType behind)
{
	return checkToken(token, shallBe, getTokenString(shallBe), getTokenString(behind));
}


// Ensure that identifiers are unique in context l
template <class T>
bool checkIdentifierUnique(const std::list<T> & l, std::string id)
{
	typename std::list<T>::const_iterator i = l.begin();
	for (; i != l.end(); i++)
		if ((*i)->identifier == id)
		{
			parse::parseError("identifier ’" + id + "’ already used, see line ", false, (*i)->line);
			return false;
		}
	return true;
}

// Ensure that identifiers does not appear in context l (for vports, every identifier only once)
template <class T>
bool checkIdentfierNotIn(const std::list<T> & l, std::string id)
{
	typename std::list<T>::const_iterator i = l.begin();
	for (; i != l.end(); i++)
		if ((*i)->identifier == id)
		{
			parse::parseError("pin identifier ’" + id + "’ already used in this vport", false);
			return false;
		}
	return true;
}


template <class T>
T getObjPointerByIdentifier(const std::list<T> & l, std::string id)
{
	typename std::list<T>::const_iterator i = l.begin();
	for (; i != l.end(); i++)
		if ((*i)->identifier == id)
		{
			return *i;
		}
	parse::parseError("unknown identifier ’" + id + "’", false);
	return 0;
}



void checkForPinCollisions(const std::list<PinBlock *> & l)
{
	std::list<PinBlock *>::const_iterator first = l.begin();
	std::list<PinBlock *>::const_iterator second;
	while (first != l.end())
	{
		second = first;
		while (++second != l.end())		// Starting with element after first
		{
			PinBlock * p1 = *first;
			PinBlock * p2 = *second;
			if (p1->port == p2->port)
			{
				for (int i = p1->firstPin; i <= p1->lastPin; i++)
				{
					if (i >= p2->firstPin && i <= p2->lastPin)
					{
						curPGP->parseErrorNum++;
						std::cerr << curPGP->iFilename << ":" << p2->line << ": error: pin ’" << p1->port << i << "’ already used, see line " << p1->line << std::endl;
					}
				}
			}
		}
		first++;
	}
}





// Parse pin from stream (= <numberOrBool>;)
void parseProperty(Property * p, std::istream & stream)
{
	Token token;

	// =
	tokenizer.getNeededToken(stream, token);
	checkToken(token, equals, p->identifier);
	
	if (token.type == number || token.type == identifier)
		tokenizer.putBackToken(token);
	else if (token.type == semicolon)
		return;
	else if (token.type == rightBrace)
	{
		tokenizer.putBackToken(token);
		return;
	}

	// <numberOrBool>
	tokenizer.getNeededToken(stream, token);

	if (!(token.type == identifier && (token.text == "true" || token.text == "false")))
		checkToken(token, number, "’true’, ’false’ or number", getTokenString(equals));
	p->value = token.text;

	if (token.type == semicolon)
		return;
	else if (token.type == rightBrace)
	{
		tokenizer.putBackToken(token);
		return;
	}

	// ;
	tokenizer.getNeededToken(stream, token);

	checkToken(token, semicolon, p->value);
	if (token.type == rightBrace || isKeyword(token.type))
	{
		tokenizer.putBackToken(token);
		return;
	}
}


// Parse pin from stream  (<identifier>: <port-character> <number>;)
// Parse pins from stream (<identifier>: <port-character> <number> - <number>;)
void parsePinBlock(PinBlock * p, TokenType entryType, std::istream & stream)
{
	Token token;

	// :
	tokenizer.getNeededToken(stream, token);
	checkToken(token, colon, p->identifier);

	if (token.type == identifier || token.type == number)
		tokenizer.putBackToken(token);

	// <port-character>
	tokenizer.getNeededToken(stream, token);

	if (token.type != identifier || token.text.length() != 1 || !isalpha(token.text[0]))
		parseError("expected port character (a, b, c...), found ’" + token.text + "’", false);
	else
	{
		const uc::AvrUC * c = p->parentPortmap->targetController;
		if (!c)
			c = curPGP->targetController;
		uc::checkPort(token.text[0], c);
	}

	if (token.type == semicolon)
		return;		// Parse error, assume that next semantic element starts behind ";"
	else if (token.type == number)
		tokenizer.putBackToken(token);
	else if (token.type == rightBrace)
	{
		tokenizer.putBackToken(token);
		return;
	}

	p->port = tolower(token.text[0]);
			
	// <number>
	tokenizer.getNeededToken(stream, token);
	
	if (checkToken(token, number, "port character"))
	{
		p->firstPin = atoi(token.text.c_str());
		uc::checkPin(p->firstPin);
	}
	else
		p->firstPin = 0;

	p->lastPin = p->firstPin;

	if (token.type == semicolon)
		return;		// Parse error, assume that next semantic element starts behind ";"
	if (token.type == rightBrace)
	{
		tokenizer.putBackToken(token);
		return;
	}

	if (entryType == pinsKeyword)
	{
		// -
		tokenizer.getNeededToken(stream, token);
		checkToken(token, minus, p->identifier);

		if (token.type == number)
			tokenizer.putBackToken(token);
		else if (token.type == semicolon)
			return;
		else if (token.type == rightBrace)
		{
			tokenizer.putBackToken(token);
			return;
		}

		// <number>
		tokenizer.getNeededToken(stream, token);
	
		if (checkToken(token, number, minus))
		{
			p->lastPin = atoi(token.text.c_str());
			uc::checkPin(p->lastPin);
		}
		else
			p->lastPin = 0;

		if (p->lastPin && p->firstPin && p->firstPin > p->lastPin)
		{
			parseError("last pin number smaller than first pin number", false);
		}

		if (token.type == semicolon)
			return;		// Parse error, assume that next semantic element starts behind ";"
		if (token.type == rightBrace)
		{
			tokenizer.putBackToken(token);
			return;
		}
	}

	// ;
	tokenizer.getNeededToken(stream, token);
	
	checkToken(token, semicolon, "pin number");
	if (token.type == rightBrace || isKeyword(token.type))
	{
		tokenizer.putBackToken(token);
		return;
	}
}



// Parse vport from stream ("{ <comma-seperated-identifier-list };")
void parseVirtualPort(VirtualPort * vp, std::istream & stream)
{
	Token token;

	//  {	
	tokenizer.getNeededToken(stream, token);
	checkToken(token, leftBrace, vp->identifier);

	// parse rest
	bool vportEnd = false;
	while (1)
	{
		tokenizer.getNeededToken(stream, token);

		if (token.type == rightBrace && !vportEnd)
		{
			vportEnd = true;
		}
		else if (token.type == semicolon && vportEnd)
		{
			break;
		}
		else if (token.type == identifier)
		{
			// <identifier>
			PinBlock * pb = getObjPointerByIdentifier(vp->parentPortmap->pinBlocks, token.text);
			if (pb && checkIdentfierNotIn(vp->pinBlocks, token.text))
				vp->pinBlocks.push_back(pb);
			std::string behind = token.text;

			// , or }
			tokenizer.getNeededToken(stream, token);
			if (token.type == rightBrace || (!checkToken(token, comma, behind) && token.type == identifier))
				tokenizer.putBackToken(token);
		}
		else
		{
			if (vportEnd)
			{
				parseErrorCheckToken(getTokenString(semicolon), getTokenString(rightBrace), token.text);
				tokenizer.putBackToken(token);
				break;
			}
			else
			{
				parseError("expected " + getTokenString(identifier) + ", found ’" + token.text + "’", false);
				while(token.type != semicolon)
				{
					tokenizer.getNeededToken(stream, token);
				}
			}
		}
	}
}


// Parse portmap from stream ("[for <controller>] { ... };")
void parsePortmap(Portmap * pm, std::istream & stream)
{
	Token token;

	// for <controller> or {	
	tokenizer.getNeededToken(stream, token);
	pm->targetController = 0;
	if (token.type == forKeyword)
	{
		tokenizer.getNeededToken(stream, token);
		if (token.type == identifier)
		{
			const uc::AvrUC * controller = uc::getTargetController(token.text);
			if (!controller)
				parseErrorCheckToken("microcontroller name", getTokenString(forKeyword), token.text);
			pm->targetController = controller;
		}

		tokenizer.getNeededToken(stream, token);
		checkToken(token, leftBrace, "microcontroller name");
	}
	else
	{
		checkToken(token, leftBrace, "’{’ or keyword ’for’", pm->identifier);
	}

	if (!pm->targetController && !curPGP->targetController)
		parseError("Target microcontroller unknown, use keyword ’for’ or command line parameter...");

	// parse rest
	bool portmapEnd = false;
	while (1)
	{
		tokenizer.getNeededToken(stream, token);

		if (token.type == rightBrace && !portmapEnd)
		{
			portmapEnd = true;
		}
		else if (token.type == semicolon && portmapEnd)
		{
			break;
		}
		else if (token.type == vportKeyword)
		{
			// virtual port / pin group
			// Syntax:	vport <id>  {...};

			// <identifier>
			tokenizer.getNeededToken(stream, token);
			if (checkToken(token, identifier, vportKeyword))
			{
				checkIdentifierUnique(pm->properties, token.text);
				checkIdentifierUnique(pm->pinBlocks, token.text);
				checkIdentifierUnique(pm->vports, token.text);
			}

			if (token.type == rightBrace)
			{
				tokenizer.putBackToken(token);
			}
			else if (token.type != semicolon)
			{
				VirtualPort * vp = new VirtualPort;

				vp->parentPortmap = pm;
				vp->identifier = token.text;
				vp->line = tokenizer.getCurLine();

#ifdef DEBUG_PARSE_LEVEL
				std::cout << "-> Entering parse level vport" << std::endl;
#endif
				parseVirtualPort(vp, stream);
#ifdef DEBUG_PARSE_LEVEL
				std::cout << "-> Leaving parse level vport" << std::endl;
#endif

				pm->vports.push_back(vp);
			}
		}
		else if (token.type == pinKeyword || token.type == pinsKeyword)
		{
			// Syntax:	pin <identifier>: <port-character> <number>;
			// Syntax:	pins  <identifier>: <port-character> <number> - <number> ;
			TokenType entryType = token.type;

			// <identifier>
			tokenizer.getNeededToken(stream, token);
			if (checkToken(token, identifier, entryType))
			{
				checkIdentifierUnique(pm->properties, token.text);
				checkIdentifierUnique(pm->pinBlocks, token.text);
				checkIdentifierUnique(pm->vports, token.text);
			}

			if (token.type == rightBrace)
			{
				tokenizer.putBackToken(token);
			}
			else if (token.type != semicolon)
			{
				if (token.type == colon)
					tokenizer.putBackToken(token);

				PinBlock * p = new PinBlock;

				p->parentPortmap = pm;
				p->identifier = token.text;
				p->line = tokenizer.getCurLine();

#ifdef DEBUG_PARSE_LEVEL
				std::cout << "-> Entering parse level pin(s)" << std::endl;
#endif
				parsePinBlock(p, entryType, stream);
#ifdef DEBUG_PARSE_LEVEL
				std::cout << "-> Leaving parse level pin(s)" << std::endl;
#endif

				pm->pinBlocks.push_back(p);
			}
		}
		else if (token.type == propertyKeyword)
		{
			// Syntax:	property  <identifier> = <numberOrBool> ;

			// <identifier>
			tokenizer.getNeededToken(stream, token);
			if (checkToken(token, identifier, propertyKeyword))
			{
				checkIdentifierUnique(pm->properties, token.text);
				checkIdentifierUnique(pm->pinBlocks, token.text);
				checkIdentifierUnique(pm->vports, token.text);
			}

			if (token.type == rightBrace)
			{
				tokenizer.putBackToken(token);
			}
			else if (token.type != semicolon)
			{
				if (token.type == equals)
					tokenizer.putBackToken(token);

				Property * p = new Property;

				p->identifier = token.text;
				p->line = tokenizer.getCurLine();

#ifdef DEBUG_PARSE_LEVEL
	std::cout << "-> Entering parse level property" << std::endl;
#endif
				parseProperty(p, stream);
#ifdef DEBUG_PARSE_LEVEL
	std::cout << "-> Leaving parse level property" << std::endl;
#endif

				pm->properties.push_back(p);
			}
		}
		else
		{
			if (portmapEnd)
			{
				parseErrorCheckToken(getTokenString(semicolon), getTokenString(rightBrace), token.text);
				tokenizer.putBackToken(token);
				break;
			}
			else
			{
				parseError("expected " + getTokenString(vportKeyword) + ", " +
				                         getTokenString(propertyKeyword) + ", " +
				                         getTokenString(pinKeyword) + ", " +
				                         getTokenString(pinsKeyword) + " or " +
				                         getTokenString(rightBrace) +
				                         ", found ’" + token.text + "’", false);
				while(token.type != semicolon)
				{
					tokenizer.getNeededToken(stream, token);
				}
			}
		}
	}

	checkForPinCollisions(pm->pinBlocks);
}


void parse()
{
	// Parse config file

	curPGP->parseErrorNum = 0;
	
	std::ifstream iFile(curPGP->iFilename.c_str(), std::ifstream::in);

	if (!iFile)
		libcError(curPGP->iFilename);

	if (curPGP->verboseLevel)
		std::clog << "Parsing portmap file " << curPGP->iFilename << "..." << std::endl;

	Token token;
	int seqNum = 0;
	while (tokenizer.getToken(iFile, token))
	{
		if (token.type == portmapKeyword)
		{
			// Syntax:	portmap <identifier> [for <controller>] { ... };

			// <identifier>
			tokenizer.getNeededToken(iFile, token);
			if (checkToken(token, identifier, portmapKeyword))
				checkIdentifierUnique(curPGP->portmaps, token.text);
			
			if (token.type == rightBrace)
			{
				tokenizer.putBackToken(token);
			}
			else if (token.type != semicolon)
			{
				Portmap * pm = new Portmap;

				pm->identifier = token.text;
				pm->line = tokenizer.getCurLine();
				pm->sequenceNumber = seqNum;
	
#ifdef DEBUG_PARSE_LEVEL
				std::cout << "-> Entering parse level portmap" << std::endl;
#endif
				parsePortmap(pm, iFile);
#ifdef DEBUG_PARSE_LEVEL
				std::cout << "-> Leaving parse level portmap" << std::endl;
#endif
	
				curPGP->portmaps.push_back(pm);
			}
		}
		else
		{
			CopySection * cs = new CopySection;
			cs->text = token.text;
			cs->sequenceNumber = seqNum;

			curPGP->copySections.push_back(cs);
		}
		
		seqNum++;
	}
}

}	// namespace parse
