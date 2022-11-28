#include "UTTTUpdate.hpp"

using namespace arena;

UTTTUpdate::UTTTUpdate()
{
}

UTTTUpdate::~UTTTUpdate()
{

}
UTTTUpdate::UTTTUpdate(const t_pos & pos,
		const std::vector<t_pos> & legal_moves)
{
}

UTTTUpdate::UTTTUpdate(const UTTTUpdate & other)
{
	*this = other;
}

UTTTUpdate & UTTTUpdate::operator=(const UTTTUpdate & rhs)
{
	this->_enemy_pos = rhs.getPos();
	this->_legal_moves = rhs.getMoves();

	return *this;
}

std::string UTTTUpdate::str(void) const
{
	std::string ret;

	ret.append(std::to_string(this->getPos().row));
	ret.push_back(' ');
	ret.append(std::to_string(this->getPos().col));
	ret.push_back('\n');
	
	ret.append("0\n");

	return ret;
}

const t_pos &				UTTTUpdate::getPos() const {
	return this->_enemy_pos;
}

const std::vector<t_pos> &	UTTTUpdate::getMoves() const {
	return this->_legal_moves;
}

t_pos &				UTTTUpdate::getPos() {
	return this->_enemy_pos;
}

std::vector<t_pos> &	UTTTUpdate::getMoves() {
	return this->_legal_moves;
}

void	UTTTUpdate::setPos(const t_pos & pos) {
	this->_enemy_pos = pos;
}

void	UTTTUpdate::setMoves(const std::vector<t_pos> & moves) {
	this->_legal_moves = moves;
}
