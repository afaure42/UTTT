NAME	= Arena
CC		= c++
FLAGS	= -Wall -Wextra -Werror -MMD -std=c++98 
rm		= rm -f

ARENA_INCLUDE	= ./includes/
# UTTT_INCLUDE	= ${ARENA_INCLUDE}/UTTT/

INCLUDES = ARENA_INCLUDE #UTTT_INCLUDE
INCLUDES := $(addprefix "-I", ${INCLUDES})

ARENA_PATH	=	./
UTTT_PATH	=	./UTTT/


ARENA_SRC:=			customExceptions.cpp IBoard_Exceptions.cpp main_bis.cpp \
						Player.cpp utils.cpp
ARENA_SRC			:= $(addprefix ${ARENA_PATH}, ${ARENA_SRC})

UTTT_SRC			= UTTTAction.cpp UTTTBoard.cpp UTTTUpadte.cpp
UTTT_SRC			:= $(addprefix ${UTTT_PATH}, ${UTTT_SRC})

TOTAL_SRC			= ${ARENA_SRC} ${UTTT_SRC}

OBJ					= $(TOTAL_SRC:.cpp=.o)

DEP					=	${TOTAL_SRC:.o=.d}

%.o :	%.cpp
	$(CC) $(FLAGS) ${INCLUDES} -c $< -o $@

all	:	$(NAME)

-include $(DEP)

$(NAME) : $(OBJ)
	$(ARENA_PATH)
	$(ARENA_SRC)
	$(OBJ)
	$(CC) $(FLAGS) ${INCLUDES} -o $(NAME) $(OBJ)

clean :
	${RM} $(OBJ) $(DEP)

fclean : clean
	${RM} $(NAME)

re : fclean all

.PHONY : all clean fclean re .cpp.o
