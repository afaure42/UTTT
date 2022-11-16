#include "common.hpp"

Node::Node()
	: bigboard(0), action(), parent(), possible_moves(),
	  wins(), visits(), terminal(false), possible_moves_size(), id(node_counter++){};

Node::Node(const bigboard_type &bigboard, const smallboard_type (&smallboards)[9])
	: bigboard(bigboard), parent(), action(), wins(), visits(), terminal(false),
	  possible_moves_size(), id(node_counter++)
{
	memcpy(this->smallboards, smallboards, sizeof(smallboard_type[9]));
	// debug = true;
	// this->possible_moves_size = list_actions(this->possible_moves, this->bigboard, this->smallboards);
	// debug = false;
}

Node::Node(const bigboard_type &bigboard, const smallboard_type (&smallboards)[9],
		   const smallboard_type &action, Node *parent)
	: bigboard(bigboard), action(action), parent(parent), wins(), visits(), terminal(false),
	  possible_moves_size(), id(node_counter++)
{
	memcpy(this->smallboards, smallboards, sizeof(smallboard_type[9]));
	apply_action(this->bigboard, this->smallboards, this->action);

	// std::cerr << "Created node from action:";
	// print_nice_action(action);

	if (isTerminal(this->bigboard))
	{
		// this->value = isWin(this->bigboard) ? 1 : (isLost(this->bigboard) ? 0 : 0.5);
		this->wins = isWin(this->bigboard) ? 1 : 0;
		this->terminal = true;
		this->visits = 1;
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
	this->wins = ref.wins;
	this->visits = ref.visits;
	this->id = ref.getId();
	this->terminal = ref.terminal;
	return *this;
}

Node::~Node()
{
	for (int i = 0; i < this->children.size(); ++i)
		delete (this->children[i]);
}

Node * Node::add_child(const smallboard_type & action)
{
	// node_vector.push_back(Node(this->bigboard, this->smallboards, action, this));
	// Node *child = &node_vector.back();
	Node * child = new Node(this->bigboard, this->smallboards, action, this);

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
	for(int i = 0; i < this->children.size(); i++)
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
		std::cerr << "NO CORRESPONDING MOVE FOUND\n";
		// node_vector.push_back(Node(this->bigboard, this->smallboards, action, NULL));
		// return &node_vector.back();
		return new Node(this->bigboard, this->smallboards, action, NULL);
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
		// std::cerr << "Finding best move, CHILDREN.SIZE:" << children.size() << "\n";
	// int terminals = 0;
	for (int i = 0; i < children.size(); i++)
	{
		//checking for immediate win
		if (isTerminal(this->children[i]->bigboard) && isWin(this->children[i]->bigboard))
		{
			// if(debug)
				// std::cerr << "returning instant win\n";
			return this->children[i];
		}

		if (this->children[i]->wins > max_wins)
		{
			ret = this->children[i];
			max_wins = ret->wins;
		}
	}
	// std::cerr << "Number of terminal childs during move choose:" << terminals << '\n';
	// if (debug)
		// std::cerr << "returning node address:" << ret << '\n';
	return ret;
}

smallboard_type Node::select_move()
{
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
