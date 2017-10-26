/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   funcs_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/21 20:22:20 by liabanzh          #+#    #+#             */
/*   Updated: 2017/10/21 20:22:21 by liabanzh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"
#include "../includes/op.h"

void	sub(t_players *player, byte *map)
{
	int		r1;
	int		r2;

	if (check_oppcode(player, map) == 0)
		return ;
	r1 = get_REG(player, player->pos + 2, map);
	r2 = get_REG(player, player->pos + 3, map);
	player->reg[map[(player->pos + 4) % MEM_SIZE] - 1] = r1 - r2;
	player->pos += 5;
	if (r1 - r2 == 0)
		player->carry = 1;
	else
		player->carry = 0;
}

void	add(t_players *player, byte *map)
{
	int		r1;
	int		r2;

	if (check_oppcode(player, map) == 0)
		return ;
	r1 = get_REG(player, player->pos + 2, map);
	r2 = get_REG(player, player->pos + 3, map);
	player->reg[map[(player->pos + 4) % MEM_SIZE] - 1] = r1 + r2;
	player->pos += 5;
	if (r1 + r2 == 0)
		player->carry = 1;
	else
		player->carry = 0;
}

void	and_xor(t_players *player, byte *map, char flag)
{
	int		r1;
	int		r2;
	int		posit;
	char	*opp;

	posit = 1;
	r1 = 0;
	r2 = 0;
	if (check_oppcode(player, map) == 0)
		return ;
	opp = get_binary(map, player);
	if (ft_strnstr(opp, "01", 2))
	{
		r1 = get_REG(player, player->pos + posit + 1, map);
		posit++;
	}
	else if (ft_strnstr(opp, "10", 2))
	{
		r1 = get_TDIR(4, player->pos + posit + 1, map);
		posit += 4;
	}
	else if (ft_strnstr(opp, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		posit += 2;
	}
	opp += 2;
	if (ft_strnstr(opp, "01", 2))
	{
		r2 = get_REG(player, player->pos + posit + 1, map);
		posit++;
	}
	else if (ft_strnstr(opp, "10", 2))
	{
		r2 = get_TDIR(4, player->pos + posit + 1, map);
		posit += 4;
	}
	else if (ft_strnstr(opp, "11", 2))
	{
		r2 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		posit += 2;
	}
	opp -= 2;
	free(opp);
	if ((unsigned char)(map[((*player).pos + posit + 1) % MEM_SIZE] - 1) > REG_NUMBER - 1)
	{
		player->pos += posit + 2;
		return ;
	}
	if (flag == 'a')
		player->reg[map[((*player).pos + posit + 1) % MEM_SIZE] - 1] = r1 & r2;
	else if (flag == 'o')
		player->reg[map[((*player).pos + posit + 1) % MEM_SIZE] - 1] = r1 | r2;
	else if (flag == 'x')
		player->reg[map[((*player).pos + posit + 1) % MEM_SIZE] - 1] = r1 ^ r2;
	player->pos += posit + 2;
	if (flag == 'a' && (r1 & r2) == 0)
		player->carry = 1;
	else if (flag == 'o' && (r1 | r2) == 0)
		player->carry = 1;
	else if (flag == 'x' && (r1 ^ r2) == 0)
		player->carry = 1;
	else
		player->carry = 0;
}
