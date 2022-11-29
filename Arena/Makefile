NAME	= Arena
CC		= c++
FLAGS	= -Wall -Wextra -Werror -MMD
rm		= rm -f

ARENA_INCLUDE	= ./includes/
UTTT_INCLUDE	= ${ARENA_INCLUDE}/UTTT/

INCLUDES = ${ARENA_INCLUDE} ${UTTT_INCLUDE}
INCLUDES := $(addprefix -I, ${INCLUDES})

ARENA_PATH	=	./
UTTT_PATH	=	./UTTT/


UTTT_SRC			=	UTTTAction.cpp UTTTBoard.cpp UTTTUpdate.cpp \
						t_pos.cpp
UTTT_SRC			:= $(addprefix ${UTTT_PATH}, ${UTTT_SRC})

ARENA_SRC			=	customExceptions.cpp IBoard.cpp \
						Player.cpp utils.cpp main_bis.cpp
ARENA_SRC			:= $(addprefix ${ARENA_PATH}, ${ARENA_SRC})

TOTAL_SRC			= ${ARENA_SRC} ${UTTT_SRC}

OBJ					= $(TOTAL_SRC:.cpp=.o)

DEP					=	${OBJ:%.o=%.d}

%.o :	%.cpp
	$(CC) $(FLAGS) ${INCLUDES} -c $< -o $@

all	:	$(NAME)

-include $(DEP)

$(NAME) : $(OBJ)
	$(CC) $(FLAGS) ${INCLUDES} -o $(NAME) $(OBJ)

clean :
	${RM} ${OBJ} ${DEP}
fclean : clean
	${RM} $(NAME)

re : fclean all

.PHONY : all clean fclean re .cpp.o
