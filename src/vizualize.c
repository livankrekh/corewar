/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vizualize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/08/03 19:12:45 by liabanzh          #+#    #+#             */
/*   Updated: 2017/08/03 19:12:47 by liabanzh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"
#include "../includes/op.h"

void	get_players(WINDOW *win, byte *memory, t_players *players)
{
	int 	i;
	int 	player;
	char 	*tmp2;
	char 	*tmp;
	int 	y;
	int 	x;
	int 	max_X;

	i = 0;
	y = 1;
	x = 2;

	player = 1;
	max_X = 0;
	getmaxyx(win, y, max_X);
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_BLACK);
	init_pair(4, COLOR_YELLOW, COLOR_BLACK);
	wmove(win, 1, 2);
	while (i < MEM_SIZE)
	{
		if (i == 0 || memory[i] != 0)
		{
			tmp = players[player - 1].comands;
			players[player - 1].pos = i;
			tmp2 = ft_strnew(2);
			while (*tmp != '\0')
			{
				getyx(win, y, x);
				if (x + 4 >= max_X)
					wmove(win, ++y, 2);
				ft_strncpy(tmp2, tmp, 2);
				wattron(win, COLOR_PAIR(player));
				wprintw(win, "%s", tmp2);
				wattroff(win, COLOR_PAIR(player));
				wprintw(win, " ");
				tmp += 2;
				i++;
			}
			free(tmp2);
			player++;
		}
		else
		{
			getyx(win, y, x);
			if (x + 4 >= max_X)
				wmove(win, ++y, 2);
			wprintw(win, "00 ");
			i++;
		}
	}
	wrefresh(win);
}

void	status_bar(WINDOW **win2, WINDOW *win, t_players *players)
{
	int 	max_X;
	int 	max_Y;

	getmaxyx(win, max_Y, max_X);
	*win2 = newwin(max_Y, max_X * 0.2, 0, max_X * 0.8 + 1);
	box(*win2, 0, 0);
	wrefresh(*win2);
	players = NULL;
}

void	vizualize(byte *memory, t_players *players, WINDOW **win1, WINDOW *win)
{
	int 	max_X;
	int 	max_Y;

	getmaxyx(win, max_Y, max_X);
	*win1 = newwin(max_Y, max_X * 0.8, 0, 0);
	start_color();
	box(*win1, 0, 0);
	//wbkgd(win, COLOR_PAIR(2));
	wrefresh(win);
	wrefresh(*win1);
	get_players(*win1, memory, players);
}
