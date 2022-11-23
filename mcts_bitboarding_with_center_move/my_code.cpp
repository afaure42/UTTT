#include "common.hpp"
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

/*				GLOBALS				*/

uint_fast32_t to_index[257];
bool debug = false;
unsigned long node_counter = 0;
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
//please dont ask me to explain this you'll just see blood tears running from my eyes
//and we dont want that do we ?

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

// std::vector<Node> node_vector;

//PLAYER1 IS ALWAYS US, PLAYER2 ALWAYS THE ENEMY, PLAYER1 is ALWAYS TRUE

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
	if (action & TO_MOVE_MASK)
    	big_board |= LAST_PLAYER_MASK;
	else
		big_board &= ~LAST_PLAYER_MASK;
	if (debug) {
	std::cerr << "ACTION FROM PLAYER" << (big_board & LAST_PLAYER_MASK ? "1" : "2") << '\n';
	}

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
            current |= ((PLAYER_WIN_MASK) << player_offset);
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
                big_board |= ((PLAYER_WIN_MASK) << player_offset);
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
	if (debug)
		std::cerr << "LISTING ACTIONS FOR PLAYER" << (player ? "1" : "2") << '\n';

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

bool isTerminal(const bigboard_type & bigboard) {
	return (bigboard & FINISH_MASK);
}

bool isWin(const bigboard_type & bigboard) {
	return (bigboard >> PLAYER1_OFFSET) & PLAYER_WIN_MASK;
}

bool isLost(const bigboard_type & bigboard) {
	return (bigboard) & PLAYER_WIN_MASK;
}


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

int main(int argc, char **argv)
{
	try {
	to_index_init();
	srand(time(NULL));
	std::ofstream tree_file;
	smallboard_type smallboards[9];
	bigboard_type bigboard = 0;
	// node_vector.reserve(100000);
    memset(smallboards, 0, sizeof(smallboard_type[9]));
	// node_vector.push_back(Node(bigboard, smallboards));
    // Node * root = &node_vector.back();
	Node * root = new Node(bigboard, smallboards);
	Node * permanent_root = root;
	// std::cerr << '\n';
	int opponent_row;
	int opponent_col;
	Node * current;
    // game loop
    while (1) 
    {
		current = root;
		clock_t now = clock();
        cin >> opponent_row >> opponent_col; cin.ignore();
		// cerr << argv[0] << ":input received:" << opponent_row << " " << opponent_col << std::endl;
        if (opponent_row != -1)
        {
            // std::cerr << "Removing unusefull branches\n";
			smallboard_type action = 0;
			// std::cerr << elapsed_time(now) << std::endl;
			action = from_xy(opponent_col, opponent_row);
			// print_nice_action(action);

			// std::cerr << "Enemy Action:\n";
			// print_nice_action(action);
            current = root->select_enemy_move(action);
            delete root;
			root = current;
        }
		else
		{
			current->bigboard &= ~LAST_PLAYER_MASK;
			current->possible_moves[0] = from_xy(4, 4);
			current->possible_moves[0] |= TO_MOVE_MASK;
			current->possible_moves_size = 1;
		}

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
        current = mcts(current, 60, now);
		// for(int i = 0; i < current->children.size(); i++)
			// std::cerr << "[" << get_y_x(current->children[i]->action) << "]";
		// std::cerr << '\n';
		
		dump_tree(tree_file, *permanent_root);
		// std::cerr << "Terminal nodes :" << terminal_nodes << '\n';
        // std:cerr << "Number of childs in best move node:" << current->children.size() << 
        // " score:" << current->wins << ", visits:" << current->visits << '\n';
        // print_nice_bigboard(current->smallboards, current->bigboard);
        // std::cerr << "LET ME GUESS" << std::endl;
        current->parent = NULL;
        // delete root;
        root = current;
		// print_nice_action(current->action);
        // print_nice_bigboard(current->smallboards, current->bigboard);

		// cerr << "sending:" << get_y_x(current->action) << endl;
        cout << get_y_x(current->action) << endl;
		if (opponent_col == -1)
			print_nice_bigboard(current->smallboards, current->bigboard);
    }
	}
	catch (std::exception & e)
	{
		std:: cerr << "EXCEPTION OCCURED: " << e.what() << std::endl;
	}
}
