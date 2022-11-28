#ifndef T_POS_HPP
# define T_POS_HPP

namespace arena
{
	struct t_pos
	{
		t_pos();
		t_pos(const t_pos & ref);
		t_pos(int col, int row);
		t_pos & operator=(const t_pos & rhs);

		int col;
		int row;
	};
}

#endif
