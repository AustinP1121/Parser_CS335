/*
* Project: Assignment #4, Parser
* Author: Austin Pedigo
* Class: CS-335
*
* Description:
*	Simple bootstrap parser written in C++. This class enables the
*	parser functionality, which is based on a given LL grammar.
*/

#include "ParserUtilities.h"

ParserUtilities::ParserUtilities()
{
	//read in buffer
	ReadTokens();

	//open the output file
	OpenWriteFile();

	//start the program
	Program();

	//close the output file
	outFile.close();
}

ParserUtilities::~ParserUtilities()
{
}

void ParserUtilities::InvokeParseError()
{
	//displays the error message and halts the program
	std::string _errorMessage = "\n\nPARSE ERROR AT LINE\n\n";

	outFile << _errorMessage << ++_tokenBufferCounter;

	std::cout << _errorMessage << ++_tokenBufferCounter;
	
	_ASSERT(false);
}

std::vector<std::string> ParserUtilities::ReadTokens()
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
			_tokenBuffer.push_back(line);
			i++;
		}
		
		//cleanup
		_tokenFile.close();
	}
	
	return _tokenBuffer;
}

void ParserUtilities::OpenWriteFile()
{
	outFile.open("../OutputFile.txt");
}

void ParserUtilities::IncrementBufferIndex()
{
	int _currentIndex = _tokenBufferCounter;
	
	//if the next index is within bounds, continue
	if (_tokenBuffer.size() > ++_currentIndex)
	{
		*_tokenBufferCounterPtr += 1;
	}
	//halt program otherwise
	else
	{
		std::cout << "\n\nIncremented counter exceeds the bounds of the buffer\n\n";
		_ASSERT(false);
	}
}

std::string ParserUtilities::Match(std::string _expectedToken, std::string _inputToken)
{
	//consume token if it matches the expected one
	if (_inputToken == _expectedToken)
	{
		outFile << "Token: " << _inputToken << " consumed\n";
		std::cout << "Token: " << _inputToken << " consumed" << "\n";
	}
	else
	{
		InvokeParseError();
	}

	return std::string();
}

bool ParserUtilities::IsID(std::string _inputToken)
{
	//regex pattern for id tokens (variable names)
	const std::regex _idPattern("[a-zA-Z]+");

	//if the input token matches the id pattern, return true
	if (std::regex_match(_inputToken, _idPattern))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

bool ParserUtilities::IsNumber(std::string _inputToken)
{
	//regex pattern for number tokens including numbers like 1, 2, 1.0, 45.4443, so on
	std::regex _numberPattern("[0-9]+[.]?[0-9]*");
	
	//if the input token matches the number pattern, return true
	if (std::regex_match(_inputToken, _numberPattern))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

std::string ParserUtilities::Mult_op()
{
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	//if it's a multiplication symbol, consume
	if (_inputToken == "*")
	{
		Match("*", _inputToken);
		IncrementBufferIndex();
	}

	//if it's a divisor, consume
	else if (_inputToken == "/")
	{
		Match("/", _inputToken);
		IncrementBufferIndex();
	}

	//otherwise, parse error
	else
	{
		InvokeParseError();
	}

	return _inputToken;
}

std::string ParserUtilities::Add_op()
{
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	//if it's an addition symbol, consume
	if (_inputToken == "+")
	{
		Match("+", _inputToken);
		IncrementBufferIndex();
	}

	//if it's a subtraction symbol, consume
	else if (_inputToken == "-")
	{
		Match("-", _inputToken);
		IncrementBufferIndex();
	}

	//otherwise, parse error
	else
	{
		InvokeParseError();
	}

	return _inputToken;
}

std::string ParserUtilities::Factor()
{
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	if (IsNumber(_inputToken))
	{
		Match(_inputToken, _inputToken);

		IncrementBufferIndex();
	}
	
	else if(IsID(_inputToken))
	{
		Match(_inputToken, _inputToken);

		IncrementBufferIndex();
	}

	else if(_inputToken == "(")
	{
		std::cout << "\nExecuting Expr() ";
		outFile << "\nExecuting Expr() ";
		
		Match("(", _inputToken);
		
		IncrementBufferIndex();
		_inputToken = _tokenBuffer[_tokenBufferCounter];
		
		Expr();

		_inputToken = _tokenBuffer[_tokenBufferCounter];

		Match(")", _inputToken);
		
		IncrementBufferIndex();
		
	}
	
	else
	{
		InvokeParseError();
	}
	
	return _inputToken;
}

std::string ParserUtilities::Factor_tail()
{
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	if (_inputToken == "*" 
		|| _inputToken == "/")
	{
		outFile << "\nExecuting Mult_op()		Factor()	Factor_tail()\n";
		std::cout << "\nExecuting Mult_op()		Factor()	Factor_tail()\n";
		
		Mult_op();

		Factor();

		Factor_tail();
	}
	//do this if the token is not +, -, ), an id, read, write, or $$
	else if (_inputToken != "+"
		&& _inputToken != "-"
		&& _inputToken != ")"
		&& _inputToken != "read"
		&& _inputToken != "write"
		&& IsID(_inputToken) != true
		&& _inputToken != "$$")
	{
		InvokeParseError();
	}
	//continue
	
	return _inputToken;
}

std::string ParserUtilities::Term()
{
	//<term> -> <factor> <factor_tail>
	
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	//continue if term conditions are met
	if (IsID(_inputToken) == true 
		|| IsNumber(_inputToken) == true 
		|| _inputToken == "(")
	{
		outFile << "\nExecuting Factor()	Factor_tail()\n";
		std::cout << "\nExecuting Factor()	Factor_tail()\n";

		Factor();
		
		Factor_tail();
	}
	else
	{
		InvokeParseError();
	}

	
	return _inputToken;
}

std::string ParserUtilities::Term_tail()
{
	//<term_tail> -> <add_op> <term> <term_tail> | epsilon

	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	//continue if term_tail conditions are met
	if (_inputToken == "+" 
		|| _inputToken == "-")
	{
		outFile << "\nExecuting Add_op()	Term()	Term_tail()\n";
		std::cout << "\nExecuting Add_op()	Term()	Term_tail()\n";
		
		Add_op();

		Term();

		Term_tail();
	}
	
	else if (_inputToken != ")"
		&& _inputToken != "read"
		&& _inputToken != "write"
		&& IsID(_inputToken) != true
		&& _inputToken != "$$")
	{
		InvokeParseError();
	}
	else
	{
		//epsilon production
	}
	
	return _inputToken;
}

std::string ParserUtilities::Expr()
{
	//<expr> -> <term> <term_tail>
	
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];
	
	//continue if the token is an id, number, or open parentheses
	if(IsID(_inputToken) == true 
		|| IsNumber(_inputToken) == true 
		|| _inputToken == "(") 
	{
		outFile << "\nExecuting Term()	Term_tail()\n";
		std::cout << "\nExecuting Term()	Term_tail()\n";
		
		Term();
		
		Term_tail();
	}

	else
	{
		InvokeParseError();
	}
	
	return _inputToken;
}

std::string ParserUtilities::Stmt()
{
	//<stmt> -> <id> = <expr> | read <id> | write <expr>

	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	//continue if the token is an id and it's not read or write
	if (IsID(_inputToken) == true && _inputToken != "read" && _inputToken != "write")
	{
		outFile << "\nExecuting Expr()\n";
		std::cout << "\nExecuting Expr()\n";
		Match(_inputToken, _inputToken);

		IncrementBufferIndex();

		_inputToken = _tokenBuffer[_tokenBufferCounter];
		
		Match(":=", _inputToken);

		IncrementBufferIndex();

		_inputToken = _tokenBuffer[_tokenBufferCounter];
		
		Expr();
	}

	//if the token is read
	else if (_inputToken == "read")
	{
		Match("read", _inputToken);

		IncrementBufferIndex();
		
		_inputToken = _tokenBuffer[_tokenBufferCounter];

		Match(_inputToken, _inputToken);

		IncrementBufferIndex();

		_inputToken = _tokenBuffer[_tokenBufferCounter];
	}

	//if the token is write
	else if (_inputToken == "write")
	{
		Match("write", _inputToken);
		
		outFile << "\nExecuting Expr()\n";
		std::cout << "\nExecuting Expr()\n";

		IncrementBufferIndex();

		_inputToken = _tokenBuffer[_tokenBufferCounter];
		
		Expr();
	}

	else
	{
		InvokeParseError();
	}
	
	return _inputToken;
}

std::string ParserUtilities::Stmt_list()
{
	//<stmt_list> -> <stmt> <stmt_list> | epsilon
		
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	//continue parse tree if it's an id or read/write
	if (IsID(_inputToken) == true
		|| _inputToken == "read"
		|| _inputToken == "write")
	{
		outFile << "\nExecuting Stmt()	Stmt_list()\n";
		std::cout << "\nExecuting Stmt()	Stmt_list()\n";
		
		Stmt();

		Stmt_list();
	}

	else if (_inputToken != "$$")
	{
		InvokeParseError();
	}
	
	return _inputToken;
}

void ParserUtilities::Program()
{
	//<program> -> <stmt_list> $$
	
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];
	
	if (IsID(_inputToken) == true
		|| _inputToken == "read"
		|| _inputToken == "write"
		|| _inputToken == "$$")
	{
		outFile << "\n Executing Program() Stmt_list() \n";
		std::cout << "\n Executing Program() Stmt_list() \n";
		
		//start parse tree
		Stmt_list();
		
		_inputToken = _tokenBuffer[_tokenBufferCounter];
		
		//end of parse tree
		Match("$$", _inputToken);
		
	}
	else
	{
		InvokeParseError();
	}	
}
