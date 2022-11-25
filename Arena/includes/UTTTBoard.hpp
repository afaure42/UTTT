#ifndef UTTTBOARD_HPP
# define UTTTBOARD_HPP

#include "IBoard.hpp"

namespace arena
{

class UTTTBoard : public IBoard
{
public:
	enum e_result
	{
		NOTHING,
		DRAW,
		PLAYER1,
		PLAYER2
	};

	UTTBBoard *	clone(void) override const;
	void		printTotalResults(void) override const;
	void		resolveGame(Player *) override;
	void		getPlayerSize(void) override const;
	void		print(void) override const;

private:
	static int _s_player1_wins;
	static int _s_player2_wins;
	static int _s_draws;

	/**
	 * @brief function to apply a move to the board ( MUST BE VALID )
	 * 
	 * @return result of the action
	 * @param move the move of the last player
	 * @param player TRUE for player1, FALSE for player2
	 */
	e_result			_applyAction(t_pos move, bool player);

	/**
	 * @brief function to get the possible actions after a move has been applied
	 * 
	 * @return a std::vector<t_pos> containing all the possible positions
	 */
	std::vector<t_pos>	_listActions(void) const;



	template <typename T, size_t N, size_t M>
	e_result _getBoardState(int row, int col, bool last_player,
			T (&grid)[N][M]);

	e_result	_result;
	bool		_finished;

	e_result	_finished_grids[3][3];
	t_pos		_last_move;
	e_result	_grid[9][9];	
};

}

#include "UTTTBoard.templates.hpp"

#endif
