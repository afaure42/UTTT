#ifndef UTTTACTION_HPP
#define UTTTACTION_HPP


#include "t_pos.hpp"
#include "IAction.hpp"
#include "customExceptions.hpp"
#include <unistd.h>

// #include "UTTT.hpp"

namespace arena
{

class UTTTAction : public IAction
{
public:
	~UTTTAction();
	UTTTAction();
	UTTTAction(const arena::t_pos & pos);
	UTTTAction(const UTTTAction & other);

	UTTTAction & operator=(const UTTTAction & rhs);

	IAction *	clone() const override;

	void		set(const int & fd) override;

	arena::t_pos & getPos(void);
	const	arena::t_pos & getPos(void) const;
	
	void		setPos(arena::t_pos & pos);

private:
	arena::t_pos	_pos;
};

}

#endif
