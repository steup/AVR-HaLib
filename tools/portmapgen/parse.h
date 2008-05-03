
#pragma once

#include <string>


namespace parse
{

enum TokenType
{
	unknown = 0,
	leftBrace = 1,
	rightBrace = 2,
	colon = 3,
	semicolon = 4,
	minus = 5,
	equals = 6,

	number = 7,
	identifier = 8,

	// Keywords
	portmapKeyword = 9,
	pinKeyword = 10,
	pinsKeyword = 11,
	vportKeyword = 12,
	forKeyword = 13,
	propertyKeyword = 14
};

struct Token
{
	TokenType type;
	std::string text;
};


class Tokenizer
{
	int  currentLineNumber;
	bool insidePortmapBlock;
	int  braceDepth;

	Token * putBack;

public:
	Tokenizer() : 
		currentLineNumber(1),
		insidePortmapBlock(false),
		braceDepth(0),
		putBack(0)
	{
	}

	int getCurLine()
	{
		return currentLineNumber;
	}

	// Get a token from stream
	std::istream & getToken(std::istream & stream, Token & token);

	// Get token or produce an error on EOF
	void getNeededToken(std::istream & stream, Token & token);

	// Put back one Token, will be returned first when getting a token next time
	void putBackToken(Token t);
};

extern Tokenizer tokenizer;


void parseError(std::string s, bool fatal = true, int line = 0);

void parse();

}	//namespace parse
