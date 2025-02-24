# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: varodrig <varodrig@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/24 18:57:49 by varodrig          #+#    #+#              #
#    Updated: 2025/02/24 18:57:50 by varodrig         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

###################
##ARGUMENTS

NAME = philo
FLAGS = -Wall -Werror -Wextra
CC = cc

###################
##SOURCES

SRCS	= eating.c \
init.c \
main.c \
parsing.c \
philo_routine.c \
toolbox.c \
utils.c \
waiter_routine.c
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