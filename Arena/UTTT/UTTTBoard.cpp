#include "UTTTBoard.hpp"

using namespace arena;

int UTTTBoard::_s_player1_wins = 0;
int UTTTBoard::_s_player2_wins = 0;
int UTTTBoard::_s_player1_first = 0;
int UTTTBoard::_s_player2_first = 0;
int UTTTBoard::_s_draws = 0;
int UTTTBoard::_s_games = 0;
std::mutex UTTTBoard::_s_vars_mutex;

UTTTBoard::UTTTBoard()
:IBoard(), _finished(), _result(), _last_move()
{
	memset(this->_finished_grids, '\0', sizeof(UTTTBoard::e_result) * 9);
	memset(this->_grid, '\0', sizeof(UTTTBoard::e_result) * 81);
}

UTTTBoard::~UTTTBoard()
{
}

IBoard *		UTTTBoard::clone(void) const {
	return new UTTTBoard();
}

std::ostream	& UTTTBoard::printTotalResults(std::ostream & os) const
{
	this->_s_vars_mutex.lock();
	os << "Player 1 Wins:" << this->_s_player1_wins << "; Player 1 starts:" << this->_s_player1_first << '\n'
		<< "\nPlayer 2 Wins:" << this->_s_player2_wins << "; Player 2 starts:" << this->_s_player2_first << '\n'
		<< "\nDraws:" << this->_s_draws << std::endl
		<< "Games:" << this->_s_games << std::endl;
	this->_s_vars_mutex.unlock();
	return os;
}

void			UTTTBoard::clearTotalResults(void)
{
	this->_s_vars_mutex.lock();
	this->_s_player1_wins = 0;
	this->_s_player2_wins = 0;
	this->_s_player1_first = 0;
	this->_s_player2_first = 0;
	this->_s_draws = 0;
	this->_s_vars_mutex.unlock();
}

void			UTTTBoard::resolveGame(const std::vector<std::string> & players)
{
	if (players.size() != 2)
		throw (IBoard::BadPlayerCount());
	
	UTTTAction current(t_pos(-1, -1));
	UTTTUpdate update;

	int swap;
	const std::string * s1_ptr;
	const std::string * s2_ptr;

	this->_s_vars_mutex.lock();
	swap = this->_s_games & 1;
	++this->_s_games;
	if (swap)
		++this->_s_player1_first;
	else
		++this->_s_player2_first;
	this->_s_vars_mutex.unlock();

	if (swap)
	{
		s1_ptr = &players[1];
		s2_ptr = &players[0];
	}
	else
	{
		s1_ptr = &players[0];
		s2_ptr = &players[1];
	}

	Player player1(*s1_ptr);
	Player player2(*s2_ptr);
	player1.launchPlayer();
	player2.launchPlayer();

	update.setPos(current.getPos());
	player1.sendUpdate(update);
	while(1)
	{
		//receiving action from player 1 and applying it to the board
		player1.recvAction(current);
		if (this->_applyAction(current.getPos(), true) != UTTTBoard::NOTHING)
			break;

		//creating an update for player 2 and sending it
		update.setPos(current.getPos());
		update.setMoves(this->_listLegalMoves());
		player2.sendUpdate(update);


		//receiving action from player 2 and applying it to the board
		player2.recvAction(current);
		if (this->_applyAction(current.getPos(), false) != UTTTBoard::NOTHING)
			break;

		//creating an update for player 1 and sending it
		update.setPos(current.getPos());
		update.setMoves(this->_listLegalMoves());
		player1.sendUpdate(update);
	}

	if (swap)
	{
		if (this->_result == PLAYER1)
			this->_result = PLAYER2;
		else if (this->_result == PLAYER2)
			this->_result = PLAYER1;
	}
	this->_updateResult();
	// this->_s_vars_mutex.lock();
	//update the static values
	// this->_s_vars_mutex.unlock();
}
void			UTTTBoard::clearBoard(void)
{
}

int				UTTTBoard::getPlayerSize(void) const {
	return 2;
}
int	&			UTTTBoard::getGamesPlayed(void) {
	return this->_s_games;
}

void			UTTTBoard::_updateResult(void)
{
	this->_s_vars_mutex.lock();
	
	if (this->_result == PLAYER1)
		this->_s_player1_wins++;
	else if (this->_result == PLAYER2)
		this->_s_player2_wins++;
	else
		this->_s_draws++;
	this->_s_vars_mutex.unlock();
}

UTTTBoard::e_result		UTTTBoard::_applyAction(t_pos move, bool player)
{
	UTTTBoard::e_result ret = NOTHING;
	UTTTBoard::e_result small_board_state = NOTHING;
	
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
			if (this->_grid[row][col] == arena::UTTTBoard::PLAYER1)
				os << 'o';
			else if (this->_grid[row][col] == arena::UTTTBoard::PLAYER2)
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
