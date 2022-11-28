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

#include <thread>
#include <mutex>

#include <functional> //std::ref

#include "IAction.hpp"
#include "IUpdate.hpp"
#include "IBoard.hpp"
#include "Player.hpp"
#include "customExceptions.hpp"
#include "utils.hpp"

#define WRITE_END	1
#define READ_END	0

#endif
