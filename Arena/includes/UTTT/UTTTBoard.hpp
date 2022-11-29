#ifndef UTTTBOARD_HPP
# define UTTTBOARD_HPP

#include <mutex>
#include "UTTT.hpp"
#include "customExceptions.hpp"

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

	UTTTBoard();
	~UTTTBoard();

	arena::IBoard *		clone(void) const override;
	std::ostream &		printTotalResults(std::ostream & os) const override;
	void				clearTotalResults(void) override;
	int	&				getGamesPlayed(void) override;

	void				resolveGame(const std::vector<std::string> & players) override;
	void				clearBoard(void) override;

	int					getPlayerSize(void) const override;
	std::ostream &		write(std::ostream & os) const override;
private:
	static int _s_player1_wins;
	static int _s_player2_wins;
	static int _s_player1_first;
	static int _s_player2_first;
	static int _s_draws;
	static int _s_games;
	static std::mutex _s_vars_mutex;

	/**
	 * @brief function to apply a move to the board ( MUST BE VALID )
	 * 
	 * @return result of the action
	 * @param move the move of the last player
	 * @param player TRUE for player1, FALSE for player2
	 */
	e_result			_applyAction(t_pos move, bool player);

	/**
	 * @brief function to get the possible moves
	 * 
	 * @return a std::vector<t_pos> containing all the possible positions
	 */
	std::vector<t_pos>	_listLegalMoves(void) const;

	void				_updateResult(void);

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
