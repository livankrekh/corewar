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

void	end_game(t_players *players, byte *map, t_players **stack)
{
	int 		i;
	t_players	*tmp;
	t_players	*tmp2;

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
			if (tmp2 == NULL)
				*stack = tmp->next;
			else
				tmp2->next = tmp->next;
			free(tmp->reg);
			tmp->reg = NULL;
			free(tmp);
			if (tmp2 == NULL)
				tmp = *stack;
			// else if (tmp2 == NULL)
			// 	return ;
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

void	check_end(t_players *players, byte *map, t_players **stack)
{
	int 		i;

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
		end_game(players, map, stack);
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
	// WINDOW		*win;
	// WINDOW		*win1;
	// WINDOW		*win2;

	i = 0;
	flags->cycles = 1;
	flags->cycles_test = 1;
	copy = NULL;
	stack = NULL;
	flags->DIE = CYCLE_TO_DIE;
	flags->max_checks = 0;
	map = get_map(players, count, &(flags->cycles));
	// win1 = NULL;
	// win2 = NULL;
	// win = initscr();
	// vizualize(map, players, &win1, win);
	// status_bar(&win2, players);
	// curs_set(0);
	// getch();
	// cursor_refresh(win1, win2, players, map);
	// getch();
	while (players[i].header.prog_name[0] != '\0')
		get_stop(&(players[i++]), map);
	while (flags->DIE > 0)
	{
		// if (flags->cycles >= 3080 && flags->cycles <= 3300)
		// 	getch();
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
			check_end(players, map, &stack);
			flags->cycles_test = 0;
		}
		// copy = stack;
		// refresh_map(win1, map);
		// cursor_refresh(win1, win2, players, map);
		// status_bar(&win2, players);
		// cursor_refresh_stack(win1, win2, stack, map);
		// wrefresh(win1);
		// usleep(1000);
		flags->cycles++;
		flags->cycles_test++;
	}
	end_game(players, map, &stack);
	// delwin(win1);
	// delwin(win2);
	// delwin(win);
	// endwin();
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
	go_vm(players, count, flags);
}
