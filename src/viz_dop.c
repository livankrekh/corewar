/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   viz_dop.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/05 16:05:11 by liabanzh          #+#    #+#             */
/*   Updated: 2017/08/05 16:05:13 by liabanzh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"
#include "../includes/op.h"

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

void	refresh_map(WINDOW *win, byte *map)
{
	int 	i;
	int 	x;
	int 	y;
	char	*tmp;
	int 	max_X;

	i = 0;
	y = 1;
	x = 2;

	max_X = 0;
	getmaxyx(win, y, max_X);
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	wmove(win, 1, 2);
	while (i < MEM_SIZE)
	{
		getyx(win, y, x);
		if (x + 4 >= max_X)
			wmove(win, ++y, 2);
		if (map[i] != 0)
		{
			tmp = get_hex(map[i]);
			wattron(win, COLOR_PAIR(1));
			wprintw(win, "%s", tmp);
			wattroff(win, COLOR_PAIR(1));
			wprintw(win, " ");
			free(tmp);
		}
		else
			wprintw(win, "00 ");
		i++;
	}
}

void	cursor_refresh(WINDOW *win1, WINDOW *win2, t_players *players, byte *map)
{
	int 	x;
	int 	y;
	int 	max;
	int 	player;
	char	*tmp;

	y = 0;
	x = 0;
	player = 1;
	getmaxyx(win1, y, x);
	max = (x - 5) / 3;
	init_pair(5, COLOR_WHITE, COLOR_BLUE);
	init_pair(6, COLOR_WHITE, COLOR_GREEN);
	init_pair(7, COLOR_WHITE, COLOR_RED);
	init_pair(8, COLOR_WHITE, COLOR_YELLOW);
	y = 4;
	while (players[player - 1].header.name != NULL)
	{
		wmove(win1, players[player - 1].pos / max + 1, (players[player - 1].pos % max * 3) + 2);
		tmp = get_hex(map[players[player - 1].pos]);
		wattron(win1, COLOR_PAIR(player + 4) | A_BOLD);
		wprintw(win1, "%s", tmp);
		wattroff(win1, COLOR_PAIR(player + 4) | A_BOLD);
		wmove(win2, y, 23);
		wprintw(win2, "%d", players[player - 1].live);
		y += 4;
		player++;
		free(tmp);
	}
	wrefresh(win1);
	wrefresh(win2);
	tmp = NULL;
}
