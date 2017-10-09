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

void	get_players_viz(WINDOW *win, byte *memory, t_players *players)
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
	max_X = 196;
	// getmaxyx(win, y, max_X);
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

void	status_bar(WINDOW **win2, t_players *players)
{
	int 	y;
	int 	i;

	i = 0;
	y = 2;
	*win2 = newwin(69, 57, 0, 198);
	box(*win2, 0, 0);
	while (players[i].header.prog_name[0] != '\0')
	{
		wmove(*win2, y, 2);
		wprintw(*win2, "Player: ");
		wattron(*win2, COLOR_PAIR(i + 1) | A_BOLD);
		wprintw(*win2, "%s", players[i].header.prog_name);
		wattroff(*win2, COLOR_PAIR(i + 1) | A_BOLD);
		wmove(*win2, ++y, 2);
		wprintw(*win2, "Player number: %d", players[i].num);
		wmove(*win2, ++y, 2);
		wprintw(*win2, "Player live counter: %d", players[i].live);
		wmove(*win2, ++y, 2);
		wprintw(*win2, "Player live amount: %d", players[i].live_amount);
		wmove(*win2, ++y, 2);
		wprintw(*win2, "Player last live: %d", players[i].last_live);
		y += 2;
		i++;
	}
	wmove(*win2, y + 2, 2);
	wprintw(*win2, "CYCLES: %d", *(players[0].cycles));
	wrefresh(*win2);
}

void	vizualize(byte *memory, t_players *players, WINDOW **win1, WINDOW *win)
{
	// int 	max_X;
	// int 	max_Y;

	// getmaxyx(win, max_Y, max_X);
	*win1 = newwin(69, 197, 0, 0);
	start_color();
	box(*win1, 0, 0);
	//wbkgd(win, COLOR_PAIR(2));
	wrefresh(win);
	wrefresh(*win1);
	get_players_viz(*win1, memory, players);
}
