#include "Board.hpp"

arena::Board::Board()
:_finished(), _result(), _last_move()
{
	memset(this->_finished_grids, '\0', sizeof(Board::e_result) * 9);
	memset(this->_grid, '\0', sizeof(Board::e_result) * 81);
}

arena::Board::~Board()
{

}

arena::Board::e_result arena::Board::applyAction(t_pos move, bool player)
{
	e_result ret = NOTHING;
	e_result small_board_state = NOTHING;
	
	this->_grid[move.row][move.col] = player ? PLAYER1 : PLAYER2;

	small_board_state = this->_getBoardState(move.row - (move.row % 3), move.col - (move.col % 3), player, this->_grid);
	this->_finished_grids[move.row / 3][move.col / 3] = small_board_state;	

	if (small_board_state != NOTHING)
	{
		ret = this->_getBoardState(0, 0, player, this->_finished_grids);
		if (ret != NOTHING)
		{
			this->_result = ret;
			this->_finished = true;
		}
	}

	return ret;
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
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (ref.getGrid()[row][col] == arena::Board::PLAYER1)
				os << 'o';
			else if (ref.getGrid()[row][col] == arena::Board::PLAYER2)
				os << 'x';
			else
				os << ' ';
			if (col == 2 || col == 5)
				os << '|';
		}
		os << std::endl;
		if (row == 2 || row == 5)
			os << "-----------" << std::endl;
	}

	return os;
}

