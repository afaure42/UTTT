#ifndef BOARD_HPP
#define BOARD_HPP

#include "common.hpp"

struct t_pos;

class Board
{
public:
	Board();
	~Board();

	enum e_result
	{
		NOTHING,
		DRAW,
		PLAYER1,
		PLAYER2
	};

	/**
	 * @brief function to apply an move to the board ( MUST BE VALID )
	 * 
	 * @return result of the action
	 * @param move the move of the last player
	 * @param player TRUE for player1, FALSE for player2
	 */
	e_result applyAction(t_pos move, bool player);

	/**
	 * @brief function to get the possible actions after a move has been applied
	 * 
	 * @return a std::vector<t_pos> containing all the possible positions
	 */
	std::vector<t_pos>	listActions(void) const;
	
	e_result getResult(void) const;
	const Board::e_result (&getGrid(void) const)[9][9];


	bool isFinished(void) const;

	/*			UTILS			*/
	void printBoard(void) const;

private:
	Board(const Board & other);

	e_result _getBoardState(int col, int row, bool last_player,
			t_grid &grid);
	e_result	_result;
	bool		_finished;

	e_result	_finished_grids[9][9];
	t_pos		_last_move;
	e_result	_grid[9][9];	

};

std::ostream & operator<<(std::ostream & os, const Board & ref);

#endif
