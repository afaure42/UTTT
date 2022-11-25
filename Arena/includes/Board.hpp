#ifndef BOARD_HPP
#define BOARD_HPP

#include "common.hpp"
#include "t_pos.hpp"

class t_pos;
namespace arena
{
class Board
{
public:
	Board();
	~Board();

	e_result getResult(void) const;
	const Board::e_result (&getGrid(void) const)[9][9];
	const Board::e_result (&getFinished(void) const)[3][3];


	bool isFinished(void) const;

	/*			UTILS			*/
	void printBoard(void) const;
	void printSmallBoard(void) const;

private:
	Board(const Board & other);



};
}

std::ostream & operator<<(std::ostream & os, const arena::Board & ref);


#endif
