/**
 *	\file	parse.h
 *	\brief	Parse portmap definitions
 *	\author	Philipp Werner
 *	\date	20.05.2008
 *
 */


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
	comma = 7,

	number = 8,
	identifier = 9,

	// Keywords
	portmapKeyword = 10,
	pinKeyword = 11,
	pinsKeyword = 12,
	vportKeyword = 13,
	forKeyword = 14,
	propertyKeyword = 15
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
