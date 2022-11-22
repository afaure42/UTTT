#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "common.hpp"

struct t_pos;

class Player
{
public:
	Player(std::string path_to_exec);
	~Player();

	t_pos	recvPos();
	void	sendPos(t_pos);
	void	launchPlayer(void);
	void	waitProcess(void);
	void	killProcess(void);

	class playerNotLaunchedException : public std::exception
	{
		const char * what () const throw ();
	};
private:
	Player();
	int _out_fd;
	int _in_fd;

	std::string _path;
	pid_t 		_pid;
	int 		_pipe_in[2];
	int			_pipe_out[2];
};

#endif
