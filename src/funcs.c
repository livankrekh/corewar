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

void	live(t_players *players, byte map)
{
	int		i;

	i = 0;
	while (players[i].header.name != NULL)
	{
		if (map == players[i].num)
			players[i].live++;
		i++;
	}
}

char	*get_binary(byte *map, t_players *player)
{
	char	*tmp;
	char	*res;
	int		i;

	res = ft_strnew(8);
	tmp = ft_itoa_base(map[(*player).pos + 1], 2);
	if (ft_strlen(tmp) < 8)
	{
		i = ft_strlen(tmp);
		while (i)
		{
			res[7 - (ft_strlen(tmp) - i)] = tmp[i - 1];
			i--;
		}
		while (i < 8 - (int)ft_strlen(tmp))
			res[7 - (ft_strlen(tmp) + i++)] = '0';
	}
	return (res);
}

void	sti(t_players *player, byte *map, unsigned int *reg)
{
	char				*binary;
	int					r1;
	int					r2;
	unsigned int		res;

	binary = get_binary(map, player);
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
