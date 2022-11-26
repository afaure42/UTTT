#include "UTTTBoard.hpp"

using namespace arena;

UTTTBoard::UTTTBoard()
:_finished(), _result(), _last_move()
{
	memset(this->_finished_grids, '\0', sizeof(Board::e_result) * 9);
	memset(this->_grid, '\0', sizeof(Board::e_result) * 81);
}

IBoard *		UTTTBoard::clone(void) const override {
	return new UTTBoard();
}

std::ostream	UTTTBoard::printTotalResults(std::ostream & os) override const
{
	os "Player 1 Wins:" << this->_player1_wins << "; Player 1 starts:" << player1_first << '\n'
		<< "Player 2 Wins:" << player2_wins << "; Player 2 starts:" << player2_first << '\n'
		<< "Draws:" << draws << std::endl;
}

void			UTTTBoard::clearTotalResults(void) override
{
	this->_s_vars_mutex.lock();
	this->_s_player1_wins = 0;
	this->_s_player2_wins = 0;
	this->_s_draws = 0;
	this->_s_vars_mutex.unlock();
}

void			UTTTBoard::resolveGame(std::vector<std:string> & players) override
{
	if (players.size() != 2)
		throw (IBoard::BadPlayerCount());
	
	UTTTAction current(UTTTBoard::t_pos(-1, -1));
	UTTTUpdate update();

	Player player1(players[0]);
	Player player2(players[1]);

	player1.launchPlayer();
	player2.launchPlayer();

	update.setPos(current.getPos());
	player1.sendUpdate(update);
	while(1)
	{
		//receiving action from player 1 and applying it to the board
		player1.recvAction(current);
		if (this->_applyAction(current, true) != UTTTBoard::NOTHING)
			break;

		//creating an update for player 2 and sending it
		update.setPos(current.getPos);
		update.setMoves(this->_listLegalMoves());
		player2.sendUpdate(update);


		//receiving action from player 2 and applying it to the board
		player2.recvAction(current);
		if (this->_applyAction(current, false) != UTTTBoard::NOTHING)
			break;

		//creating an update for player 1 and sending it
		update.setPos(current.getPos);
		update.setMoves(this->_listLegalMoves());
		player1.sendUpdate(update);
	}

	this->_s_vars_mutex.lock();
	//update the static values
	this->_s_vars_mutex.unlock();
}

void			UTTTBoard::clearBoard(void) override
{
}

int				UTTTBoard::getPlayerSize(void) override const {
	return 2;
}

e_result		UTTTBoard::_applyAction(t_pos move, bool player)
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

std::vector<t_pos>	UTTTBoard::_listLegalMoves(void) const {
	return std::vector<t_pos>();
}

std::ostream &	UTTTBoard::write(std::ostream & os) const
{
	for (int row = 0; row < 9; row++)
	{
		for (int col = 0; col < 9; col++)
		{
			if (ref._grid[row][col] == arena::UTTTBoard::PLAYER1)
				os << 'o';
			else if (ref._grid[row][col] == arena::UTTTBoard::PLAYER2)
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
