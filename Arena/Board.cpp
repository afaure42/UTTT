#include "Board.hpp"

arena::Board::Board()

arena::Board::~Board()
{

}

arena::Board::e_result arena::Board::applyAction(t_pos move, bool player)
{
}

std::vector<arena::t_pos> arena::Board::listActions(void) const
{
	return std::vector<arena::t_pos>();
}

bool arena::Board::isFinished(void) const	{
	return (this->_finished);
}

arena::Board::e_result arena::Board::getResult(void) const {
	return (this->_result);
}

const arena::Board::e_result (&arena::Board::getGrid(void) const)[9][9] {
	return (this->_grid);
}

const arena::Board::e_result (&arena::Board::getFinished(void) const)[3][3] {
	return (this->_finished_grids);
}

void arena::Board::printBoard(void) const
{
	std::cout << *this;
}

void arena::Board::printSmallBoard(void) const
{
	for(int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			if (this->_finished_grids[row][col] == arena::Board::PLAYER1)
				std::cout << 'o';
			else if (this->_finished_grids[row][col] == arena::Board::PLAYER2)
				std::cout << 'x';
			else if (this->_finished_grids[row][col] == arena::Board::DRAW)
				std::cout << 'D';
			else
				std::cout << ' ';
		}
		std::cout << '\n';
	}
}

std::ostream & operator<<(std::ostream & os, const arena::Board & ref)
{
}

