#include "common.hpp"
#include "UTTT.hpp"

using namespace arena;

int number_of_games = 0;

void do_game(const std::vector<std::string> * players, const IBoard * board_factory, std::mutex * vars_mutex)
{
	while (true)
	{
		IBoard * current = board_factory->clone();
		vars_mutex->lock();
		if (current->getGamesPlayed() >= number_of_games)
		{
			vars_mutex->unlock();
			break;
		}
		current->getGamesPlayed()++;
		vars_mutex->unlock();

		current->resolveGame(*players);
		delete current;
	}
}

int main(int argc, char *argv[])
{
	std::mutex vars_mutex;
	IBoard * board_factory = NULL;
	std::vector<std::string> players;

	if (argc < 5)
	{
		std::cout << "ERROR: Usage ./Arena <gamemode> <nbr of games>" \
		"<nbr of threads> <player_path> *[player_path]" << std::endl;
		return (EXIT_FAILURE);
	}

	srand(time(NULL));


	//init of variables from arguments
	int number_of_threads = 1;
	try {
		number_of_games = std::stoi(argv[2]);
		number_of_threads = std::stoi(argv[3]);
	}
	catch(const std::exception& e) {
		std::cout << e.what() << '\n';
		std::cout << "Invalid value in nbr of games or nbr of threads\n";
		return (EXIT_FAILURE);
	}
	for(int i = 0; argv[i + 4] != NULL; i++)
		players.push_back(argv[i + 4]);

	//launching games
	try
	{
		std::vector<std::thread *> threads;
		threads.reserve(number_of_threads);

		for(int i = 0; i < number_of_threads; i++)
			threads.push_back(new std::thread(do_game,
					&players, board_factory, &vars_mutex));

		for (int i = 0; i < number_of_threads; i++)
		{
			threads[i]->join();
			delete threads[i];
		}
	}
	catch (const arena::syscall_error & e)
	{
		std::cerr << e.what() << " " << e.err() << '\n';
		return(EXIT_FAILURE);
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	board_factory->printTotalResults(std::cout);
	return (EXIT_SUCCESS);
}
