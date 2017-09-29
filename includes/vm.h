/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mponomar <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/08 17:10:54 by mponomar          #+#    #+#             */
/*   Updated: 2017/07/08 17:12:39 by mponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H

// # if defined(linux) || defined(_WIN32)
// # include <GL/glut.h>
// # else
// # include <GLUT/GLUT.h>
// # endif
# include <ncurses.h>
# include "op.h"
# include "../libft/libft.h"
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>
# define RED "\x1b[31m"
# define END "\033[0m"

typedef	unsigned char 		byte;
typedef	int					t_bool;
typedef struct s_players 	t_players;

struct					s_players
{
	header_t			header;
	char				*comands;
	int					live;
	int					live_amount;
	int					num;
	int					vis_num;
	int 				pos;
	unsigned int		*reg;
	int					stop;
	int					carry;
	int					*live_ptr;
	t_players			*next;
};

typedef struct			s_flags
{
	int 				visual;
	int					dump;
}						t_flags;

// void					print_comands(void);
// int						get_flags(t_flags *flags, char **argv);
// void					print_error(char *str);
// t_flags					*create_flags(void);
// t_players				*get_players(char **argv, int argc);
// t_players 				*create_players(void);
int						translate(byte r1, byte r2, byte r3, byte r4);
void					vizualize(byte *memory, t_players *players, WINDOW **win1, WINDOW *win);
void					status_bar(WINDOW **win2, WINDOW *win, t_players *players);
void					cursor_refresh(WINDOW *win1, WINDOW *win2, t_players *players, byte *map);
void					zjmp(t_players *player, byte *map);
void					live(t_players *players, byte *map, t_players *player);
void					sti(t_players *player, byte *map);
void					and_xor(t_players *player, byte *map, char flag);
char					*get_binary(byte *map, t_players *player);
void					ld(t_players *player, byte *map);
void					st(t_players *player, byte *map);
void					add(t_players *player, byte *map);
void					sub(t_players *player, byte *map);
void					ldi(t_players *player, byte *map);
void					lldi(t_players *player, byte *map);
void					lld(t_players *player, byte *map);
void					fork_func(t_players *player, byte *map, t_players **stack);
void					lfork_func(t_players *player, byte *map, t_players **stack);
void					refresh_map(WINDOW *win1, byte *map);

#endif
