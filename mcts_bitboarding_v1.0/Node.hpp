#ifndef NODE_HPP
#define NODE_HPP

#include "common.hpp"

typedef	uint_fast16_t	pos_type;
typedef uint_fast32_t	smallboard_type;
typedef uint_fast64_t 	bigboard_type;

class Node
{
public:
    Node();
	Node(	const bigboard_type & bigboard,
			const smallboard_type (&smallboards)[9]);
	Node(	const bigboard_type & bigboard, const smallboard_type (&smallboards)[9],
			const smallboard_type & action, Node * parent);
    Node(const Node & ref);
    Node & operator=(const Node & ref);
	~Node();

	void	apply_action(const smallboard_type & action);
	void	list_actions(void);

	Node * add_child(const smallboard_type & action);
	Node * remove_child(const Node * & ref);
	Node * select_enemy_move(const smallboard_type & action);
	Node * get_best_move();
	smallboard_type select_move();
	int	getId(void) const;

    bigboard_type bigboard;
	smallboard_type smallboards[9];
    smallboard_type action;

    Node * parent;
    std::vector<Node *> children;
    smallboard_type possible_moves[81];
	int possible_moves_size;
    bool terminal;
    // float value;
	int wins;
    int visits;
	int id;
private:
};

#endif
