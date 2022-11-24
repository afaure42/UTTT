#include "Player.hpp"

using namespace arena;

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
	if (this->_pid != -1)
		this->stopPlayer();

	close_pipe(this->_pipe_in);
	close_pipe(this->_pipe_out);

	if (this->_in_fd != -1)
		close(this->_in_fd);
	if (this->_in_fd != -1)
		close(this->_out_fd);
}

void Player::launchPlayer(char *name)
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
	
		char **args = (char **)malloc(2 * sizeof(char*));
		args[0] = name;
		args[1] = NULL;

		if (execvp(this->_path.c_str(), args) < 0)
			throw(arena::syscall_error(errno, "CHILD: Player::launchPlayer: execvp:"));
	}
}

void Player::stopPlayer(void)
{
	this->_killProcess();
	this->_waitProcess();
}

void Player::_killProcess(void)
{
	kill(this->_pid, SIGINT);
}

void Player::_waitProcess(void)
{
	waitpid(this->_pid, NULL, 0);
}

arena::t_pos Player::recvPos()
{
	t_pos ret;
	char buff[8];
	
	for(int i = 0;;i++)
	{
		read(this->_in_fd, buff + i, 1);
		if (buff[i] == '\n')
		{
			buff[i + 1] = '\0';
			break;
		}
	}

	ret.row = buff[0] - '0';
	ret.col = buff[2] - '0';
	return ret;
}

void Player::sendPos(arena::t_pos & pos)
{
	char buff[9];
	if (pos.col == -1)
	{
		buff[0] = '-';
		buff[1] = '1';
		buff[2] = ' ';
		buff[3] = '-';
		buff[4] = '1';
		buff[5] = '\n';
		buff[6] = '0';
		buff[7] = '\n';
		buff[8] = '\0';
	}
	else
	{
		buff[0] = '0' + pos.row;
		buff[1] = ' ';
		buff[2] = '0' + pos.col;
		buff[3] = '\n';
		buff[4] = '0';
		buff[5] = '\n';
		buff[6] = '\0';
	}

	// std::cout << buff << '\n';
	write(this->_out_fd, buff, pos.col == -1 ? 8 : 6);
}