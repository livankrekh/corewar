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

# include <ncurses.h>
# include "op.h"
# include "../libft/libft.h"
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>
// # include "../SDL2.framework/Versions/A/Headers/SDL.h"
// # include "../SDL2_mixer.framework/Versions/A/Headers/SDL_mixer.h"
# define RED "\x1b[31m"
# define END "\033[0m"
# define HEX "0123456789abcdef"
# include <fcntl.h>
# include <unistd.h>
# define MAP_SIZE (MEM_SIZE - 1)

typedef	unsigned char 		byte;
typedef	int					t_bool;
typedef struct s_players 	t_players;

typedef struct          s_op
{
    char                name[20];
    int                 arguments;
    int                 r_d_i[10];
    int                 number;
    int                 cycles;
    char                desriptions[200];
    int                 octage;
    int                 carry;
    int                 label_size;
}                       t_op;

struct					s_players
{
	header_t			header;
	char				*comands;
	int					live;
	int					live_amount;
	int					num;
	int 				pos;
	unsigned int		*reg;
	int					stop;
	int					carry;
	int					*cycles;
	int					curr_com;
	int					*last_herro;
	int					last_live;
	int					st;
	t_players			*next;
};

typedef struct			s_flags
{
	int 				visual;
	int					dump;
	int					amount_players;
	int					amount_num;
	int					paused;
	int					process;
	int					speed;
	int					base;
	int					cycles;
	int					DIE;
	int					cycles_test;
	int					max_checks;
}						t_flags;

t_players	*copy;

void					print_comands(void);
void					print_data_players(t_players **players);
void					print_error(char *str);
int						get_flags(t_flags *flags, char **argv);
int						get_players(t_players *players, char **argv, int argc, t_flags *flags);
int						is_digit(char *str);
t_flags					*create_flags(void);
t_players				*create_players(void);
long long int			ft_atoiLong(const char *str);
int						ft_swap_players(t_players **players, int amount_players);
int						translate(byte r1, byte r2, byte r3, byte r4);
void					vizualize(byte *memory, t_players *players, WINDOW **win1, WINDOW *win);
void					status_bar(WINDOW **win2, t_players *players);
void					cursor_refresh(WINDOW *win1, WINDOW *win2, t_players *players, byte *map);
void					cursor_refresh_stack(WINDOW *win1, WINDOW *win2, t_players *players, byte *map);
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
void					aff(t_players *player, byte *map);
void					start_vm(t_players **tmp, int count, t_flags *flags);
void					get_stop(t_players *player, byte *map);
int 					translate(byte r1, byte r2, byte r3, byte r4);
int						get_TDIR(int n, int posit, byte *map);
int						get_REG(t_players *player, int posit, byte *map);
int						get_INDIR(t_players *player, char flag, int posit, byte *map);
int						check_oppcode(t_players *player, byte *map);

#endif
