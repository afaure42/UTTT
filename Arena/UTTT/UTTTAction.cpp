#include "UTTTAction.hpp"

using namespace arena;

UTTTAction::~UTTTAction()
{
}

UTTTAction::UTTTAction()
:_pos()
{

}

UTTTAction::UTTTAction(const UTTTBoard::t_pos & pos)
:_pos(pos)
{
}

UTTTAction::UTTTAction(const UTTTAction & other)
{
	*this = other;
}

UTTTAction & UTTTAction::operator=(const UTTTAction & rhs)
{
	this->_pos = rhs.getPos();
}

IAction *	UTTTAction::clone() const override
{
	return new UTTTAction(*this);
}

void		UTTTAction::set(const int & fd)
{
	char buff[128];

	for(int i = 0; i < 128; i++)
	{
		if (read(fd, buff + i, 1) < 0)
			throw arena::syscal_error(errno, "UTTTAction: set: read:");
		if (buff[i] == '\n')
		{
			buff[i + 1] = '\0';
			break;
		}		
	}

	this->_pos.row = buff[0] - '0';
	this->_pos.col = buff[2] - '0';
}

UTTTBoard & UTTTAction::getPos(void)
{
	return this->_pos;
}

void		UTTTAction::setPos(UTTTBoard & pos)
{
	this->_pos = pos;
}
