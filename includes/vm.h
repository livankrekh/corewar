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

typedef	unsigned char byte;
typedef	int	t_bool;

typedef struct			s_players
{
	header_t			header;
	char				*comands;
	int					live;
	int					live_amount;
	int					num;
	int					vis_num;
	int 				pos;
	unsigned char		*reg;
}						t_players;

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
void					vizualize(byte *memory, t_players *players, WINDOW **win1, WINDOW *win);
void					status_bar(WINDOW **win2, WINDOW *win, t_players *players);
void					cursor_refresh(WINDOW *win1, WINDOW *win2, t_players *players, byte *map);
void					zjmp(t_players *player, byte *map);
void					live(t_players *players, byte map);
void					sti(t_players *player, byte *map);

#endif
