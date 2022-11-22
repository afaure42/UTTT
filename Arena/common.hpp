#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <exception>

#include <cstring>//memset
#include <stdio.h>//pipe
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Board.hpp"
#include "Player.hpp"
#include "customExceptions.hpp"
#include "utils.hpp"

#define WRITE_END	1
#define READ_END	0


struct t_pos
{
	t_pos();
	t_pos(const t_pos & ref):col(ref.col), row(ref.row)
	{}
	t_pos(int col, int row):col(col), row(row)
	{}
	int col;
	int row;
};

#endif
