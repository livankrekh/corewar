/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funcs.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/19 14:45:43 by liabanzh          #+#    #+#             */
/*   Updated: 2017/07/19 14:45:46 by liabanzh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"
#include "../includes/op.h"
#include <ncurses.h>

void	zjmp(t_players *player, byte *map)
{
	int		jmp;

	jmp = map[(*player).pos + 1] * 256 + map[(*player).pos + 2];
	if (jmp > 4100)
		jmp = jmp - 0xFFFF;
	(*player).pos += jmp;
}

void	live(t_players *players, byte map, int count)
{
	int		i;

	i = 0;
	while (i < count)
	{
		if (map == players[i].num)
			players[i].live++;
		i++;
	}
}

void	sti(t_players *player, byte *map, unsigned int *reg)
{
	char				*binary;
	int					r1;
	int					r2;
	unsigned int		res;

	binary = ft_itoa_base(map[(*player).pos + 1], 2);
	binary += 2;
	res = reg[(map[(*player).pos + 2] > 17) ? 0 : map[(*player).pos + 2]];
	if (ft_strnstr(binary, "10", 2))
		r1 = map[(*player).pos + 3] * 256 + map[(*player).pos + 4];
	else
		r1 = map[(*player).pos + (map[(*player).pos + 3] * 256 + map[(*player).pos + 4])] * 256
			+ map[(*player).pos + (map[(*player).pos + 3] * 256 + map[(*player).pos + 4]) + 1];
	binary += 2;
	if (ft_strnstr(binary, "10", 2))
		r2 = map[(*player).pos + 5] * 256 + map[(*player).pos + 6];
	else
		r2 = map[(*player).pos + (map[(*player).pos + 5] * 256 + map[(*player).pos + 6])] * 256
			+ map[(*player).pos + (map[(*player).pos + 5] * 256 + map[(*player).pos + 6]) + 1];
	(*player).pos += r1 + r2;
	map[(*player).pos + 1] = res / (256 * 256 * 256);
	map[(*player).pos + 2] = res / (256 * 256);
	map[(*player).pos + 3] = res / 256;
	map[(*player).pos + 4] = res % (256);
	binary -= 4;
	free(binary);
}
