#ifndef IUPDATE_HPP
# define IUPDATE_HPP

#include <string>

namespace arena
{

class IUpdate
{
public:
	virtual ~IUpdate(){};

	virtual std::string str(void) const = 0;
};

}

#endif
