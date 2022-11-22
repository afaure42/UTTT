#include "utils.hpp"

void close_pipe(int pype[2])
{
	close(pype[0]);
	close(pype[1]);
}
