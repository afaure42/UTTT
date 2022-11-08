#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>
#include <map>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <bitset>
#include <fstream>
#pragma GCC optimize ("O3")

using namespace std;
//seed=510707828

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
// 256 = 0
// 128 = 1
// 64 = 2
// 32 = 3
// 16 = 4
// 8 = 5
// 4 = 6
// 2 = 7
// 1 = 8
// std::map<uint_fast32_t, uint_fast8_t> to_index
// {
//     std::make_pair(256, 0),
//     std::make_pair(128, 1),
//     std::make_pair(64, 2),
//     std::make_pair(32, 3),
//     std::make_pair(16, 4),
//     std::make_pair(8, 5),
//     std::make_pair(4, 6),
//     std::make_pair(2, 7),
//     std::make_pair(1, 8)
// };

uint_fast32_t to_index[257];

void to_index_init()
{
	to_index[256] =  (uint_fast8_t)0;
	to_index[128]= (uint_fast8_t)1;
	to_index[64]= (uint_fast8_t)2;
	to_index[32]= (uint_fast8_t)3;
	to_index[16]= (uint_fast8_t)4;
	to_index[8]= (uint_fast8_t)5;
	to_index[4]= (uint_fast8_t)6;
	to_index[2]= (uint_fast8_t)7;
	to_index[1] = (uint_fast8_t)8;
}

const uint_fast32_t wins[]
{
    0b0000000000000111,
    0b0000000000111000, //3 rows
    0b0000000111000000,
    0b0000000100100100,
    0b0000000010010010, // 3 col
    0b0000000001001001,
    0b0000000100010001, // 2 diag
    0b0000000001010100,
};

class Node;

// std::vector<Node> node_vector;
//    0 1 2
//    3 4 5
//    6 7 8

// TRUE = PLAYER1
//              GAME BOARD
//                LAST_MOVE            DRAW                         PLAYER1    PLAYER2 WINNER  PLAYER2
//  | 0000 000[0 0000 0000] | 0000 000[0 0000 0000] |[0][0]00 000[0 0000 0000] [0][0]00 000[0 0000 0000]
//                                                  ^  ^IS PLAYER1 WINNER     ^Last Player to play 
//                                              Game finished
#define LAST_MOVE_MASK	0xFFFF000000000000
#define LAST_MOVE_OFFSET		48

#define LAST_PLAYER_MASK		0x8000
#define LAST_PLAYER_OFFSET		15
#define FINISH_MASK				0x80000000
#define FINISH_OFFSET			31
#define PLAYER_WIN_MASK     	0x4000
#define PLAYER_WIN_OFFSET   	14
#define DRAW_OFFSET        		32

//             ACTION BOARD
//           BIG BOARD REF        SMALL BOARD REF           
// [0]000 000[0 0000 0000] | 0000 000[0 0000 0000] 
// PLAYER TO MOVE
#define TO_MOVE_MASK        0x80000000
#define TO_MOVE_OFFSET		31

#define POSITION_MASK       0x01FF
#define PLAYER1_OFFSET      16

#define WIN_NUMBER 8
#define PLAYER1_SCORE   1
#define PLAYER2_SCORE   0
#define DRAW_SCORE      0.5

#define C               1.2f

//PLAYER1 IS ALWAYS US, PLAYER2 ALWAYS THE ENEMY, PLAYER1 is ALWAYS TRUE

typedef	uint_fast16_t	pos_type;
typedef uint_fast32_t	smallboard_type;
typedef uint_fast64_t 	bigboard_type;

//please dont ask me to explain this you'll just see blood tears running from my eyes
//and we dont want that do we ?
//[0][0][0]
//[0][0][0]
//[0][0][0]
int get_x(const smallboard_type & action);
int get_y(const smallboard_type & action);
// bool debug = true;
unsigned long node_counter = 0;
// unsigned turn = 0;

smallboard_type from_xy(smallboard_type x, smallboard_type y)
{
	smallboard_type ret = 0;

	// std::cerr << "x:" << x << "y:" << y << '\n';
	ret =  (256U >> (((unsigned)std::floor(y / 3)) * 3 + (unsigned)std::floor(x / 3) )) << 16;
	// std::cerr << y / 3 << "* 3 + " <<  + x /3 << '\n';
	// std::cerr << (int)((((y / 3)) * 3) + (int)((int)x / 3) ) << '\n';
	ret |=  256U >> ((y % 3) * 3) + x % 3;
	// std::cerr << ((y % 3) * 3) + x % 3 << '\n';
	return ret;
}

int get_x(const smallboard_type & action){
	return ((to_index[action >> 16 & POSITION_MASK] % 3) * 3
			+ to_index[action & POSITION_MASK] % 3);
}

int get_y(const smallboard_type & action) {
	return (((to_index[action >> 16 & POSITION_MASK] / 3) * 3)
			+ (to_index[action & POSITION_MASK] / 3));
}

std::string get_y_x(const smallboard_type & action) {
	std::string ret;

	ret.push_back(get_y(action) + '0');
	ret.push_back(' ');
	ret.push_back(get_x(action) + '0');
	return ret;
}

void print_nice_action(const smallboard_type & action)
{
	std::string bits = std::bitset<32>(action).to_string();
	

	std::cerr << "ACTION:\n"
	<< bits.substr(0, 8) << ' ' << bits.substr(8, 8) << ' ' << bits.substr(16, 8) << ' ' << bits.substr(24, 8) << '\n'
	<< "PLAYER:" << ((action & TO_MOVE_MASK) ? "PLAYER1" : "PLAYER2")
	<< "\nBIGBOARD_ID:" <<  (u_int)to_index[(action >> 16) & POSITION_MASK]
	<< "\nSMALLBOARD_ID:" << (u_int)to_index[action & POSITION_MASK]
	<< std::endl;
}

void print_bigboard(bigboard_type & bigboard)
{
    std::string bits = std::bitset<64>(bigboard).to_string();

    std::cerr << "LAST PLAY["
    << bits.substr(0, 4) << ' ' << bits.substr(4, 4) << ' ' << bits.substr(8, 4) << ' ' << bits.substr(12, 4)
    << "]\n" << "DRAW:["
    << bits.substr(16, 4) << ' ' << bits.substr(20, 4) << ' ' << bits.substr(24, 4) << ' ' << bits.substr(28, 4)
    << "]\nPLA1:["
    << bits.substr(32, 4) << ' ' << bits.substr(36, 4) << ' ' << bits.substr(40, 4) << ' ' << bits.substr(44, 4)
    << "]\nPLA2:["
    << bits.substr(48, 4) << ' ' << bits.substr(52, 4) << ' ' << bits.substr(56, 4) << ' ' << bits.substr(60, 4)
	<< "]\n";
}

void print_nice_bigboard(const smallboard_type *boards, const bigboard_type bigboard)
{
	// if (debug) {
		// print_nice_action(boards[7]);
	// }
    for(int i = 0; i < 9; i++)
    {
       
        for (int j = 0; j < 9; j++)
        {
			smallboard_type pos = from_xy(j, i);

            if ( boards[to_index[(pos >> 16) & POSITION_MASK]] & (pos & POSITION_MASK))
                std::cerr << 'x';
            else if ( boards[to_index[(pos >> 16) & POSITION_MASK]] & ((pos & POSITION_MASK) << 16))
                std::cerr << 'o';
            else
                std::cerr << '-';
			if (j == 2 || j == 5)
				std::cerr << '|';
        }
        std::cerr << '\n';
		if (i == 2 || i == 5)
			std::cerr << '\n';
    }
}

void apply_action(bigboard_type & big_board, smallboard_type *boards, const smallboard_type & action)
{
	// if (debug)
		// print_nice_action(action);
	// if (node_counter == 82)
		// print_nice_bigboard(boards, big_board);

    smallboard_type small_pos = ((action >> 16) & POSITION_MASK);
    smallboard_type & current = boards[to_index[small_pos]];
	// if (debug && (turn == 50 || turn == 51))
		// print_nice_action(current);
	// if (debug) {
		// std::cerr << "small_pos:" << small_pos
		// << ";index:" << (int)to_index[small_pos] <<'\n';
	// }

    //switching last player using XOR (this will just flip the bit)
	if (big_board != 0)
    	big_board ^= LAST_PLAYER_MASK;
	// if (debug) {
	// std::cerr << "ACTION FROM PLAYER" << (big_board & LAST_PLAYER_MASK ? "1" : "2") << '\n';
	// }

    //adding the small move to the history ( so i can test for possible actions later)
    big_board = big_board & ~LAST_MOVE_MASK;
    big_board |= (action & POSITION_MASK) << LAST_MOVE_OFFSET;
	// if (debug) {
	// std::cerr << "Adding Small Move\n";
	// print_bigboard(big_board);}

    //determining wich player is doing the move
    short         player_offset    = (action & TO_MOVE_MASK ) ? 16 : 0;
    
    //adding the move to the player in the small board
    current |= ((action & 0x01FF) << player_offset);

    //set the bit on the little board to set last player
    current |= (action & TO_MOVE_MASK) >> 16;

    //then test if little board is finished
	// if (debug) {
	// std::cerr << "Modifications should be over\n";
	// print_nice_action(action);
	// print_bigboard(big_board);
	// print_nice_action(current);
	// print_nice_action(boards[7]);
	// print_nice_bigboard(boards, big_board);
	// }

	
    //first testing for a win
    for (int i = 0; i < WIN_NUMBER; i++)
    {
        //only testing for one player since the other cant have won
        if ( ((current >> player_offset) & wins[i]) == wins[i])
        {
            current |= ((1 << PLAYER_WIN_OFFSET) << player_offset);
            big_board |= ((action >> 16) & POSITION_MASK) << player_offset; 
            current |= FINISH_MASK;
            break;
        }
    }

    //then testing for a DRAW
    if (!(current & FINISH_MASK) && ((current | current >> 16) & POSITION_MASK) == POSITION_MASK)
    {
        current |= FINISH_MASK;
        big_board |= ((action >> 16) & POSITION_MASK) << DRAW_OFFSET;
    }

    //if so update big board
    if (current & FINISH_MASK)
    {
        //test if big board is finished
        for(int i = 0; i < WIN_NUMBER; i++)
        {
            if ( ((smallboard_type)(big_board >> player_offset) & wins[i]) == wins[i])
            {
                big_board |= FINISH_MASK;
                big_board |= ((1 << PLAYER_WIN_OFFSET) << player_offset);
                break;
            }
        }
        if (!(big_board & FINISH_MASK) && ((big_board | big_board >> 16 | big_board >> 32) & POSITION_MASK) == POSITION_MASK)
            big_board |= FINISH_MASK;
    }
	/*if (debug && (turn == 50 || turn == 51))
		print_nice_action(current);
	if (debug && current & FINISH_MASK)
	{
		std::cerr << "DETECTED A WIN DURING ACTION:\n";
		print_nice_action(action);
		print_nice_bigboard(boards, big_board);
	}
	if (debug && big_board & FINISH_MASK)
	{
		std::cerr << "DETECTED A BIG WIN\n";
		print_bigboard(big_board);
	}*/
}

float get_board_score(bigboard_type & bigboard)
{
    if (bigboard & PLAYER_WIN_OFFSET)
        return (PLAYER2_SCORE);
    if (bigboard >> 16 & PLAYER_WIN_OFFSET)
        return (PLAYER1_SCORE);
    return (DRAW_SCORE);
}

smallboard_type create_action(
	const pos_type & bigboard_pos, 
	const pos_type & smallboard_pos,
	const bool & player)
{
	smallboard_type ret = 0;

	ret = bigboard_pos;
	ret = ret << 16;
	ret |= smallboard_pos;
	ret |= player << TO_MOVE_OFFSET;

	// std::cerr << "create action returned:";
	// print_nice_action(ret);
	return ret;
}

void inline list_smallboard_actions(smallboard_type (&list)[81], int & size,
	pos_type bigboard_pos, smallboard_type smallboard, bool player)
{
	if (smallboard & FINISH_MASK)
		return;
	//keeping only the positions of both player combined
	smallboard = (smallboard | (smallboard >> 16)) & POSITION_MASK;
	/*if (debug && turn == 50) {
		std::cerr << "bigboard_pos:" << (int)to_index[bigboard_pos] << '\n';
		print_nice_action(smallboard);
	}*/

	for(int i = 0; i < 9; i++)
	{
		if(!(smallboard & (1U << i)))
			list[size++] = (create_action(bigboard_pos, 1 << i, player));
	}
}
// uint16_t getDrawPos(const bigboard_type & type)
int list_actions(smallboard_type (&list)[81],
    const bigboard_type & bigboard, const smallboard_type * smallboard)
{
	int size = 0;
	pos_type last_move = (bigboard >> LAST_MOVE_OFFSET) & POSITION_MASK;

	bool player = !(bigboard & LAST_PLAYER_MASK);
	// if (debug)
		// std::cerr << "LISTING ACTIONS FOR PLAYER" << (player ? "1" : "2") << '\n';

    //testing if targeted move space is full
    if (smallboard[to_index[last_move]] & FINISH_MASK || last_move == 0)
	{
 	   for(int i = 0; i < 9; i++)
	   {
		// if (debug && turn == 50)
			// std::cerr << "LISTING ACTIONS FOR BOARD:" << i << '\n';
		list_smallboard_actions(list, size, 1 << (8 - i), smallboard[i], player);
	   }
	}
	else
		list_smallboard_actions(list, size, last_move, smallboard[to_index[last_move]], player);
	/*if (debug && turn == 50) {
	std::cerr << "LISTING ACTIONS FOR BIGBOARD:\n"
	<< "IS FINISHED:" << (smallboard[to_index[bigboard >> LAST_MOVE_OFFSET]] & FINISH_MASK)<< '\n';
	print_nice_action(smallboard[to_index[bigboard >> LAST_MOVE_OFFSET]]);

	print_nice_bigboard(smallboard, bigboard);
	for(int i = 0; i < list.size(); i++)
		std::cerr << "[" << get_y_x(list[i]) << "]";
	std::cerr << '\n';}
	*/
	return (size);
}

inline bool isTerminal(const bigboard_type & bigboard) {
	return (bigboard & FINISH_MASK);
}

inline bool isWin(const bigboard_type & bigboard) {
	return (bigboard >> PLAYER1_OFFSET) & PLAYER_WIN_MASK;
}

inline bool isLost(const bigboard_type & bigboard) {
	return (bigboard) & PLAYER_WIN_MASK;
}

class Node
{
public:
    Node()
    :bigboard(0), action(), parent(), possible_moves(),
		wins(), visits(), terminal(false), possible_moves_size(), id(node_counter++)
	{};

    Node(const bigboard_type & bigboard, const smallboard_type (&smallboards)[9])
    :bigboard(bigboard), parent(), action(), wins(), visits(), terminal(false),
		possible_moves_size(), id(node_counter++)
    {
		this->bigboard |= LAST_PLAYER_MASK;
		memcpy(this->smallboards, smallboards, sizeof(smallboard_type[9]));
		this->possible_moves_size = list_actions(this->possible_moves, this->bigboard, this->smallboards);
    }

    Node(const bigboard_type & bigboard, const smallboard_type (&smallboards)[9],
		const smallboard_type & action, Node * parent)
    :bigboard(bigboard), action(action), parent(parent), wins(), visits(), terminal(false),
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
    Node(const Node & ref)
	{
		*this = ref;
	}

    Node & operator=(const Node & ref)
    {
        memcpy(&this->smallboards, &ref.smallboards, sizeof(smallboard_type[9]));
		memcpy(&this->possible_moves, &ref.possible_moves, sizeof(smallboard_type[81]));
		this->bigboard = ref.bigboard;
        this->action = ref.action;
        this->parent = ref.parent;
        this->children = ref.children;
        this->wins = ref.wins;
        this->visits = ref.visits;
		this->id = ref.getId();
		this->terminal = ref.terminal;
        return *this;
    };

    ~Node()
    {
        for(int i = 0; i < this->children.size(); ++i)
            delete(this->children[i]);
    }

    Node * add_child(const smallboard_type & action)
    {
		// node_vector.push_back(Node(this->bigboard, this->smallboards, action, this));
        // Node *child = &node_vector.back();
		Node * child = new Node(this->bigboard, this->smallboards, action, this);

        this->children.push_back(child);
        return child;
    }

    Node * remove_child(const Node * & ref)
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

    Node * select_enemy_move(const smallboard_type & action)
    {
        Node * ret = NULL;


        // std::cerr << "in enemy move\n";
        // std::cerr << "children size:" << this->children.size() << '\n';
        for(int i = 0; i < this->children.size(); i++)
        {
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

    Node * get_best_move()
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

    smallboard_type select_move()
    {
        smallboard_type ret = this->possible_moves[this->possible_moves_size--];
        return ret;
    }

	int getId(void) const {
		return this->id;
	}
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

/*void print_board(const int (&board)[3][3])
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            std::cerr << '['
            << ((board[i][j] == PLAYER1) ? 'X' : ((board[i][j] == PLAYER2) ? 'O' : ' ')) << ']';
        }
        std::cerr << '\n';
    }
}*/

void dump_node(Node & node, ofstream & myfile)
{
	myfile << '\t' << node.getId() << "[label=\"[" << get_y_x(node.action) << "]\nW=" << node.wins << ";V=" << node.visits<< '\"';
	if (isTerminal(node.bigboard))
	{
		myfile << " color=\"";
		if (isWin(node.bigboard))
			myfile << "green";
		else if (isLost(node.bigboard))
			myfile << "red";
		else
			myfile << "blue";
		myfile << '\"';
	}
	myfile <<"]\n";
	for(int i = 0; i < node.children.size(); i++) {
		myfile << '\t' << node.getId() << " -> " << node.children[i]->getId() << std::endl;
	}
	for (int i = 0; i < node.children.size(); i++) {
		dump_node(*(node.children[i]), myfile);
	}
}

void dump_tree(ofstream & myfile, Node & root)
{
	if (!myfile.is_open())
		myfile.open("tree_dump.dot");
	myfile << "strict digraph {\n";
	myfile << "\tnode [shape=\"rect\"]\n";

	dump_node(root, myfile);

	myfile << "}";
	myfile.close();
}


typedef vector<Node *>::iterator      node_iter;
typedef vector<Node>::const_iterator     cst_node_iter;

float ucb1(const Node &node)
{
    if (node.visits == 0)
        return 0;
    return ((float)node.wins / node.visits + C * sqrt(log(node.parent->visits) / node.visits));
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
        return 0;
}

void update_path(Node * node)
{
    int wins = node->wins;
    node = node->parent;

    while(node != NULL)
    {
        node->wins += wins;
        ++node->visits;
        node = node->parent;
    }
}

int terminal_nodes = 0;

void mcts_iteration(Node * node)
{
	if (node == NULL)
		std:cerr << "Brace for impact\n";
    // std::cerr << "Selecting" << std::endl;
    while(node->possible_moves_size == 0 && !isTerminal(node->bigboard))
        node = select_child(*node);

	if (isTerminal(node->bigboard))
	{
		terminal_nodes++;
		// std::cerr << node->id << '\n';
	}

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
        node->wins = rollout(node->bigboard, node->smallboards, node->possible_moves, node->possible_moves_size);
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

    std::cerr << "Starting MCTS iterations\n";
    while (elapsed_time(start_time) < max_time)
    {
        mcts_iteration(root);
        i++;
    }

    std::cerr << "rollouts this turn: " << i << '\n';
    return root->get_best_move();
}

int main()
{
	try {
	to_index_init();
	std::ofstream tree_file;
	smallboard_type smallboards[9];
	bigboard_type bigboard = 0;
	// node_vector.reserve(100000);
    memset(smallboards, 0, sizeof(smallboard_type[9]));
	// node_vector.push_back(Node(bigboard, smallboards));
    // Node * root = &node_vector.back();
	Node * root = new Node(bigboard, smallboards);
	Node * permanent_root = root;
    std::cerr << "root possible moves:"<< root->possible_moves_size << "\n";
	for(int i = 0; i < root->possible_moves_size; i++)
		std::cerr << "[" << get_y_x(root->possible_moves[i]) << "]";
	std::cerr << '\n';
	int opponent_row;
	int opponent_col;
	Node * current;
    // game loop
    while (1) 
    {
		std::bitset<32> bits(FINISH_MASK);
		std::cerr << bits.to_string() << std::endl;
		current = root;
		clock_t now = clock();
        cin >> opponent_row >> opponent_col; cin.ignore();
        if (opponent_row != -1)
        {
            // std::cerr << "Removing unusefull branches\n";
			smallboard_type action = 0;
			// std::cerr << elapsed_time(now) << std::endl;
			action = from_xy(opponent_col, opponent_row);
			print_nice_action(action);

			// std::cerr << "Enemy Action:\n";
			// print_nice_action(action);
            current = root->select_enemy_move(action);
            delete root;
			root = current;
        }
        std::cerr << "Not Crashed yet" << std::endl;

        int valid_action_count;
        cin >> valid_action_count; cin.ignore();
        for (int i = 0; i < valid_action_count; i++) {
            int row;
            int col;
            cin >> row >> col; cin.ignore();
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;


        // std::cerr << "Before MCTS\n";
		// print_bigboard(current->bigboard);
        current = mcts(current, 70, now);
        print_nice_bigboard(current->smallboards, current->bigboard);
		// for(int i = 0; i < current->children.size(); i++)
			// std::cerr << "[" << get_y_x(current->children[i]->action) << "]";
		// std::cerr << '\n';
		
		dump_tree(tree_file, *current);
		// std::cerr << "Terminal nodes :" << terminal_nodes << '\n';
        std:cerr << "Number of childs in best move node:" << current->children.size() << 
        " score:" << current->wins << ", visits:" << current->visits << '\n';
        // std::cerr << "LET ME GUESS" << std::endl;
        current->parent = NULL;
        // delete root;
        root = current;
		// print_nice_action(current->action);
        // print_nice_bigboard(current->smallboards, current->bigboard);

        cout << get_y_x(current->action) << endl;
    }
	}
	catch (std::exception & e)
	{
		std:: cerr << e.what() << std::endl;
	}
}
