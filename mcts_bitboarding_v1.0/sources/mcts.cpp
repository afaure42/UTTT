#include "common.hpp"

typedef std::vector<Node *>::iterator	  node_iter;
typedef std::vector<Node>::const_iterator	 cst_node_iter;

float ucb1(const Node &node)
{
	if (node.visits == 0)
		return 0;
	return (node.value / (float)node.visits + C * sqrt(log(node.parent->visits) / node.visits));
}

Node * select_child(Node & node)
{
	Node * best_node = NULL;
	float best_value;

	for(node_iter iter = node.children.begin(); iter != node.children.end(); iter++)
	{
		float current_value = ucb1(**iter);

		if (best_node == NULL)
		{
			best_node = *iter;
			best_value = current_value;
			continue;
		}

		if (current_value > best_value)
		{
			best_node = *iter;
			best_value = current_value;
		}

	}
	return best_node;
}

void expand(Node * node)
{

}

float rollout(
		bigboard_type bigboard,
		const smallboard_type (&start_boards)[9],
		const smallboard_type (&possible_actions)[81],
		int size)
{
	smallboard_type smallboards[9];
	smallboard_type actions[81];


	// debug = false;
	memcpy(smallboards, start_boards, sizeof(smallboard_type[9]));
	memcpy(actions, possible_actions, sizeof(smallboard_type[81]));
	// if (node_counter == 3) {
		// print_nice_bigboard(smallboards, bigboard);
	// }
	smallboard_type current_action;
	// turn = 0;
	while (1)
	{
		// if (node_counter == 3)
			// print_nice_bigboard(smallboards, bigboard);
		current_action = actions[rand() % size];
		// if (debug) {
			// std::cerr << "ROLLOUT TURN:" << turn << '\n';
		// }
		apply_action(bigboard, smallboards, current_action);

		if (isTerminal(bigboard))
			break;
		
		// std::cerr << "PLAYING IN :[" << get_y_x(current_action) << "]\n";
		size = list_actions(actions, bigboard, smallboards);
		// turn++;
	}
	// debug = true;
	// if (node_counter == 3)
	// {
		// print_nice_bigboard(smallboards, bigboard);
		// print_bigboard(bigboard);
	// }
	if (isWin(bigboard))
		return 1;
	if (isLost(bigboard))
		return 0;
	else
		return 0.5;
}

void update_path(Node * node)
{
	float value = node->value;
	node = node->parent;

	while(node != NULL)
	{
		node->value += value;
		++node->visits;
		node = node->parent;
	}
}

int terminal_nodes = 0;

void mcts_iteration(Node * node)
{
	if (node == NULL)
		std::cerr << "Brace for impact\n";
	// std::cerr << "Selecting" << std::endl;
	while(node->possible_moves_size == 0 && !isTerminal(node->bigboard))
		node = select_child(*node);

	// if (isTerminal(node->bigboard))
	// {
		// terminal_nodes++;
		// std::cerr << node->id << '\n';
	// }

	// std::cerr << "Selection finished" << std::endl;
	if (!isTerminal(node->bigboard))
	{
		// std::cerr << "expanding" << std::endl;
		smallboard_type action = node->select_move();
		// std::cerr << "expanding" << std::endl;
		node = node->add_child(action);
	}

	if(!isTerminal(node->bigboard))
	{
		// std::cerr << "Starting Rollout" << std::endl;
		node->value = rollout(node->bigboard, node->smallboards, node->possible_moves, node->possible_moves_size);
		node->visits = 1; 
	}

	update_path(node);
}

long elapsed_time(clock_t t1)
{
	return ((double)clock() - t1) / CLOCKS_PER_SEC * 1000;
}

Node * mcts(Node * root, int max_time, clock_t start_time)
{
	int i = 0;

	// std::cerr << "Starting MCTS iterations\n";
	#ifdef FIXED_ROLLOUT
	while (i < ROLLOUT_PER_TURN)
	#else
	while (elapsed_time(start_time) < max_time)
	#endif
	{
		mcts_iteration(root);
		// i++;
	}

	// std::cerr << "rollouts this turn: " << i << '\n';
	// total_rollouts += i;
	// turns++;
	return root->get_best_move();
}
