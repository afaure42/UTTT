#include "common.hpp"
#include "t_pos.hpp"

using namespace arena;

char name1[] = "PLAYER1";
char name2[] = "PLAYER2";

size_t player1_wins;
size_t player2_wins;
size_t player1_first;
size_t player2_first;
size_t draws;
int number_of_games;

void do_game(char *argv[], std::mutex *vars_mutex)
{
	int swap = 1;
	while (true)
	{
		vars_mutex->lock();
		if (number_of_games <= 0)
		{
			vars_mutex->unlock();
			break;
		}
		swap = number_of_games & 1;
		number_of_games--;
		vars_mutex->unlock();

		arena::Board::e_result result;


		if (swap)
			result = resolveGame(player1, player2);
		else
			result = resolveGame(player2, player1);


		vars_mutex->lock();
		if (swap)
		{
			if (result == arena::Board::PLAYER1)
				player1_wins++;
			else if (result == arena::Board::PLAYER2)
				player2_wins++;
			else
				draws++;
			player1_first++;
		}
		else
		{
			if (result == arena::Board::PLAYER1)
				player2_wins++;
			else if (result == arena::Board::PLAYER2)
				player1_wins++;
			else
				draws++;
			player2_first++;
		}
		vars_mutex->unlock();
	}
}

int main(int argc, char *argv[])
{
	if (argc != 5)
	{
		std::cout << "ERROR: Usage ./Arena <path_toplayer> <path_to_player> <number_of games> <number_of_threads>\n";
		return (EXIT_FAILURE);
	}

	srand(time(NULL));

	std::mutex vars_mutex;
	
	number_of_games = std::atoi(argv[3]);
	int number_of_thread = std::atoi(argv[4]);

	
	player1_wins = 0;
	player2_wins = 0;
	player1_first = 0;
	player2_first = 0;
	draws = 0 ;
	try
	{
		std::vector<std::thread *> threads;

		for (int i = 0; i < number_of_thread; i++)
			threads.push_back(new std::thread(do_game, argv, &vars_mutex));
		
		for (int i = 0; i < number_of_thread; i++)
		{
			threads[i]->join();
			delete threads[i];
		}
		
	}
	catch (const arena::syscall_error & e)
	{
		std::cerr << e.what() << " " << e.err() << '\n';
		exit(EXIT_FAILURE);
	}



	return (EXIT_SUCCESS);
}
