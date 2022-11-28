#ifndef IBOARD_HPP
#define IBOARD_HPP

#include <iostream>
#include <string>
#include <vector>
#include <exception>

#include "Player.hpp"

namespace arena
{
	/**
	 * @brief An interface to a board,
	 * a board represents a game and its rules, there can be many instances
	 * of the same board type runnig asynchronously to augment performances
	 * 
	 * A Board is expected to save the results of its game in static variables
	 * that can be printed using printTotalResults
	 * 
	 * And can be cleared using 
	 */
	class IBoard
	{
	public:
		virtual ~IBoard(){};

		
		/**
		 * @brief function to return new instance of current board
		 *  it will use the new operator so it will have to be freed
		 * @return IBoard* pointer to the allocated Board
		 */
		virtual IBoard * clone(void) const = 0;

		/**
		 * @brief function to print the results of all
		 * games played on this board type so far
		 */
		virtual std::ostream &	printTotalResults(std::ostream & os) const = 0;

		/**
		 * @brief function to clear the static values 
		 * of all games played on this board type
		 */
		virtual void	clearTotalResults(void) = 0;

		/**
		 * @brief Get a ref to the number of games played since last clear
		 * @return int 
		 */
		virtual int		& getGamesPlayed(void) = 0;

		/**
		 * @brief function to play a game on this board
		 * will save internally the result of the game
		 * @param players a vector containing the pathes to player binaries
		 */
		virtual void	resolveGame(const std::vector<std::string> & players) = 0;

		/**
		 * @brief function to clear a board so that it can receive
		 * a new call to resolveGame
		 */
		virtual void	clearBoard(void) = 0;

		/**
		 * @brief Get the number of players needed for this game
		 * 
		 * @return an int
		 */
		virtual int		getPlayerSize(void) const = 0;

		/**
		 * @brief  will print the current board,
		 * with or without debug info it is up
		 * to the implementor
		 */
		virtual std::ostream & write(std::ostream & os) const = 0;


		/*				EXCEPTIONS				*/



		class BoardNotEmpty : public std::exception
		{
		public:
			const char * what () const throw ();
		};

		class BadPlayerCount : public std::exception
		{
		public:
			const char * what () const throw ();
		};
	};
}

std::ostream & operator<<(std::ostream & os, const arena::IBoard & board)
{
	return board.write(os);
}

#endif
