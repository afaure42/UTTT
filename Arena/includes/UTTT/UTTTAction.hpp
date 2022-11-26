#ifndef UTTTACTION_HPP
#define UTTTACTION_HPP

#include "IAction.hpp"
#include "UTTTBoard.hpp"

namespace arena
{

class UTTTAction : public IAction
{
public:
	~UTTTAction();
	UTTTAction();
	UTTTAction(const UTTTBoard::t_pos & pos);
	UTTTAction(const UTTTAction & other);

	UTTTAction & operator=(const UTTTAction & rhs);

	IAction *	clone() const override;

	void		set(const int & fd) override;

	UTTTBoard & getPos(void);
	
	void		setPos(UTTTBoard & pos);

private:
	UTTTBoard::t_pos	_pos;
};

}

#endif
