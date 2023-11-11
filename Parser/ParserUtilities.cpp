#include "ParserUtilities.h"

//grammar: 
//<program> -> <stmt_list> $$
//<stmt_list> -> <stmt> <stmt_list> | epsilon
//<stmt> -> id := <expr> | read id | write <expr>
//<expr> -> <term> <term_tail>
//<term_tail> -> <add_op> <term> <term_tail> | epsilon
//<term> -> <factor> <factor_tail>
//<factor_tail> -> <mult_op> <factor> <factor_tail> | epsilon
//<factor> -> ( <expr> ) | id | number
//<add_op> -> + | -
//<mult_op> -> * | /


ParserUtilities::ParserUtilities()
{
	ReadTokens();

	Program();
}

ParserUtilities::~ParserUtilities()
{
}

void ParserUtilities::InvokeParseError()
{
	std::cout << "\n\nPARSE ERROR AT LINE " << _tokenBufferCounter << "\n\n";
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

void ParserUtilities::IncrementBufferIndex()
{
	int _currentIndex = _tokenBufferCounter;

	if (_tokenBuffer.size() > ++_currentIndex)
	{
		*_tokenBufferCounterPtr += 1;
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

	if (_inputToken == "*")
	{
		Match("*", _inputToken);
		IncrementBufferIndex();
	}

	else if (_inputToken == "/")
	{
		Match("/", _inputToken);
		IncrementBufferIndex();
	}

	else
	{
		InvokeParseError();
	}
	return _inputToken;
}

std::string ParserUtilities::Add_op()
{
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	if (_inputToken == "+")
	{
		Match("+", _inputToken);
		IncrementBufferIndex();
	}

	else if (_inputToken == "-")
	{
		Match("-", _inputToken);
		IncrementBufferIndex();
	}

	else
	{
		std::cout << "\n\nPARSE ERROR AT LINE " << _tokenBufferCounter << "\n\n";
		_ASSERT(false);
	}

	return _inputToken;
}

std::string ParserUtilities::Factor()
{
	std::string _inputToken = _tokenBuffer[_tokenBufferCounter];

	if (IsNumber(_inputToken))
	{
		std::cout << "Token: " << _inputToken << " consumed" << "\n";
		IncrementBufferIndex();
	}
	
	else if(IsID(_inputToken))
	{
		std::cout << "Token: " << _inputToken << " consumed" << "\n";
		IncrementBufferIndex();
	}

	else if(_inputToken == "(")
	{
		std::cout << "\nExecuting Expr() ";

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
		std::cout << "\nExecuting Mult_op()	Factor()	Factor_tail()\n";
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


	if (IsID(_inputToken) == true 
		|| IsNumber(_inputToken) == true 
		|| _inputToken == "(")
	{
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

	if (_inputToken == "+" 
		|| _inputToken == "-")
	{
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
	
	if(IsID(_inputToken) == true 
		|| IsNumber(_inputToken) == true 
		|| _inputToken == "(") 
	{
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

	if (IsID(_inputToken) == true && _inputToken != "read" && _inputToken != "write")
	{
		std::cout << "\nExecuting Match()	Expr()\n";
		Match(_inputToken, _inputToken);

		IncrementBufferIndex();

		_inputToken = _tokenBuffer[_tokenBufferCounter];
		
		Match(":=", _inputToken);

		IncrementBufferIndex();

		_inputToken = _tokenBuffer[_tokenBufferCounter];
		
		Expr();
	}

	else if (_inputToken == "read")
	{
		std::cout << "\nExecuting Match()	Match()\n";
		Match("read", _inputToken);

		IncrementBufferIndex();
		
		_inputToken = _tokenBuffer[_tokenBufferCounter];

		Match(_inputToken, _inputToken);

		IncrementBufferIndex();

		_inputToken = _tokenBuffer[_tokenBufferCounter];
	}

	else if (_inputToken == "write")
	{
		std::cout << "\nExecuting Match()	Expr()\n";
		Match("write", _inputToken);

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

	if (IsID(_inputToken) == true
		|| _inputToken == "read"
		|| _inputToken == "write")
	{
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
		std::cout << "\nExecuting Stmt_list()	Match()\n";

		Stmt_list();
		
		_inputToken = _tokenBuffer[_tokenBufferCounter];
		
		Match("$$", _inputToken);
		
	}
	else
	{
		InvokeParseError();
	}	
}
