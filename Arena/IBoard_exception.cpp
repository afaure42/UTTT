#include "IBoard.hpp"

const char * arena::IBoard::BadPlayerCount::what() const throw()
{
	return "BadPlayerCountException";
}

const char * arena::IBoard::BoardNotEmpty::what() const throw()
{
	return "BoardNotEmptyException";
}
