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

int 		translate(byte r1, byte r2, byte r3, byte r4)
{
	return (r1 * 256 * 256 * 256 + r2 * 256 * 256 + r3 * 256 + r4);
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
	else
	{
		free(res);
		return (tmp);
	}
	free(tmp);
	return (res);
}

int		get_REG(t_players *player, int posit, byte *map)
{
	byte	n;

	n = map[posit % MEM_SIZE];
	if (n > REG_NUMBER || n < 1)
		return (0);
	return ((int)player->reg[n - 1]);
}

int			get_TDIR(int n, int posit, byte *map)
{
	int		res;

	res = 0;
	if (n == 2)
		res = (short int)translate(0, 0, map[posit % MEM_SIZE], map[(posit + 1) % MEM_SIZE]);
	else if (n == 4)
		res = translate(map[posit % MEM_SIZE], map[(posit + 1) % MEM_SIZE],
				map[(posit + 2) % MEM_SIZE], map[(posit + 3) % MEM_SIZE]);
	return (res);
}

short int	get_INDIR(t_players *player, char flag, int posit, byte *map)
{
	short int	res;
	int			arg;
	int			pos;

	res = 0;
	arg = get_TDIR(2, posit, map);
	if (player->pos + arg < 0)
		pos = MEM_SIZE + (player->pos + arg);
	else
		pos = (player->pos + (arg % IDX_MOD)) % MEM_SIZE;
	if (flag == 'r')
		res = (short int)translate(0, 0, map[pos % MEM_SIZE], map[(pos + 1) % MEM_SIZE]);
	else if (flag == 'd')
		res = arg;
	return (res);
}

void	zjmp(t_players *player, byte *map)
{
	short int	jmp;

	jmp = (short int)get_TDIR(2, player->pos + 1, map) % IDX_MOD;
	if (player->carry == 1)
	{
		if (jmp + player->pos < 0)
			player->pos = MEM_SIZE + (player->pos + jmp);
		else
			player->pos = (player->pos + jmp) % MEM_SIZE;
	}
	else
		player->pos += 3;
}

void	live(t_players *players, byte *map, t_players *player)
{
	int		i;
	int		reg;

	i = 0;
	reg = get_TDIR(4, player->pos + 1, map);
	while (players[i].header.prog_name[0] != '\0')
	{
		if (reg == players[i].num)
		{
			players[i].live += 1;
			players[i].last_live = *(players[i].cycles);
			*(players[i].last_herro) = player->num;
			if (ft_strnstr(player->header.prog_name, "fork", 4))
				player->live_amount += 1;
			player->pos += 5;
			return ;
		}
		i++;
	}
	player->live_amount += 1;
	player->last_live = *(player->cycles);
	*(player->last_herro) = player->num;
	(*player).pos += 5;
}
