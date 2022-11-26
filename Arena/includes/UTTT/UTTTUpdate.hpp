#ifndef UTTTUPDATE_HPP
# define UTTTUPDATE_HPP

# include "IUpdate.hpp"
# include "UTTTBoard.hpp"
# include <vector>

namespace arena
{

class UTTTUpdate : public IUpdate
{
public:
	~UTTTUpdate();
	UTTTUpdate(void);
	UTTTUpdate(const UTTTBoard::t_pos & pos,
			const std::vector<UTTTBoard::t_pos> & legal_moves);
	UTTTUpdate(const UTTTUpdate & other);

	UTTTUpdate & operator=(const UTTTUpdate & rhs);

	std::string str(void) const override;

	UTTTBoard::t_pos &				getPos();
	std::vector<UTTTBoard::t_pos> &	getMoves();

	void	setPos(const UTTTBoard::t_pos & pos);
	void	setMoves(const std::vector<UTTTBoard::t_pos> & moves);
private:

	UTTTBoard::t_pos				_enemy_pos;
	std::vector<UTTTBoard::t_pos>	_legal_moves;
}

}

#endif
