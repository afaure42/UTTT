#ifndef APLAYER_HPP
#define APLAYER_HPP

#include <exception>
#include "IAction.hpp"

namespace arena
{
class APlayer
{
public:
	APlayer(std::string path_to_exec);
	virtual ~APlayer();

	virtual IAction	recvAction() = 0;

	virtual void	sendAction(const int & action) = 0;

	void			launchPlayer(void);
	void			stopPlayer(void);

	class playerNotLaunchedException : public std::exception
	{
		const char * what () throw();
	}
private:
	void	_waitProcess(void);
	void	_killProcess(void);
	APlayer();
	APlayer(const APlayer & other);

	int _out_fd;
	int _in_fd;

	const std::string	_path;
	pid_t 				_pid;
	int 				_pipe_in[2];
	int					_pipe_out[2];
};

}
#endif
