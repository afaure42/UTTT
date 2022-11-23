#include "common.hpp"

smallboard_type from_xy(smallboard_type x, smallboard_type y)
{
	smallboard_type ret = 0;

	// std::cerr << "x:" << x << "y:" << y << '\n';
	ret =  (256U >> ((((unsigned)std::floor(y / 3)) * 3 + (unsigned)std::floor(x / 3) )) << 16);
	// std::cerr << y / 3 << "* 3 + " <<  + x /3 << '\n';
	// std::cerr << (int)((((y / 3)) * 3) + (int)((int)x / 3) ) << '\n';
	ret |=  256U >> (((y % 3) * 3) + x % 3);
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

void dump_node(Node & node, std::ofstream & myfile)
{
	myfile << '\t' << node.getId() << "[label=\"[" << get_y_x(node.action) << "]\n"
	<<"P=" << ((node.bigboard & LAST_PLAYER_MASK) ? "o" : "x") 
	<< "W=" << node.wins << ";V=" << node.visits<< '\"';
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

void dump_tree(std::ofstream & myfile, Node & root)
{
	if (!myfile.is_open())
		myfile.open("tree_dump.dot");
	myfile << "strict digraph {\n";
	myfile << "\tnode [shape=\"rect\"]\n";

	dump_node(root, myfile);

	myfile << "}";
	myfile.close();
}

