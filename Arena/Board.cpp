#include "Board.hpp"

Board::Board()
:_finished(), _result(), _last_move()
{
	memset(this->_finished_grids, '\0', sizeof(Board::e_result) * 9);
	memset(this->_grid, '\0', sizeof(Board::e_result) * 81);
}

Board::~Board()
{

}

Board::e_result Board::_getBoardState(int col, int row, bool last_player,
	t_grid &grid)
{
	e_result player = last_player ? PLAYER1 : PLAYER2;

	//firs test if there is a win in the smallboard

	//up left to down right horizontal
	if (grid[0 + row][0 + col]
		== player
		&& grid[0 + row][0 + col]
		== grid[1 + row][1 + col]
		&& grid[1 + row][1 + col]
		== grid[2 + row][2 + col])
		return player;
	
	//up right to down left horizontal
	if (grid[0 + row][2 + col]
		== player
		&& grid[0 + row][2 + col]
		== grid[1 + row][1 + col]
		&& grid[1 + row][1 + col]
		== grid[2 + row][0 + col])
		return player;

	//three columns
	for(int i = 0; i < 3; i++)
	{
		if (grid[0 + row][i + col]
			== player 
			&& grid[0 + row][i + col]
			== grid[1 + row][i + col]
			&& grid[0 + row][i + col]
			== grid[2 + row][i + col])
			return player;
	}

	//three rows
	for(int i = 0; i < 3; i++)
	{
		if (grid[i + row][0 + col]
			== player 
			&& grid[i + row][0 + col]
			== grid[i + row][1 + col]
			&& grid[i + row][0 + col]
			== grid[i + row][2 + col])
			return player;
	}

	//check if full ( DRAW )
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if (grid[j + row][i + col] == NOTHING)
				return NOTHING;
		}
	}
	return DRAW;
}

Board::e_result Board::applyAction(t_pos move, bool player)
{
	e_result ret = NOTHING;
	e_result small_board_state = NOTHING;
	
	this->_grid[move.row][move.col] = player ? PLAYER1 : PLAYER2;

	small_board_state = this->_getBoardState(move.col / 3, move.row / 3, player, this->_grid);
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

std::vector<t_pos> Board::listActions(void) const
{
}

bool Board::isFinished(void) const	{
	return (this->_finished);
}

Board::e_result Board::getResult(void) const {
	return (this->_result);
}

const Board::e_result[9][9] &Board::getGrid(void) const {
	return (this->_grid);
}

void Board::printBoard(void) const
{
	std::cout << *this;
}

std::ostream & operator<<(std::ostream & os, const Board & ref)
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			os << (ref.getGrid()[row][col] == Board::PLAYER1 ? 'x' : 'o');
			if (col == 2 || col == 5)
				os << '|';
		}
		os << std::endl;
		if (row == 2 || row == 5)
			os << "-----------" << std::endl;
	}

	return os;
}

