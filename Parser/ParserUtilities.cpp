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

void ParserUtilities::IncrementBufferIndex()
{
	int _currentIndex = ++_tokenBufferCounter;

	if (_tokenBuffer.size() > _currentIndex)
	{
		*_tokenBufferCounterPtr += 1;
		std::string _inputToken = _tokenBuffer[_tokenBufferCounter];
	}
	else
	{
		std::cout << "\n\nIncremented counter exceeds the bounds of the buffer\n\n";
		_ASSERT(false);
	}
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
		_ASSERT(false);
	}

	return std::string();
}

std::string ParserUtilities::Mult_op()
{
	IncrementBufferIndex();
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

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

std::string ParserUtilities::Add_op()
{
	IncrementBufferIndex();
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

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

std::string ParserUtilities::Factor()
{
	IncrementBufferIndex();
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	const std::regex _numberRegex("^\d");

	if (_inputToken != "read" 
		&& _inputToken != "write" 
		&& _inputToken != "$$" 
		&& _inputToken != "+" 
		&& _inputToken != "-"
		&& _inputToken != "/"
		&& _inputToken != "*")
	{
		if (std::regex_match(_inputToken, _numberRegex))
		{
			std::cout << "Token: " << _inputToken << " consumed" << "\n";
		}
		else if(_inputToken != "(")
		{
			std::cout << "Token: " << _inputToken << " consumed" << "\n";
		}
	}
	else if(_inputToken == "(")
	{
		Match("(", _inputToken);

		Expr();

		IncrementBufferIndex();

		Match(")", _inputToken);
	}
	else
	{
		std::cout << "\n\nPARSE ERROR AT LINE " << _tokenBufferCounter << "\n\n";
		_ASSERT(false);
	}
	return std::string();
}

std::string ParserUtilities::Factor_tail()
{
	return std::string();
}

std::string ParserUtilities::Term()
{
	return std::string();
}

std::string ParserUtilities::Term_tail()
{
	return std::string();
}

std::string ParserUtilities::Expr()
{
	return std::string();
}

std::string ParserUtilities::Stmt()
{
	return std::string();
}

std::string ParserUtilities::Stmt_list()
{
	return std::string();
}
