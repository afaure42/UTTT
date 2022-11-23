#include "t_pos.hpp"

using namespace arena;

t_pos::t_pos()
:col(), row()
{
}

t_pos::t_pos(const t_pos & ref)
:col(ref.col), row(ref.row)
{

}

t_pos::t_pos(int col, int row)
:col(col), row(row)
{

}

t_pos & t_pos::operator=(const t_pos & rhs)
{
	this->col = rhs.col;
	this->row = rhs.row;

	return *this;
}
