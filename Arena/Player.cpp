#include "Player.hpp"

/*			EXCEPTIONS			*/
const char * Player::playerNotLaunchedException::what() const throw()
{
	return ("Player not launched before I/O operations");
}

Player::Player(std::string path_to_exec)
:_path(path_to_exec), _pid(-1), _in_fd(-1),
	_out_fd(-1), _pipe_in{-1, -1}, _pipe_out{-1, -1}
{
	if (pipe(this->_pipe_in) < 0)
		throw (arena::syscall_error(errno, "Player::Player: pipe:"));
	
	if (pipe(this->_pipe_out) < 0)
	{
		close_pipe(this->_pipe_in);
		throw (arena::syscall_error(errno, "Player::Player: pipe:"));
	}

	//set close on exec for both pipes
	if (fcntl(this->_pipe_in[0], F_SETFL, FD_CLOEXEC) < 0
		||fcntl(this->_pipe_in[1], F_SETFL, FD_CLOEXEC) < 0
		||fcntl(this->_pipe_out[0], F_SETFL, FD_CLOEXEC) < 0
		||fcntl(this->_pipe_out[1], F_SETFL, FD_CLOEXEC) < 0)
	{
		close_pipe(this->_pipe_in);
		close_pipe(this->_pipe_out);
		throw (arena::syscall_error(errno, "Player::Player: fcntl:"));
	}	
}

Player::~Player()
{
	close_pipe(this->_pipe_in);
	close_pipe(this->_pipe_out);

	if (this->_in_fd != -1)
		close(this->_in_fd);
	if (this->_in_fd != -1)
		close(this->_out_fd);
	if (this->_pid != -1)
	{
		this->killProcess();
		this->waitProcess();
	}
}

void Player::launchPlayer(void)
{
	this->_in_fd = dup(this->_pipe_in[READ_END]);
	if (_in_fd < 0)
		throw (arena::syscall_error(errno, "Player::launchPlayer: dup:"));

	this->_out_fd = dup(this->_pipe_out[WRITE_END]);
	if (this->_out_fd < 0)
		throw (arena::syscall_error(errno, "Player::launchPlayer: dup:"));

	if (fcntl(this->_in_fd, F_SETFL, FD_CLOEXEC) < 0
		||fcntl(this->_out_fd, F_SETFL, FD_CLOEXEC) < 0)
		throw (arena::syscall_error(errno, "Player::launchPlayer: fcntl:"));
	

	//i need to fork and setup my resources for a call to execvp
	this->_pid = fork();

	if (this->_pid < 0)
		throw (arena::syscall_error(errno, "Player::launchPlayer: fork:"));
	
	//in child
	if (this->_pid == 0)
	{
		//duping fds
		if (dup2(this->_pipe_out[READ_END], STDIN_FILENO) < 0)
			throw(arena::syscall_error(errno, "CHILD: Player::launchPlayer: dup2:"));
		
		if (dup2(this->_pipe_in[WRITE_END], STDOUT_FILENO) < 0)
			throw(arena::syscall_error(errno, "CHILD: Player::launchPlayer: dup2:"));
		
		char *args = NULL;

		if (execvp(this->_path.c_str(), &args) < 0)
			throw(arena::syscall_error(errno, "CHILD: Player::launchPlayer: execvp:"));
	}
}

void Player::killProcess(void)
{
	kill(this->_pid, SIGKILL);
}

void Player::waitProcess(void)
{
	waitpid(this->_pid, NULL, 0);
}
