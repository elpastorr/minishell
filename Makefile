SRCS =	src/minishell.c src/expand.c src/init.c src/parse.c \
		src/quote.c src/token.c src/get.c src/free.c src/handler.c \
		src/cmd.c

OBJS =	${SRCS:.c=.o}

HEADER = includes/

#DEPS =	${SRCS:.o=.d}

FLAGS =	-I $(HEADER) -Wall -Wextra -Werror

NAME =	minishell

%.o: 	%.c
	@ gcc ${FLAGS} -c $< -o $@

all:		${NAME}

${NAME}:	${OBJS}
	@ make -C libft --silent
	@ gcc ${FLAGS} ${OBJS} libft/libft.a -lreadline -o ${NAME}

clean:		${OBJS}
	@ make clean -C libft --silent
	@ rm -rf ${OBJS} ${DEPS}

fclean:		clean
	@ make fclean -C libft --silent
	@ rm -rf ${NAME}

re:			fclean
	@ make all

#-include ${DEPS}