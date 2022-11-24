#ifndef COMMON_HPP
#define COMMON_HPP

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
#include <csignal>
#include "Node.hpp"

class Node;
typedef	uint_fast16_t	pos_type;
typedef uint_fast32_t	smallboard_type;
typedef uint_fast64_t 	bigboard_type;

/*			GLOBALS			*/
extern uint_fast32_t to_index[257];
extern const uint_fast32_t wins[];
extern unsigned long node_counter;
extern bool debug;
extern int total_rollouts;
extern int turns;

//BINARY DEFINES
//    0 1 2
//    3 4 5
//    6 7 8

// TRUE = PLAYER1
//              GAME BOARD
//                LAST_MOVE            DRAW                         PLAYER1    PLAYER2 WINNER  PLAYER2
//  | 0000 000[0 0000 0000] | 0000 000[0 0000 0000] |[0][0]00 000[0 0000 0000] [0][0]00 000[0 0000 0000]
//                                                  ^  ^IS PLAYER1 WINNER     ^Last Player to play 
//                                              Game finished
// PLAYER1 = o
//PLAYER2 = x
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

/********************************/
/*			BITBOARDS			*/
/********************************/
void			apply_action(
	bigboard_type & big_board,
	smallboard_type *boards,
	const smallboard_type & action);

smallboard_type	create_action(
	const pos_type & bigboard_pos, 
	const pos_type & smallboard_pos,
	const bool & player);

int list_actions(
	smallboard_type (&list)[81],
    const bigboard_type & bigboard,
	const smallboard_type * smallboard);

bool isTerminal(const bigboard_type & bigboard);
bool isWin(const bigboard_type & bigboard);
bool isLost(const bigboard_type & bigboard);



/**************/
/* 		UTILS */
/**************/
//		PRINT
void print_nice_action(const smallboard_type & action);
void dump_node(Node & node, std::ofstream & myfile);
void dump_tree(std::ofstream & myfile, Node & root);
void print_bigboard(bigboard_type & bigboard);
void print_nice_bigboard(const smallboard_type *boards, const bigboard_type bigboard);

//else
smallboard_type from_xy(smallboard_type x, smallboard_type y);
int get_x(const smallboard_type & action);
int get_y(const smallboard_type & action);
std::string get_y_x(const smallboard_type & action);



/********************/
/*		MCTS 		*/
/********************/

#ifndef ROLLOUT_PER_TURN
# define ROLLOUT_PER_TURN 100000
#endif

#ifndef MS_PER_TURN
# define MS_PER_TURN 60
#endif

#ifndef C
#define C               0.7f
#endif
float ucb1(const Node &node);
Node * select_child(Node & node);
float rollout(
		bigboard_type bigboard,
		const smallboard_type (&start_boards)[9],
		const smallboard_type (&possible_actions)[81],
		int size);
void update_path(Node * node);
void mcts_iteration(Node * node);
long elapsed_time(clock_t t1);
Node * mcts(Node * root, int max_time, clock_t start_time);

#endif
