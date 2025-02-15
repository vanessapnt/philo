###################
##ARGUMENTS

NAME = philo
FLAGS = -Wall -Werror -Wextra
CC = cc

###################
##SOURCES

SRCS	= redlight.c
OBJS	= ${SRCS:.c=.o}
HEADER	= philo.h

###################
##RULES

all: ${NAME}

${NAME}: ${OBJS} ${HEADER}
		@${CC} ${FLAGS} ${OBJS} -o ${NAME} || { echo "Error during compilation of philo"; exit 1; }
		@echo "Philo compiled"

%.o: %.c
		@${CC} ${FLAGS} -c $< -o $@ || { echo "Error compiling $<"; exit 1; }

clean:
		@rm -f ${OBJS}
		@echo CLEAN DONE

fclean:	clean
		@rm -f ${NAME}
		@echo FCLEAN DONE

re: fclean all
		@echo RE DONE

.PHONY: all bonus clean fclean re