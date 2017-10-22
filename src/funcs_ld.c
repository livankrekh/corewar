/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funcs_ld.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/21 20:17:07 by liabanzh          #+#    #+#             */
/*   Updated: 2017/10/21 20:17:08 by liabanzh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"
#include "../includes/op.h"

void	ld(t_players *player, byte *map)
{
	char	*binary;
	int		r1;
	int		posit;

	r1 = 0;
	posit = 1;
	binary = get_binary(map, player);
	if (ft_strnstr(binary, "10", 2))
	{
		r1 = get_TDIR(4, player->pos + posit + 1, map);
		posit += 4;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		posit += 2;
	}
	free(binary);
	if (map[(player->pos + posit + 1) % MEM_SIZE] < 1 || map[(player->pos + posit + 1) % MEM_SIZE] > REG_NUMBER)
	{
		player->pos += posit + 2;
		return ;
	}
	player->reg[map[(player->pos + posit + 1) % MEM_SIZE] - 1] = r1;
	player->pos += posit + 2;
	if (r1 == 0)
		player->carry = 1;
	else
		player->carry = 0;
}

void	ldi(t_players *player, byte *map)
{
	int		r1;
	int		r2;
	int		posit;
	char	*binary;

	r1 = 0;
	r2 = 0;
	posit = 1;
	binary = get_binary(map, player);
	if (ft_strnstr(binary, "01", 2))
	{
		r1 = get_REG(player, player->pos + posit + 1, map);
		posit++;
	}
	else if (ft_strnstr(binary, "10", 2))
	{
		r1 = (short int)get_TDIR(2, player->pos + posit + 1, map);
		posit += 2;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		posit += 2;
	}
	binary += 2;
	if (ft_strnstr(binary, "10", 2))
	{
		r2 = get_TDIR(2, player->pos + posit + 1, map);
		posit += 2;
	}
	else if (ft_strnstr(binary, "01", 2))
	{
		r2 = get_REG(player, player->pos + posit + 1, map);
		posit++;
	}
	binary -= 2;
	free(binary);
	if (map[(player->pos + posit + 1) % MEM_SIZE] < 1 || map[(player->pos + posit + 1) % MEM_SIZE] > REG_NUMBER)
	{
		player->pos += ++posit + 1;
		return ;
	}
	if (player->pos + r1 + r2 < 0)
		player->reg[map[player->pos + posit + 1] - 1] = get_TDIR(4, MEM_SIZE + player->pos + ((r1 + r2) % IDX_MOD), map);
	else
		player->reg[map[player->pos + posit + 1] - 1] = get_TDIR(4, player->pos + ((r1 + r2) % IDX_MOD), map);
	player->pos += posit + 2;
}

void	lldi(t_players *player, byte *map)
{
	int		r1;
	int		r2;
	int		posit;
	char	*binary;

	r1 = 0;
	r2 = 0;
	posit = 1;
	binary = get_binary(map, player);
	if (ft_strnstr(binary, "01", 2))
	{
		r1 = get_REG(player, player->pos + posit + 1, map);
		posit++;
	}
	else if (ft_strnstr(binary, "10", 2))
	{
		r1 = get_TDIR(2, player->pos + posit + 1, map);
		posit += 2;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		posit += 2;
	}
	binary += 2;
	if (ft_strnstr(binary, "10", 2))
	{
		r2 = get_TDIR(2, player->pos + posit + 1, map);
		posit += 2;
	}
	else if (ft_strnstr(binary, "01", 2))
	{
		r2 = get_REG(player, player->pos + posit + 1, map);
		posit++;
	}
	binary -= 2;
	free(binary);
	if (map[(player->pos + posit + 1) % MEM_SIZE] < 1 || map[(player->pos + posit + 1) % MEM_SIZE] > REG_NUMBER)
	{
		player->pos += posit + 2;
		return ;
	}
	player->reg[map[player->pos + posit + 1] - 1] = get_TDIR(4, (player->pos + r1 + r2) % MEM_SIZE, map);
	player->pos += posit + 2;
	if (get_TDIR(4, (player->pos + r1 + r2) % MEM_SIZE, map) == 0)
		player->carry = 1;
	else
		player->carry = 0;
}

void	lld(t_players *player, byte *map)
{
	char	*binary;
	int		r1;
	int		posit;

	r1 = 0;
	posit = 1;
	binary = get_binary(map, player);
	if (ft_strnstr(binary, "10", 2))
	{
		r1 = get_TDIR(4, player->pos + posit + 1, map);
		posit += 4;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		posit += 2;
	}
	free(binary);
	if (map[(player->pos + posit + 1) % MEM_SIZE] < 1 || map[(player->pos + posit + 1) % MEM_SIZE] > REG_NUMBER)
	{
		player->pos += ++posit + 1;	
		return ;
	}
	player->reg[map[player->pos + posit + 1] - 1] = r1;
	player->pos += ++posit + 1;
	if (r1 == 0)
		player->carry = 1;
	else
		player->carry = 0;
}
