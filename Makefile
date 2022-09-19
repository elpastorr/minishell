
SRCS =	src/minishell.c src/expand.c src/init.c src/parse.c \
		src/quote.c src/token.c src/get.c src/free.c src/handler.c \
		src/cmd.c src/heredoc.c \
		src/fork.c \
		src/pipe_copy.c \
		src/utils.c \
		src/free_utils.c \
		src/print_test.c \
		src/signals.c \
		src/redir.c \
		src/built_utils.c \
		src/env_built.c src/built.c \

OBJS =	${SRCS:.c=.o}

HEADER = includes/

#DEPS =	${SRCS:.o=.d}

FLAGS =	-I $(HEADER) -Wall -Wextra -Werror -g3 #-fsanitize=address

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
