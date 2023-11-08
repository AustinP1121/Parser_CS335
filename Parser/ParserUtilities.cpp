#include "ParserUtilities.h"

ParserUtilities::ParserUtilities()
{
	ReadTokens(_tokenBuffer);
}

ParserUtilities::~ParserUtilities()
{
}

std::vector<std::string> ParserUtilities::ReadTokens(std::vector <std::string> _buffer)
{
	//open the file containing the tokens
	std::ifstream _tokenFile("../TokenFile.txt");
	//variable to store the line data
	std::string line;


	//ensures that an attempt is not made to open a closed file
	if (_tokenFile.is_open())
	{
		//counter
		int i = 0;
		
		//populate the buffer while line data is available
		while(std::getline(_tokenFile, line))
		{
			//push line data to the buffer
			_buffer.push_back(line);
			i++;
		}
		
		//cleanup
		_tokenFile.close();
	}
	
	return _buffer;
}

std::string ParserUtilities::Match(std::string _expectedToken, std::string _inputToken)
{
	if (_inputToken == _expectedToken)
	{
		std::cout << "Token: " << _inputToken << " consumed" << "\n";
	}
	else
	{
		std::cout << "PARSE ERROR\n";
	}

	return std::string();
}

std::string ParserUtilities::Mult_op(std::string _inputToken)
{
	if (_inputToken == "*")
	{
		Match("*", _inputToken);
	}

	else if (_inputToken == "/")
	{
		Match("/", _inputToken);
	}

	return _inputToken;
}

std::string ParserUtilities::Add_op(std::string _inputToken)
{
	if (_inputToken == "+")
	{
		Match("+", _inputToken);
	}

	else if (_inputToken == "-")
	{
		Match("-", _inputToken);
	}

	return _inputToken;
}

std::string ParserUtilities::Factor(std::string)
{
	return std::string();
}

std::string ParserUtilities::Factor_tail(std::string)
{
	return std::string();
}

std::string ParserUtilities::Term(std::string)
{
	return std::string();
}

std::string ParserUtilities::Term_tail(std::string)
{
	return std::string();
}

std::string ParserUtilities::Expr(std::string)
{
	return std::string();
}

std::string ParserUtilities::Stmt(std::string)
{
	return std::string();
}

std::string ParserUtilities::Stmt_list(std::string)
{
	return std::string();
}
