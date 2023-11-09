#pragma once

#ifndef STRING_H
#include <string>
#endif // !STRING_H

#ifndef IOSTREAM_H
#include <iostream>
#endif // !IOSTREAM_H

#ifndef VECTOR_H
#include <vector>
#endif // !VECTOR_H

#ifndef FSTREAM_H
#include <fstream>
#endif // !FSTREAM_H

#ifndef REGEX_H
#include <regex>
#endif // !REGEX_H



class ParserUtilities
{
public:
	int _tokenBufferCounter = 0;
	int* _tokenBufferCounterPtr = &_tokenBufferCounter;

	std::vector <std::string> _tokenBuffer;

	ParserUtilities();
	~ParserUtilities();

	//ingests the token from the data file
	std::vector <std::string> ReadTokens(std::vector <std::string>);
	void IncrementBufferIndex();

	std::string Match(std::string, std::string);

	std::string Mult_op();
	std::string Add_op();
	std::string Factor();
	std::string Factor_tail();
	std::string Term();
	std::string Term_tail();
	std::string Expr();
	std::string Stmt();
	std::string Stmt_list();

private:
	
};
