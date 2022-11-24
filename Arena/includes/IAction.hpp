#ifndef IACTION_HPP
# define IACTION_HPP

#include <string>

class IAction
{
public:
	virtual ~IAction();

	virtual IAction * clone() const = 0;
	virtual std::string str(void) const = 0;
}

#endif
