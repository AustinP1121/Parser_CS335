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


class ParserUtilities
{
public:
	ParserUtilities();
	~ParserUtilities();

	//ingests the token from the data file
	std::vector <std::string> ReadTokens(std::vector <std::string>);
	
	std::string Match(std::string, std::string);

	std::string Mult_op(std::string);
	std::string Add_op(std::string);
	std::string Factor(std::string);
	std::string Factor_tail(std::string);
	std::string Term(std::string);
	std::string Term_tail(std::string);
	std::string Expr(std::string);
	std::string Stmt(std::string);
	std::string Stmt_list(std::string);

private:
	std::vector <std::string> _tokenBuffer;
	
};
