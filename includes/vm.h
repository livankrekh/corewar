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

# include "op.h"
# include "../libft/libft.h"
# include <SDL2/SDL.h>
# include "../SDL2_ttf.framework/Versions/A/Headers/SDL_ttf.h"
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>
# define RED "\x1b[31m"
# define END "\033[0m"
# define true 1;
# define false 0;

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
	struct t_players	*next;
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
void					vizualize(byte *memory, int size);
void					zjmp(t_players *player, byte *map);
void					live(t_players *players, byte map, int count);
void					sti(t_players *player, byte *map, unsigned int *reg);

#endif
