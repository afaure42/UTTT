#ifndef BOARD_HPP
#define BOARD_HPP

#include "common.hpp"
#include "t_pos.hpp"

class t_pos;
namespace arena
{
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
	const Board::e_result (&getFinished(void) const)[3][3];


	bool isFinished(void) const;

	/*			UTILS			*/
	void printBoard(void) const;
	void printSmallBoard(void) const;

private:
	Board(const Board & other);

	template <typename T, size_t N, size_t M>
	e_result _getBoardState(int row, int col, bool last_player,
			T (&grid)[N][M])
	{

		e_result player = last_player ? PLAYER1 : PLAYER2;

		//firs test if there is a win in the smallboard

		//up left to down right horizontal
		if (grid[0 + row][0 + col] == player
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

	e_result	_result;
	bool		_finished;

	e_result	_finished_grids[3][3];
	t_pos		_last_move;
	e_result	_grid[9][9];	

};
}

std::ostream & operator<<(std::ostream & os, const arena::Board & ref);


#endif
