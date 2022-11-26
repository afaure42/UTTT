#include "UTTTUpdate.hpp"


UTTTUpdate::~UTTTUpdate()
{

}
UTTTUpdate::UTTTUpdate(const UTTTBoard::t_pos & pos,
		const std::vector<UTTTBoard::t_pos> & legal_moves)
{
}

UTTTUpdate::UTTTUpdate(const UTTTUpdate & other)
{
	*this = other;
}

UTTTUpdate & UTTTUpdate::operator=(const UTTTUpdate & rhs)
{
	this->_pos = rhs.getPos();
	this->_legal_moves = rhs.getMoves();

	return *this;
}

std::string UTTTUpdate::str(void) const override
{
}

UTTTBoard::t_pos &				UTTTUpdate::getPos() {
	return this->_pos;
}

std::vector<UTTTBoard::t_pos> &	UTTTUpdate::getMoves() {
	return this->_legal_moves;
}

void	UTTTUpdate::setPos(const UTTTBoard::t_pos & pos) {
	this->_pos = pos;
}

void	UTTTUpdate::setMoves(const std::vector<UTTTBoard::t_pos> & moves) {
	this->_legal_moves = moves;
}
