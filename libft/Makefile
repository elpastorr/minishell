# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: elpastor <elpastor@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/25 14:52:12 by elpastor          #+#    #+#              #
#    Updated: 2022/09/21 15:35:32 by elpastor         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS = ft_atoi.c \
ft_isprint.c \
ft_putchar_fd.c \
ft_striteri.c \
ft_strnstr.c \
ft_bzero.c \
ft_itoa.c \
ft_putendl_fd.c \
ft_strjoin.c \
ft_strrchr.c \
ft_calloc.c \
ft_memchr.c \
ft_putnbr_fd.c \
ft_strlcat.c \
ft_strtrim.c \
ft_isalnum.c \
ft_memcmp.c \
ft_putstr_fd.c \
ft_strlcpy.c \
ft_substr.c \
ft_isalpha.c \
ft_memcpy.c \
ft_split.c \
ft_strlen.c \
ft_tolower.c \
ft_isascii.c \
ft_memmove.c \
ft_strchr.c \
ft_strmapi.c \
ft_toupper.c \
ft_isdigit.c \
ft_memset.c \
ft_strdup.c \
ft_strncmp.c \
ft_newstr.c ft_strcat.c ft_strndup.c ft_memdel.c ft_strcmp.c ft_isspace.c


OBJS = ${SRCS:.c=.o}

BONUS = ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c ft_lstmap.c

BONUS_OBJS = ${BONUS:.c=.o}

FLAGS = -Wall -Wextra -Werror -g3

all:	${NAME}

${NAME}: ${OBJS}
		ar rc ${NAME} ${OBJS}
		ranlib ${NAME}
clean:
		rm -f ${OBJS} ${BONUS_OBJS}

fclean:	clean
		rm -f ${NAME}

re:		fclean all

bonus: ${OBJS} ${BONUS_OBJS}
		ar rc ${NAME} ${OBJS} ${BONUS_OBJS}

.PHONY: all clean fclean re bonus
