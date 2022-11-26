#include "UTTTBoard.hpp"

using namespace arena;

UTTTBoard::t_pos::t_pos()
:col(), row()
{
}

UTTTBoard::t_pos::t_pos(const UTTTBoard::t_pos & ref)
:col(ref.col), row(ref.row)
{

}

UTTTBoard::t_pos::t_pos(int col, int row)
:col(col), row(row)
{

}

UTTTBoard::t_pos & UTTTBoard::t_pos::operator=(const t_pos & rhs)
{
	this->col = rhs.col;
	this->row = rhs.row;

	return *this;
}
