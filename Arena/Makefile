NAME	= Arena
CC		= c++
FLAGS	= -Wall -Wextra -Werror -MMD -std=c++98 ${DEFINES}
rm		= rm -f

SRC					=
INCLUDES			=	-I./includes
				
OBJ					=	$(SRC:.cpp=.o)

DEP					=	${SRC:.o=.d}

.cpp.o:
	$(CC) $(FLAGS) ${INCLUDES} -c $< -o $@

all : $(NAME)

-include $(DEP)

$(NAME) : $(OBJ_)
	$(CC) $(FLAGS) ${INCLUDES} -o $(NAME) $(OBJ)

clean :
	${RM} $(OBJ) $(DEP)

fclean : clean
	${RM} $(NAME)

re : fclean all

.PHONY : all clean fclean re .cpp.o
