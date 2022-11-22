#include "common.hpp"

Board::e_result resolveGame(Player & player1, Player & player2)
{
	Board board();
	t_pos current(-1, -1);

	player1.launchPlayer();
	player2.launchPlayer();

	player1.sendPos(current);
	while(1)
	{
		curent = player1.recvPos();
		if (board.applyAction(current, true) != Board::NOTHING)
			break;

		player2.sendPos(current);
		current = player2.recvPos();
		if (board.applyAction(current, true) != Board::NOTHING)
			break;
	}
	return board.getResul();
}

int main()
{
	Player player1("../a.out");
	Player player2("../a.out");

	std::cout << resolveGame(player1, player2) << std::endl;

	player1.killProcess();
	player1.waitProcess();
	player2.killProcess();
	player2.waitProcess();
}
