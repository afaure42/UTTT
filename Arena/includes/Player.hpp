#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "common.hpp"
#include "t_pos.hpp"

namespace arena
{
template<typename Action_t>
class Player
{
public:
	Player(std::string path_to_exec);
	~Player();

	Action_t	recvPos();

	void		sendPos(const Action_t & pos);

	void		launchPlayer(char *name);
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
