#ifndef IACTION_HPP
# define IACTION_HPP

#include <string>

namespace arena
{
/**
 * @brief a class representing an action taken by a player
 * you should use this interface to receive action from players
 * by overriding the set function, reading and parsing from the fd
 */
class IAction
{
public:
	virtual ~IAction(){};

	virtual IAction * clone() const = 0;


	virtual void	set(const int & fd) = 0;
};

}
#endif
