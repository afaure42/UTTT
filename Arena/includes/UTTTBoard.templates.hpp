#ifndef UTTTBOARD_TEMPLATES
#define UTTTBOARD_TEMPLATES

#include "UTTTBoard.hpp"

template <typename T, size_t N, size_t M>
arena::UTTTBoard::e_result arena::UTTTBoard::_getBoardState(
	int row, int col, bool last_player,	T (&grid)[N][M])
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

#endif
