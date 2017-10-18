# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/03/20 16:55:36 by liabanzh          #+#    #+#              #
#    Updated: 2017/03/20 16:55:38 by liabanzh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME1 = corewar
CC = gcc
SRC = src/go.c src/funcs.c src/vizualize.c src/viz_dop.c \
	src/create_fun.c src/get_flags.c src/get_players.c src/help_func.c src/main.c \
	src/print_func.c
LIB = libft/libft.a
FLAGS = -Wall -Wextra -Werror
# SDL_FLAG = -F. -framework SDL2
# SDL_mixer = -I /vm/SDL2_mixer.framework/Versions/A/Headers \
# 			-F ./ -framework SDL2_mixer


.PHONY: all re clean fclean

all: $(NAME1)

$(NAME1): $(LIB) $(SRC:.c=.o)
	$(CC) -o $(NAME1) $(SRC:.c=.o) $(LIB) $(FLAGS) -lncurses

$(LIB):
	make -C ./libft

%.o: %.c
	@$(CC) -o $@ -c $< $(FLAGS)

clean:
	make clean -C ./libft
	rm -f $(SRC:.c=.o) $(MAIN1:.c=.o) $(MAIN2:.c=.o)

fclean: clean
	rm -f $(LIB) $(NAME1) $(NAME2)
	
re: fclean all
