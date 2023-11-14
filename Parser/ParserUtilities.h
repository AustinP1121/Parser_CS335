/*
* Project: Assignment #4, Parser
* Author: Austin Pedigo
* Class: CS-335
* 
* Description: 
*	Simple bootstrap parser written in C++. This class enables the 
*	parser functionality, which is based on a given LL grammar. 
*/

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
	//constructor 
	ParserUtilities();
	//destructor
	~ParserUtilities();

	//error handler
	void InvokeParseError();

	//ingests the token from the data file
	std::vector <std::string> ReadTokens();
	//initializes output file
	void OpenWriteFile();
	
	//increments the global buffer index
	void IncrementBufferIndex();

	//matches the first argument with the second
	std::string Match(std::string, std::string);
	
	//checks if the input string is an id
	bool IsID(std::string);

	//checks if the input string is a number
	bool IsNumber(std::string);

	//handles the mult_op 
	std::string Mult_op();
	//handles the add_op
	std::string Add_op();
	//handles the factor
	std::string Factor();
	//handles the factor_Tail
	std::string Factor_tail();
	//handles the term
	std::string Term();
	//handles the term_tail
	std::string Term_tail();
	//handles the expr
	std::string Expr();
	//handles the stmt
	std::string Stmt();
	//handles the stmt_list
	std::string Stmt_list();
	
	//point of start and return
	void Program();

protected: 
	int _tokenBufferCounter = 0;
	int* _tokenBufferCounterPtr = &_tokenBufferCounter;

	std::ofstream outFile;

	std::vector <std::string> _tokenBuffer;
	
private:
	
};
