/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   go.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/18 17:55:40 by liabanzh          #+#    #+#             */
/*   Updated: 2017/07/18 17:55:41 by liabanzh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"
#include "../includes/op.h"
#include <unistd.h>

void	get_stop(t_players *player, byte *map)
{
	int 	stop[16] = {9, 4, 4, 9, 9, 5, 5, 5, 19, 24, 24, 799, 9, 49, 999, 1};
	if (map[player->pos] > 0 && map[player->pos] <= 16)
		player->stop = stop[map[player->pos] - 1];
	else
		player->stop = 0;
	player->curr_com = map[player->pos];
}

void	delete_split(char ***split)
{
	int		i;

	i = 0;
	while (*split[i] != NULL)
	{
		free(*split[i]);
		*split[i] = NULL;
		i++;
	}
	free(*split);
	*split = NULL;
}

byte	*get_map(t_players *players, int count, int *cycles)
{
	unsigned char	*map;
	int				i;
	int				j;
	char			*ptr;
	char			*tmp;

	j = 1;
	i = 0;
	map = (unsigned char*)malloc(sizeof(unsigned char) * MEM_SIZE);
	ptr = players[0].comands;
	players[0].cycles = cycles;
	while (i < MEM_SIZE && j <= count)
	{
		if (i >= (MEM_SIZE / count) * j)
		{
			ptr = players[j].comands;
			players[j].cycles = cycles;
			j++;
		}
		if (ptr != NULL && *ptr != '\0')
		{
			tmp = ft_strnew(2);
			map[i] = ft_atoi_base(ft_strncpy(tmp, ptr, 2), 16);
			ptr += 2;
			free(tmp);
		}
		else
			map[i] = 0;
		i++;
	}
	return (map);
}

void	get_command(t_players *player, byte *map, t_players **stack, t_players *players)
{
	if (player->stop == 0)
	{
		if (player->curr_com == 1)
			live(players, map, player);
		else if (player->curr_com == 2)
			ld(player, map);
		else if (player->curr_com == 3)
			st(player, map);
		else if (player->curr_com == 4)
			add(player, map);
		else if (player->curr_com == 5)
			sub(player, map);
		else if (player->curr_com == 6)
			and_xor(player, map, 'a');
		else if (player->curr_com == 7)
			and_xor(player, map, 'o');
		else if (player->curr_com == 8)
			and_xor(player, map, 'x');
		else if (player->curr_com == 9)
			zjmp(player, map);
		else if (player->curr_com == 10)
			ldi(player, map);
		else if (player->curr_com == 11)
			sti(player, map);
		else if (player->curr_com == 12)
			fork_func(player, map, stack);
		else if (player->curr_com == 13)
			lld(player, map);
		else if (player->curr_com == 14)
			lldi(player, map);
		else if (player->curr_com == 15)
			lfork_func(player, map, stack);
		else if (player->curr_com == 16)
			aff(player, map);
		else
			player->pos += 1;
		player->pos = player->pos % MEM_SIZE;
		get_stop(player, map);
	}
	else
		player->stop -= 1;
}

int 	get_alive_players(t_players *players)
{
	int 		res;
	int 		i;
	t_players	*tmp;

	res = 0;
	i = 0;
	tmp = players;
	while (tmp[i].header.prog_name[0] != '\0')
	{
		if (tmp[i].comands != NULL)
			res++;
		i++;
	}
	return (res);
}

int 	get_last(t_players *players)
{
	int 		i;
	int 		res;

	i = 0;
	res = i;
	while (players[i].header.prog_name[0] != '\0')
	{
		printf("Player #%d \'%s\' with last_live = %d\n", i + 1, players[i].header.prog_name, players[i].last_live);
		if (*(players[i].last_herro) == players[i].num)
			res = i;
		if (players[i].comands != NULL)
			free(players[i].comands);
		i++;
	}
	printf("LAST CYCLE = %d\n", *(players[0].cycles));
	return (res);
}

void	end_game(t_players *players, byte *map, t_players **stack, t_flags *flags)
{
	int 		i;
	t_players	*tmp;
	t_players	*tmp2;
	int ch;
	int y;

	y = 0;
	i = 0;
	free(map);
	map = NULL;
	tmp = *stack;
	while (tmp != NULL)
	{
		tmp2 = tmp->next;
		free(tmp->reg);
		free(tmp);
		tmp = tmp2;
	}
	*stack = NULL;
	i = get_last(players);
	if (flags->visual == 0)
	{
		printf("Contestant %d \'%s\', has won!\n", players[i].num * -1, players[i].header.prog_name);
		i = 0;
		while (players[i].header.prog_name[0] != '\0')
		{
			free(players[i].reg);
			players[i].reg = NULL;
			i++;
		}
		exit(1);
	}
	else
	{
		if (flags->amount_players == 2)
			y += 4;
		else if (flags->amount_players == 3)
			y += 8;
		else if (flags->amount_players == 4)
			y += 12;
		attron(COLOR_PAIR(8) | A_BOLD);
		mvwprintw(stdscr, 30 + y, 199, "The winner is : ");
		attroff(COLOR_PAIR(8) | A_BOLD);
		attron(COLOR_PAIR(i + 1) | A_BOLD);
		mvwprintw(stdscr, 30 + y, 215, "%s", players[i].header.prog_name);
		attroff(COLOR_PAIR(i + 1) | A_BOLD);
		attron(COLOR_PAIR(8) | A_BOLD);
		mvwprintw(stdscr, 32 + y , 199, "Press 'Esc' to finish.");
		attroff(COLOR_PAIR(8) | A_BOLD);
		while (1)
		{
			if ((ch = getch()) == 27)
			{
				endwin();
				exit(1);
			}
		}
	}
}

int 	get_lives(t_players *players)
{
	int 	res;
	int 	i;

	res = 0;
	i = 0;
	while (players[i].header.prog_name[0] != '\0')
	{
		res += players[i].live + players[i].live_amount;
		i++;
	}
	return (res);
}

int 	check_count_proccess(t_players *stack)
{
	int 	res;

	res = 0;
	while (stack)
	{
		res++;
		stack = stack->next;
	}
	return (res);
}

void	check_dead_proccess(t_players **stack)
{
	t_players	*tmp;
	t_players	*tmp2;

	tmp = *stack;
	tmp2 = NULL;
	while (tmp)
	{
		if (tmp->live + tmp->live_amount == 0)
		{
			// if (*stack == tmp2)
			// 	*stack = tmp2->next;
			// else
			// 	tmp2->next = tmp->next;
			// free(tmp->reg);
			// tmp->reg = NULL;
			// free(tmp);
			// tmp = tmp2->next;
			if (tmp2 == NULL)
				*stack = tmp->next;
			else
				tmp2->next = tmp->next;
			free(tmp->reg);
			tmp->reg = NULL;
			free(tmp);
			if (tmp2 == NULL)
				tmp = *stack;
			else
				tmp = tmp2->next;
		}
		else
		{
			tmp->live = 0;
			tmp->live_amount = 0;
			tmp2 = tmp;
			tmp = tmp->next;
		}
	}
}

void	check_end(t_players *players, byte *map, t_players **stack, t_flags *flags)
{
	int 	i;

	i = 0;
	while (players[i].header.prog_name[0] != '\0')
	{
		if (players[i].live + players[i].live_amount == 0 && players[i].comands != NULL)
		{
			free(players[i].comands);
			players[i].comands = NULL;
		}
		players[i].live = 0;
		players[i].live_amount = 0;
		i++;
	}
	check_dead_proccess(stack);
	if (get_alive_players(players) == 0 && *stack == NULL)
		end_game(players, map, stack, flags);
}

void	print_map(byte *map)
{
	int 	i;

	i = 0;
	while (i < MEM_SIZE)
	{
		if (i % 64 == 0)
			printf("\n0x%04x : ", i);
		printf("%02x ", map[i]);
		i++;
	}
	printf("\n\n");
}

void	go_vm(t_players *players, int count, t_flags *flags)
{
	byte		*map;
	t_players	*stack;
	t_players	*tmp;
	int			i;

	i = 0;
	flags->cycles = 1;
	flags->cycles_test = 1;
	copy = NULL;
	stack = NULL;
	flags->DIE = CYCLE_TO_DIE;
	flags->max_checks = 0;
	map = get_map(players, count, &(flags->cycles));
	while (players[i].header.prog_name[0] != '\0')
		get_stop(&(players[i++]), map);
	while (flags->DIE > 0)
	{
		if (flags->dump == flags->cycles)
			print_map(map);
		tmp = stack;
		while (tmp != NULL)
		{
			get_command(tmp, map, &stack, players);
			tmp = tmp->next;
		}
		i = count - 1;
		while (i >= 0)
		{
			if (players[i].comands != NULL)
				get_command(&(players[i]), map, &stack, players);
			i--;
		}
		if (flags->cycles_test >= flags->DIE)
		{
			i = 0;
			if (get_lives(players) >= NBR_LIVE || flags->max_checks == 10)
			{
				flags->DIE -= CYCLE_DELTA;
				flags->max_checks = 0;
			}
			else
				flags->max_checks++;
			check_end(players, map, &stack, flags);
			flags->cycles_test = 0;
		}
		flags->cycles++;
		flags->cycles_test++;
	}
	end_game(players, map, &stack, flags);
}


































void	players_vis_area(t_players *players, t_flags *flags)
{
	int i; // номер игрока
	int y; // передвежение игрику вниз карты
	int x; // передвежение по иксу вправо
	int counter; // движение по вей карте по которой нам нужно печатать
	int size; // площадка памяти выделеная под каждого игрока
	int wow; // инетратор для заполнение этой площадки и если что то заполнением 00 до конца
	char *tmp;
	char *tmp1;

	size = MEM_SIZE / flags->amount_players;
	i = 0;
	y = 2;
	x = 3;
	counter = 0;
	if (counter < MEM_SIZE)
	{
		while (i < flags->amount_players)
		{
			wow = 0;
			tmp = players[i].comands;
			while (*tmp != '\0')
			{
				if (x > 192)
				{
					x = 3;
					y++;
				}
				tmp1 = ft_strnew(2);
				ft_strncpy(tmp1, tmp, 2);
				attron(COLOR_PAIR(i + 1));
				mvwprintw(stdscr, y, x, "%s ", tmp1);
				attroff(COLOR_PAIR(i + 1));
				tmp += 2;
				x += 3;
				counter++;
				wow++;
				free(tmp1);
			}
			while (wow < size)
			{
				if (x > 192)
				{
					if (i == 2)
					{
						attron(COLOR_PAIR(33) | A_BOLD);
						mvwprintw(stdscr, 65, 192, "00 ");
						attroff(COLOR_PAIR(33) | A_BOLD);
					}	
					x = 3;
					y++;
				}
				attron(COLOR_PAIR(33) | A_BOLD);
				mvwprintw(stdscr, y, x, "00 ");
				attroff(COLOR_PAIR(33) | A_BOLD);
				x += 3;
				counter++;
				wow++;
			}
			i++;
		}
	}
}

char	*get_hex(byte val)
{
	char	*tmp;
	char	*res;

	tmp = ft_itoa_base(val, 16);
	res = ft_strnew(2);
	if (ft_strlen(tmp) == 1)
	{
		res[1] = tmp[0];
		res[0] = '0';
	}
	else
		return (tmp);
	free(tmp);
	return (res);
}

void    board_kay(t_flags *flags)
{
    noecho();
    int ch;

    ch = getch();
    if (ch == 113) // Q
    {
    	if (flags->speed > 1)
    		flags->speed--;
    }
    else if (ch == 101) // E
    {
    	if (flags->speed < 250)
    		flags->speed++;
    }
    else if (ch == 32) //space
    {
    	if (flags->paused == 0)
    		flags->paused = 1;
    	else
    		flags->paused = 0;
    }
    else if (ch == 27) // esc
    {
		endwin();
		exit(1);
    }
}

void	print_board()
{
	int i;

	i = 0;
	attron(COLOR_PAIR(34));
	while (i < 254)
	{
		mvwprintw(stdscr, 0, i, "*");
		mvwprintw(stdscr, 67, i, "*");
		i++;
	}
	i = 0;
	while (i < 67)
	{
		mvwprintw(stdscr, i, 0, "*");
		mvwprintw(stdscr, i, 253, "*");
		mvwprintw(stdscr, i, 196, "*");
		i++;
	}
	attroff(COLOR_PAIR(34));
}

int     size_processes(t_players **stack)
{
	t_players *tmp;
	int i;

	tmp = *stack;
	i = 0;
	while (tmp != NULL)
	{
		i++;
		tmp = tmp->next;
	}
	return (i);
}

int 	print_rc_next(int *did)
{
	int i;
	int size;

	size = 0;
	i = 0;
	while (i < 4)
	{
		if (did[i] > 0)
			size++;
		i++;
	}
	return (size);
}

void 	print_rc_end(int skiko, int color, int pos, int y)
{
	int i;

	i = 0;
	while (i < skiko)
	{
		attron(COLOR_PAIR(color + 1));
		mvwprintw(stdscr, y, pos + i, "-");
		attroff(COLOR_PAIR(color + 1));
		i++;
	}
}


int     find_i(int *i, int *did, int skiko, int all)
{
	double res;

	res = 0;
	while (*i < 4)
	{
		if (did[*i] > 0)
			break;
		(*i)++;
	}
	res = all - did[*i];
	if (did[*i] > res)
	{
		res = ((double)did[*i] / (double)all) * 100;
		skiko = skiko * (res / 100);
	}
	else if (did[*i] < res)
	{
		res = ((double)did[*i] / (double)all) * 100;
		skiko = skiko * (res / 100);
	}
	else 
		skiko =  skiko / 2;
	return (skiko);
}

void	print_rc(int all, int size, int *did, int y)
{
	int i;
	int skiko;
	int kol;
	int pos;

	i = 0;
	kol = 0;
	if (size == 1)
	{
		while (i < 4)
		{
			if (did[i] > 0)
				break;
			i++;
		}
		print_rc_end(50, i, 200, 16 + y);
	}
	else if (size == 2)
	{
		skiko = find_i(&i, did, 50, all);
		pos = skiko;
		print_rc_end(skiko, i, 200, 16 + y);
		i++;
		skiko = 50 - skiko;
		find_i(&i, did, 50, all);
		print_rc_end(skiko, i, 200 + pos, 16 + y);
	}
	else if (size == 3)
	{
		skiko = find_i(&i, did, 50, all);
		print_rc_end(skiko, i, 200, 16 + y);
		pos = skiko;
		i++;
		skiko = find_i(&i, did, 50, all);
		print_rc_end(skiko, i, 200 + pos, 16 + y);
		pos += skiko;
		i++;
		skiko = 50 - pos;
		find_i(&i, did, 50, all);
		print_rc_end(skiko, i, 200 + pos, 16 + y);
	}
	else if (size == 4)
	{
		skiko = find_i(&i, did, 50, all);
		print_rc_end(skiko, i, 200, 16 + y);
		pos = skiko;
		i++;
		skiko = find_i(&i, did, 50, all);
		print_rc_end(skiko, i, 200 + pos, 16 + y);
		pos += skiko;
		i++;
		skiko = find_i(&i, did, 50, all);
		print_rc_end(skiko, i, 200 + pos, 16 + y);
		pos += skiko;
		i++;
		skiko = 50 - pos;
		find_i(&i, did, 50, all);
		print_rc_end(skiko, i, 200 + pos, 16 + y);
	}
}

void	print_row(t_players *players, int point, int y)
{
	int did[4];
	int all;
	int i;

	i = 0;
	all = 0;
	attron(COLOR_PAIR(33) | A_BOLD);
	mvwprintw(stdscr, 16 + y, 199, "[--------------------------------------------------");
	attroff(COLOR_PAIR(33) | A_BOLD);
	i = 0;
	while (players[i].header.prog_name[0] != '\0')
	{
		if (point == 0)
		{
			did[i] = players[i].live;
			all = all + players[i].live;
		}
		else if (point == 1)
		{
			did[i] = players[i].last_live;
			all = all + players[i].last_live;
		}
		i++;
	}
	if ((print_rc_next(did)) > 0)
		print_rc(all, print_rc_next(did), did, y);
	attron(COLOR_PAIR(33) | A_BOLD);
	mvwprintw(stdscr, 16 + y, 250, "]");
	attroff(COLOR_PAIR(33) | A_BOLD);
}

void	lives_right_row(t_players *players, int DIE, int y)
{
	attron(COLOR_PAIR(8) | A_BOLD);
	mvwprintw(stdscr, 15 + y, 199, "Live breakdown for current period :");
	attroff(COLOR_PAIR(8) | A_BOLD);
	print_row(players, 0, y);
	attron(COLOR_PAIR(8) | A_BOLD);
	mvwprintw(stdscr, 18 + y, 199, "Live breakdown for last period :");
	attroff(COLOR_PAIR(8) | A_BOLD);
	print_row(players, 1, y + 3);
	attron(COLOR_PAIR(8) | A_BOLD);
	mvwprintw(stdscr, 21 + y, 199, "CYCLE_TO_DIE :        ");
	mvwprintw(stdscr, 21 + y, 199, "CYCLE_TO_DIE : %d", DIE);
	mvwprintw(stdscr, 23 + y, 199, "CYCLE_DELTA : 50");
	mvwprintw(stdscr, 25 + y, 199, "NBR_LIVE : 21");
	mvwprintw(stdscr, 27 + y, 199, "MAX_CHECKS : 10");
	attroff(COLOR_PAIR(8) | A_BOLD);
}

void 	players_right_bar(t_players *players)
{
	int i;
	int k;

	i = 0;
	k = 0;
	while (players[i].header.prog_name[0] != '\0')
	{
		attron(COLOR_PAIR(8) | A_BOLD);
		mvwprintw(stdscr, 11 + k, 199, "Player %d :", players[i].num);
		attroff(COLOR_PAIR(8) | A_BOLD);
		attron(COLOR_PAIR(i + 1) | A_BOLD);
		mvwprintw(stdscr, 11 + k, 234, "%s", players[i].header.prog_name);
		attroff(COLOR_PAIR(i + 1) | A_BOLD);
		attron(COLOR_PAIR(8) | A_BOLD);
		mvwprintw(stdscr, 12 + k, 201, "Last live :");
		mvwprintw(stdscr, 12 + k, 234, "%d", players[i].last_live);
		mvwprintw(stdscr, 13 + k, 201, "Lives in current period :");
		mvwprintw(stdscr, 13 + k, 234, "          ");
		mvwprintw(stdscr, 13 + k, 234, "%d", players[i].live);
		k += 4;
		i++;
	}
}

void	right_bar(t_flags *flags, t_players *players, t_players *stack)
{
	if (flags->paused == 1)
	{
		attron(COLOR_PAIR(6) | A_BOLD);
		mvwprintw(stdscr, 2, 199, "             ");
		mvwprintw(stdscr, 2, 199, "** PAUSED **");
		attroff(COLOR_PAIR(6) | A_BOLD);
	}
	else
	{
		attron(COLOR_PAIR(7) | A_BOLD);
		mvwprintw(stdscr, 2, 199, "             ");
		mvwprintw(stdscr, 2, 199, "** RUNNING **");
		attroff(COLOR_PAIR(7) | A_BOLD);
	}
	attron(COLOR_PAIR(8) | A_BOLD);
	mvwprintw(stdscr, 4, 199, "Speed cycles/second : x");
	mvwprintw(stdscr, 4, 222, "        ");
	mvwprintw(stdscr, 4, 222, "%d", flags->speed);
	mvwprintw(stdscr, 7, 199, "Cycle : ");
	mvwprintw(stdscr, 7, 207, "%d", *(players[0].cycles) - 1);
	flags->process = size_processes(&stack) + flags->amount_players;
	mvwprintw(stdscr, 9, 199, "Processes : ");
	mvwprintw(stdscr, 9, 211, "            ");
	mvwprintw(stdscr, 9, 211, "%d", flags->process);
	attroff(COLOR_PAIR(8) | A_BOLD);
}

void	include_colors(void)
{
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_BLUE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);

	init_pair(6, COLOR_RED, COLOR_BLACK);
	init_pair(7, COLOR_GREEN, COLOR_BLACK);
	init_pair(8, COLOR_WHITE, COLOR_BLACK);

	init_pair(10, COLOR_BLACK, COLOR_GREEN);
	init_pair(11, COLOR_BLACK, COLOR_BLUE);
	init_pair(12, COLOR_BLACK, COLOR_RED);
	init_pair(13, COLOR_BLACK,  COLOR_YELLOW);

	init_pair(33, 241, COLOR_BLACK);
	init_pair(34, 241, 241);
}

void	include_beginer_vis(t_players *players, t_flags *flags)
{
	initscr();
	nodelay(stdscr, true);
    keypad(stdscr, true);
    raw();
    curs_set(0);
    start_color();
    include_colors();
    players_vis_area(players, flags);
}

void	print_new_map(byte *map, byte *map_color)
{
	int i;
	int y;
	int x;
	char *tmp;

	i = 0;
	y = 2;
	x = 3;
	while (i < MEM_SIZE)
	{
		if (x > 192)
		{
			x = 3;
			y++;
		}
		tmp = get_hex(map[i]);
		if (map_color[i] == 1)
		{
			attron(COLOR_PAIR(1));
			mvwprintw(stdscr, y, x, "%s ", tmp);
			attroff(COLOR_PAIR(1));
		}
		else if (map_color[i] == 2)
		{
			attron(COLOR_PAIR(2));
			mvwprintw(stdscr, y, x, "%s ", tmp);
			attroff(COLOR_PAIR(2));
		}
		else if (map_color[i] == 3)
		{
			attron(COLOR_PAIR(3));
			mvwprintw(stdscr, y, x, "%s ", tmp);
			attroff(COLOR_PAIR(3));
		}
		else if (map_color[i] == 4)
		{
			attron(COLOR_PAIR(4));
			mvwprintw(stdscr, y, x, "%s ", tmp);
			attroff(COLOR_PAIR(4));
		}
		else
		{
			attron(COLOR_PAIR(33) | A_BOLD);
			mvwprintw(stdscr, y, x, "%s ", tmp);
			attroff(COLOR_PAIR(33) | A_BOLD);
		}
		free(tmp);
		x += 3;
		i++;
	}

}

byte	*get_map_color(t_players *players, int count, int *cycles)
{
	unsigned char	*map;
	int				i;
	int				j;
	char			*ptr;

	j = 1;
	i = 0;
	map = (unsigned char*)malloc(sizeof(unsigned char) * MEM_SIZE);
	ptr = players[0].comands;
	players[0].cycles = cycles;
	while (i < MEM_SIZE && j <= count)
	{
		if (i >= (MEM_SIZE / count) * j)
		{
			ptr = players[j].comands;
			players[j].cycles = cycles;
			j++;
		}
		if (ptr != NULL && *ptr != '\0')
		{
			map[i] = j;
			ptr += 2;
		}
		else
			map[i] = 0;
		i++;
	}
	return (map);
}

void	get_color(t_players *players, t_players **stack, t_flags *flags)
{
	int i;
	int k;
	t_players *tmp;

	i = 0;
	tmp = *stack;
	while (players[i].header.prog_name[0] != '\0')
	{
		if (players[i].st > -1)
		{
			k = 0;
			while (k < 4)
			{
				flags->map_color[players[i].st] = i + 1;
				players[i].st++;
				if (players[i].st == 4095)
					players[i].st = 0;
				k++;
			}
			players[i].st = -1;
		}
		i++;
	}
	while (tmp != NULL)
	{
		if (tmp->st > -1)
		{
			k = 0;
			while (k < 4)
			{
				flags->map_color[tmp->st] = tmp->number;
				tmp->st++;
				if (tmp->st == 4095)
					tmp->st = 0;
				k++;
			}
			tmp->st = -1;
		}
		tmp = tmp->next;
	}
}

void	refresh_cursor(t_players *players, t_flags *flags, t_players *stack, byte *map)
{
	t_players	*tmp;
	int 		i;

	i = 0;
	tmp = stack;
	while (players[i].header.prog_name[0] != '\0')
	{
		if (flags->map_color[players[i].pos] == 0)
			attron(COLOR_PAIR(34));
		else
			attron(COLOR_PAIR(flags->map_color[players[i].pos] + 9));
		mvwprintw(stdscr, (players[i].pos / 64) + 2, (players[i].pos % 64) * 3 + 3, "%02x", map[players[i].pos]);
		if (flags->map_color[players[i].pos] == 0)
			attroff(COLOR_PAIR(34));
		else
			attroff(COLOR_PAIR(flags->map_color[players[i].pos] + 9));
		mvwprintw(stdscr, (players[i].pos / 64) + 2, (players[i].pos % 64) * 3 + 5, " ");
		i++;
	}
	while (tmp)
	{
		if (flags->map_color[tmp->pos] == 0)
			attron(COLOR_PAIR(34));
		else
			attron(COLOR_PAIR(flags->map_color[tmp->pos] + 9));
		mvwprintw(stdscr, (tmp->pos / 64) + 2, (tmp->pos % 64) * 3 + 3, "%02x", map[tmp->pos]);
		if (flags->map_color[tmp->pos] == 0)
			attroff(COLOR_PAIR(34));
		else
			attroff(COLOR_PAIR(flags->map_color[tmp->pos] + 9));
		mvwprintw(stdscr, (tmp->pos / 64) + 2, (tmp->pos % 64) * 3 + 5, " ");
		tmp = tmp->next;
	}
}

void	include_refresh_vis(t_players *players, t_flags *flags, t_players *stack, byte *map)
{
	// refresh();
	noecho();
	print_board();
	print_new_map(map, flags->map_color);
	right_bar(flags, players, stack);
	players_right_bar(players);
	if (flags->amount_players == 1)
		lives_right_row(players, flags->DIE, 0);
	else if (flags->amount_players == 2)
		lives_right_row(players, flags->DIE, 4);
	else if (flags->amount_players == 3)
		lives_right_row(players, flags->DIE, 8);
	else if (flags->amount_players == 4)
		lives_right_row(players, flags->DIE, 12);
	board_kay(flags);
}

void	go_vm_vis(t_players *players, int count, t_flags *flags)
{
	byte		*map;
	t_players	*stack;
	t_players	*tmp;
	int			i;

	i = 0;
	flags->cycles = 1;
	flags->cycles_test = 1;
	copy = NULL;
	stack = NULL;
	flags->DIE = CYCLE_TO_DIE;
	flags->max_checks = 0;
	map = get_map(players, count, &(flags->cycles));
	flags->map_color = get_map_color(players, count, &(flags->cycles));
	include_beginer_vis(players, flags);
	refresh_cursor(players, flags, stack, map);
	refresh();
	while (players[i].header.prog_name[0] != '\0')
		get_stop(&(players[i++]), map);
	while (flags->DIE > 0)
	{
		if (flags->paused == 0)
		{
			if (flags->dump == flags->cycles)
			print_map(map);
			tmp = stack;
			while (tmp != NULL)
			{
				get_command(tmp, map, &stack, players);
				tmp = tmp->next;
			}
			i = count - 1;
			while (i >= 0)
			{
				if (players[i].comands != NULL)
					get_command(&(players[i]), map, &stack, players);
				i--;
			}
			get_color(players, &stack, flags);
			if (flags->cycles_test >= flags->DIE)
			{
				i = 0;
				if (get_lives(players) >= NBR_LIVE || flags->max_checks == 10)
				{
					flags->DIE -= CYCLE_DELTA;
					flags->max_checks = 0;
				}
				else
					flags->max_checks++;
				check_end(players, map, &stack, flags);
				flags->cycles_test = 0;
			}
			include_refresh_vis(players, flags, stack, map);
			refresh_cursor(players, flags, stack, map);
			refresh();
			// cursor_refresh(win1, win2, players, map);
			// cursor_refresh_stack(win1, win2, stack, map);
			usleep(100000 / flags->speed);
			flags->cycles++;
			flags->cycles_test++;
		}
		else
		{
			if (getch() == 32)
				flags->paused = 0;
			// include_refresh_vis(players, flags, stack, map);
			// refresh_cursor(players, flags, stack, map);
			// refresh();
		}
	}
	end_game(players, map, &stack, flags);
}

void	start_vm(t_players **tmp, int count, t_flags *flags)
{
	t_players	players[count + 1];
	int 		i;
	t_players	*tmp1;

	i = 0;
	while (*tmp)
	{
		players[i] = **tmp;
		players[i].num *= -1;
		players[i].pos = 0 + (i * (MEM_SIZE / count));
		players[i].reg[0] = players[i].num;
		players[i].last_herro = &(flags->base);
		tmp1 = (*tmp)->next;
		free(*tmp);
		tmp = &tmp1;
		i++;
	}
	players[count].header.prog_name[0] = '\0';
	players[count].comands = NULL;
	tmp = NULL;
	i = 0;
	while (players[i].header.prog_name[0] != '\0')
	{
		players[i].number = i + 1;
		i++;
	}
	if (flags->visual == 1)
		go_vm_vis(players, count, flags);
	else
		go_vm(players, count, flags);
}
