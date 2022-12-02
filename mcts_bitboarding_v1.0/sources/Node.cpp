#include "common.hpp"



Node::Node()
	: bigboard(0), action(), parent(), possible_moves(),
	  possible_moves_size(), terminal(false), value(), visits(), id(node_counter++),
	  proven(false){};

Node::Node(const bigboard_type &bigboard, const smallboard_type (&smallboards)[9])
	: bigboard(bigboard), action(), parent(), possible_moves_size(),
	terminal(false), value(), visits(), id(node_counter++), proven(false)
{
	memcpy(this->smallboards, smallboards, sizeof(smallboard_type[9]));
	// debug = true;
	// this->possible_moves_size = list_actions(this->possible_moves, this->bigboard, this->smallboards);
	// debug = false;
}

Node::Node(const bigboard_type &bigboard, const smallboard_type (&smallboards)[9],
		   const smallboard_type &action, Node *parent)
	: bigboard(bigboard), action(action), parent(parent), possible_moves_size(),
	terminal(false), value(), visits(), id(node_counter++), proven(false)
{
	memcpy(this->smallboards, smallboards, sizeof(smallboard_type[9]));
	apply_action(this->bigboard, this->smallboards, this->action);

	// std::cerr << "Created node from action:";
	// print_nice_action(action);

	if (isTerminal(this->bigboard))
	{
		this->value = isWin(this->bigboard) ? 1 : (isLost(this->bigboard) ? 0 : 0.5);
		proven_node_count++;
		this->proven = true;
		if (isWin(this->bigboard) || isLost(this->bigboard))
			this->state = Node::e_state::WIN;
		// else if (isLost(this->bigboard))
			// this->state = Node::e_state::LOSE;
		else
			this->state = Node::e_state::DRAW;

		// this->value = isWin(this->bigboard) ? 1 : 0;
		this->terminal = true;
		this->visits = 1;
		this->propagateProvenState();
	}
	else
		this->possible_moves_size = list_actions(this->possible_moves, this->bigboard, this->smallboards);
}

Node::Node(const Node &ref)
{
	*this = ref;
}

Node & Node::operator=(const Node &ref)
{
	memcpy(&this->smallboards, &ref.smallboards, sizeof(smallboard_type[9]));
	memcpy(&this->possible_moves, &ref.possible_moves, sizeof(smallboard_type[81]));
	this->possible_moves_size = ref.possible_moves_size;
	this->bigboard = ref.bigboard;
	this->action = ref.action;
	this->parent = ref.parent;
	this->children = ref.children;
	// this->wins = ref.wins;
	this->value = ref.value;
	this->visits = ref.visits;
	this->id = ref.getId();
	this->terminal = ref.terminal;
	return *this;
}

Node::~Node()
{
	for (std::size_t i = 0; i < this->children.size(); ++i)
		delete (this->children[i]);
}

Node * Node::add_child(const smallboard_type & action)
{
	// node_vector.push_back(Node(this->bigboard, this->smallboards, action, this));
	// Node *child = &node_vector.back();
	std::allocator_traits<std::allocator<Node>>::construct(alloc, (nodes + nodes_index),
			this->bigboard, this->smallboards, action, this);
	Node * child = nodes + nodes_index++;
	if (nodes_index == NODES_SIZE)
		nodes_index = 0;
	// Node * child = alloc.construct(nodes + nodes_index++, this->bigboard, this->smallboards, action, this);
	// Node * child = std::construct_at(nodes + nodes_index++, this->bigboard, this->smallboards, action, this);

	this->children.push_back(child);
	return child;
}

Node * Node::remove_child(const Node * & ref)
{
	Node *ret = NULL;

	std::vector<Node *>::iterator it;

	it = std::find(this->children.begin(), this->children.end(), ref);
	if (it == this->children.end())
		return NULL;
	else
	{
		ret = *it;
		this->children.erase(it);
		return ret;
	}
}

Node * Node::select_enemy_move(const smallboard_type & action)
{
	Node * ret = NULL;


	if (debug)
	{
		std::cerr << "in enemy move\n";
		print_nice_action(action);
		std::cerr << "children size:" << this->children.size() << '\n';
	}
	for(std::size_t i = 0; i < this->children.size(); i++)
	{
		// std::cerr << "TESTING:\n";
		// print_nice_action(this->children[i]->action);
		if ((this->children[i]->action & action) == action)
		{
			// std::cerr << "Found corresponding child\n";
			// std::cerr << "Enemy move was:" << get_y_x(this->children[i]->action) << '\n';

			ret = this->children[i];
			this->children.erase(this->children.begin() + i);
			break;
		}
	}
	if (ret == NULL)
	{
		// std::cerr << "NO CORRESPONDING MOVE FOUND\n";
		// node_vector.push_back(Node(this->bigboard, this->smallboards, action, NULL));
		// return &node_vector.back();
		std::allocator_traits<std::allocator<Node>>::construct(alloc, (nodes + nodes_index),
				this->bigboard, this->smallboards, action, (Node *)NULL);
		ret = nodes + nodes_index++;
		if (nodes_index == NODES_SIZE)
			nodes_index = 0;
		return ret;
		// return alloc.construct(nodes + nodes_index++, this->bigboard, this->smallboards, action, (Node *)NULL);
		// return std::construct_at(nodes + nodes_index++, this->bigboard, this->smallboards, action, NULL);
	}
	else
	{
		ret->parent = NULL;
		return ret;
	}
}

Node * Node::get_best_move()
{
	int max_wins = 0;
	Node * ret = NULL;

	// if (debug)
		// std::cerr << "CHILDREN SIZE:" << children.size() << '\n';
	// if (isTerminal(this->bigboard))
		// print_nice_bigboard(this->smallboards, this->bigboard);
	// int terminals = 0;
	// std::cerr << children.size() << std::endl;
	for (std::size_t i = 0; i < children.size(); i++)
	{
		//checking for immediate win
		if ((isTerminal(this->children[i]->bigboard) && isWin(this->children[i]->bigboard))
			|| (this->children[i]->proven && this->children[i]->state == Node::e_state::WIN))
		{
			// if(debug)
				// std::cerr << "returning instant win\n";
			if (this->proven)
				std::cerr << "Returning proven win\n";
			return this->children[i];
		}

		if (this->children[i]->value >= max_wins)
		{
			ret = this->children[i];
			max_wins = ret->value;
		}
	}
	// std::cerr << "Number of terminal childs during move choose:" << terminals << '\n';
	// if (debug)
		// std::cerr << "returning node address:" << ret << '\n';
	if (!ret)
		print_nice_bigboard(this->smallboards, this->bigboard);
	return ret;
}

smallboard_type Node::select_move()
{
	if (this->possible_moves_size == 0)
		std::cerr << "FPE INCOMING BE CAREFULL" << std::endl;
	int move = rand() % this->possible_moves_size;
	smallboard_type ret = this->possible_moves[move];
	// std::cerr << "BEFORE:\n";
	// for(int i = 0; i < this->possible_moves_size; i++)
		// std::cerr << "[" << get_y_x(this->possible_moves[i]) << "]";
	// std::cerr << "CHOSEN ONE IS :" << get_y_x(this->possible_moves[move]) << '\n';
	memmove(this->possible_moves + move, this->possible_moves + move + 1, 
			sizeof(smallboard_type) * (this->possible_moves_size-- - move));
	// std::cerr << "AFTER:\n";
	// for(int i = 0; i < this->possible_moves_size; i++)
		// std::cerr << "[" << get_y_x(this->possible_moves[i]) << "]";
	// std::cerr << '\n';
	// smallboard_type ret = this->possible_moves[--this->possible_moves_size];
	return ret;
}

int Node::getId(void) const {
	return this->id;
}


void Node::propagateProvenState(void)
{
	if (!this->parent) return;
	if (!this->proven) return;

	if (this->state == WIN)
	{
		this->parent->proven = true;
		proven_node_count++;
		this->parent->state = LOSE;
		this->parent->propagateProvenState();
		return;
	}

	//checking siblings
	//if not all siblings have been made we cant check further
	if (this->parent->possible_moves_size != 0)
		return ;


	bool all_siblings_proven = true;
	bool all_siblings_lose = true;
	for(std::size_t i = 0; i < this->parent->children.size(); i++)
	{
		if (this->parent->children[i]->proven == false)
		{
			all_siblings_lose = false;
			all_siblings_proven = false;
			break;
		}
		if (this->parent->children[i]->state == Node::e_state::WIN
			|| this->parent->children[i]->state == Node::e_state::DRAW)
			all_siblings_lose = false;
	}

	if (this->state == Node::e_state::DRAW && all_siblings_proven)
	{
		this->parent->proven = true;
		proven_node_count++;
		this->parent->state = Node::e_state::DRAW;
		this->parent->propagateProvenState();
	}

	else if (all_siblings_lose) //current node == DRAW is implicit
	{
		this->parent->proven = true;
		proven_node_count++;
		this->parent->state = Node::e_state::WIN;
		this->parent->propagateProvenState();
	}

}
