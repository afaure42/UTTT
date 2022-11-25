#ifndef IBOARD_HPP
#define IBOARD_HPP

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
		virtual ~IBoard();

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
		virtual void	printResults(void) const = 0;

		/**
		 * @brief function to play a game on this board
		 * will update the internal static values relative to the
		 * result of this game
		 * @param players array of players
		 */
		virtual void	resolveGame(Player * players) = 0;

		/**
		 * @brief function to clear the static values 
		 * of all games played on this board type
		 */
		virtual void	clearResults(void) = 0;
		
		/**
		 * @brief Get the number of players needed for this game
		 * 
		 * @return 
		 */
		virtual int		getPlayerSize(void) const = 0;

		/**
		 * @brief  will print the current board,
		 * with or without debug info it is up
		 * to the implementor
		 */
		virtual void	print(void) const = 0;
	}
}

#endif
