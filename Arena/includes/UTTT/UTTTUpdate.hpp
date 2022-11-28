#ifndef UTTTUPDATE_HPP
# define UTTTUPDATE_HPP

# include "UTTT.hpp"
# include <vector>


namespace arena
{

class UTTTUpdate : public IUpdate
{
public:
	~UTTTUpdate();
	UTTTUpdate(void);
	UTTTUpdate(const arena::t_pos & pos,
			const std::vector<arena::t_pos> & legal_moves);
	UTTTUpdate(const UTTTUpdate & other);

	UTTTUpdate & operator=(const UTTTUpdate & rhs);

	std::string str(void) const override;

	arena::t_pos &				getPos();
	std::vector<arena::t_pos> &	getMoves();
	const arena::t_pos &				getPos() const;
	const std::vector<arena::t_pos> &	getMoves() const;

	void	setPos(const arena::t_pos & pos);
	void	setMoves(const std::vector<arena::t_pos> & moves);
private:

	arena::t_pos				_enemy_pos;
	std::vector<arena::t_pos>	_legal_moves;
};

}

#endif
