#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "common.hpp"
// #include "t_pos.hpp"
#include "IAction.hpp"
#include "IUpdate.hpp"

namespace arena
{
class Player
{
public:
	Player(std::string path_to_exec);
	Player(const Player & ref);
	~Player(void);

	/**
	 * @brief this will call the function set from the
	 * given action with the fd corresponding
	 * to this player's output
	 * 
	 * @param action a reference an action class
	 */
	void		recvAction(IAction & action);

	/**
	 * @brief this will call the function str from
	 * the given update and write the output to
	 * this player's input
	 * 
	 * @param update a const reference to an update class
	 */
	void		sendUpdate(const IUpdate & update);

	void		launchPlayer();
	void		stopPlayer(void);

	class playerNotLaunchedException : public std::exception
	{
		const char * what () const throw ();
	};
private:
	void	_waitProcess(void);
	void	_killProcess(void);
	Player();
	int _out_fd;
	int _in_fd;

	std::string _path;
	pid_t 		_pid;
	int 		_pipe_in[2];
	int			_pipe_out[2];
};
}

#endif
