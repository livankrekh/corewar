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

int 	translate(byte r1, byte r2, byte r3, byte r4)
{
	return (r1 * 256 * 256 * 256 + r2 * 256 * 256 + r3 * 256 + r4);
}

void	aff(t_players *player, byte *map)
{
	int		reg;

	reg = player->reg[map[player->pos + 2]] % 256;
	ft_putstr("Player #");
	ft_putnbr(player->num);
	ft_putstr(" says: ");
	ft_putchar(reg);
	ft_putchar('\n');
	player->pos += 2;
}

void	fork_func(t_players *player, byte *map, t_players **stack)
{
	int			dir;
	t_players	*tmp;

	dir = translate(0, 0, map[player->pos + 1], map[player->pos + 2]) % IDX_MOD;
	if (*stack == NULL)
	{
		*stack = (t_players*)malloc(sizeof(t_players));
		tmp = *stack;
	}
	else
	{
		tmp = *stack;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = (t_players*)malloc(sizeof(t_players));
		tmp = tmp->next;
	}
	tmp->header.name = "fork";
	tmp->comands = NULL;
	tmp->reg = player->reg;
	if (ft_strnstr(player->header.name, "fork", 4))
		tmp->live_ptr = player->live_ptr;
	else
		tmp->live_ptr = &(player->live);
	tmp->next = NULL;
	tmp->pos = player->pos + dir;
	tmp->stop = 0;
	tmp->carry = player->carry;
	player->pos += 3;
}

void	lfork_func(t_players *player, byte *map, t_players **stack)
{
	int			dir;
	t_players	*tmp;

	dir = translate(0, 0, map[player->pos + 1], map[player->pos + 2]);
	if (*stack == NULL)
	{
		*stack = (t_players*)malloc(sizeof(t_players));
		tmp = *stack;
	}
	else
	{
		tmp = *stack;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = (t_players*)malloc(sizeof(t_players));
		tmp = tmp->next;
	}
	tmp->header.name = "fork";
	tmp->comands = NULL;
	tmp->reg = player->reg;
	tmp->live_ptr = &(player->live);
	tmp->next = NULL;
	tmp->pos = player->pos + dir;
	tmp->stop = 0;
	tmp->carry = player->carry;
	tmp->num = 0;
	player->pos += 3;
}

void	and_xor(t_players *player, byte *map, char flag)
{
	int r1;
	int r2;
	int posit;
	char *opp;

	posit = 1;
	r1 = 0;
	r2 = 0;
	opp = get_binary(map, player);
	if (ft_strnstr(opp, "01", 2))
		r1 = (*player).reg[map[(*player).pos + posit++ + 1] - 1];
	else if (ft_strnstr(opp, "10", 2))
	{
		r1 = translate(map[(*player).pos + 2], map[(*player).pos + 3], map[(*player).pos + 4], map[(*player).pos + 5]);
		posit += 4;
	}
	else if (ft_strnstr(opp, "11", 2))
	{
		r1 = translate(map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 1], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 2], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 3]);
		posit += 2;
	}
	opp += 2;
	if (ft_strnstr(opp, "01", 2))
		r2 = (*player).reg[map[(*player).pos + posit++ + 1] - 1];
	else if (ft_strnstr(opp, "10", 2))
	{
		r2 = translate(map[(*player).pos + posit + 1], map[(*player).pos + posit + 2], map[(*player).pos + posit + 3], map[(*player).pos + posit + 4]);
		posit += 4;
	}
	else if (ft_strnstr(opp, "11", 2))
	{
		r2 = translate(map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 1], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 2], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 3]);
		posit += 2;
	}
	opp -= 2;
	free(opp);
	if (flag == 'a')
		(*player).reg[map[(*player).pos + posit + 1] - 1] = r1 & r2;
	else if (flag == 'o')
		(*player).reg[map[(*player).pos + posit + 1] - 1] = r1 | r2;
	else if (flag == 'x')
		(*player).reg[map[(*player).pos + posit + 1] - 1] = r1 ^ r2;
	(*player).pos += posit + 2;
	if (flag == 'a' && (r1 & r2) == 0)
		(*player).carry = 1;
	else if (flag == 'o' && (r1 | r2) == 0)
		(*player).carry = 1;
	else if (flag == 'x' && (r1 ^ r2) == 0)
		(*player).carry = 1;
}

void	zjmp(t_players *player, byte *map)
{
	int		jmp;

	jmp = translate(0, 0, map[(*player).pos + 1], map[(*player).pos + 2]);
	if (jmp > 0x7FFF)
		jmp = jmp - 0xFFFF - 1;
	else
		jmp %= MEM_SIZE;
	if ((*player).carry == 1)
		(*player).pos += jmp;
	else
		(*player).pos += 5;
}

void	live(t_players *players, byte *map, t_players *player)
{
	int		i;

	i = 0;
	while (players[i].header.name != NULL)
	{
		if ((unsigned int)translate(*map, *(map + 1), *(map + 2), *(map + 3)) == (unsigned int)(players[i].num * -1))
		{
			players[i].live_amount += 1;
			break ;
		}
		i++;
	}
	if (players[i].header.name == NULL && ft_strnstr(player->header.name, "fork", 4))
		*(player->live_ptr) += 1;
	else if (players[i].header.name == NULL)
		(*player).live += 1;
	(*player).pos += 5;
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

void	sti(t_players *player, byte *map)
{
	char	*binary;
	int		r1;
	int		r2;
	int		posit;

	binary = get_binary(map, player);
	binary += 2;
	posit = 2;
	r1 = 0;
	r2 = 0;
	if (ft_strnstr(binary, "01", 2))
		r1 = (*player).reg[map[(*player).pos + posit++ + 1] - 1];
	else if (ft_strnstr(binary, "10", 2))
	{
		r1 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = translate(0, 0, map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])] + 1);
		posit += 2;
	}
	binary += 2;
	if (ft_strnstr(binary, "01", 2))
		r2 = (*player).reg[map[(*player).pos + posit++ + 1] - 1];
	else if (ft_strnstr(binary, "10", 2))
	{
		r2 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	binary -= 4;
	free(binary);
	map[(*player).pos + r1 + r2] = (*player).reg[map[(*player).pos + 2] - 1] / 0x1000000;
	map[(*player).pos + r1 + r2 + 1] = (*player).reg[map[(*player).pos + 2] - 1] / 0x10000;
	map[(*player).pos + r1 + r2 + 2] = (*player).reg[map[(*player).pos + 2] - 1] / 0x100;
	map[(*player).pos + r1 + r2 + 3] = (*player).reg[map[(*player).pos + 2] - 1] % 0x100;
	(*player).pos += posit + 1;
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
		r1 = (*player).reg[map[(*player).pos + posit++ + 1]];
	else if (ft_strnstr(binary, "10", 2))
	{
		r1 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = translate(0, 0, map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])] + 1);
		posit += 2;
	}
	binary += 2;
	if (ft_strnstr(binary, "10", 2))
	{
		r2 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "01", 2))
		r2 = (*player).reg[map[(*player).pos + posit++ + 1]];
	binary -= 2;
	free(binary);
	(*player).reg[map[(*player).pos + posit++ + 1] - 1] = translate(map[(*player).pos + r1 + r2 % IDX_MOD], map[(*player).pos + r1 + r2 + 1 % IDX_MOD], map[(*player).pos + r1 + r2 + 2 % IDX_MOD], map[(*player).pos + r1 + r2 + 3 % IDX_MOD]);
	(*player).pos += posit + 1;
}

void	sub(t_players *player, byte *map)
{
	int		r1;
	int		r2;

	r1 = (*player).reg[map[(*player).pos + 2] - 1];
	r2 = (*player).reg[map[(*player).pos + 3] - 1];
	(*player).reg[map[(*player).pos + 4] - 1] = r1 - r2;
	(*player).pos += 5;
	if (r1 - r2 == 0)
		(*player).carry = 1;
}

void	add(t_players *player, byte *map)
{
	int		r1;
	int		r2;

	r1 = (*player).reg[map[(*player).pos + 2] - 1];
	r2 = (*player).reg[map[(*player).pos + 3] - 1];
	(*player).reg[map[(*player).pos + 4] - 1] = r1 + r2;
	(*player).pos += 5;
	if (r1 + r2 == 0)
		(*player).carry = 1;
}

void	st(t_players *player, byte *map)
{
	int		r2;
	char	*binary;
	int		posit;

	r2 = 0;
	posit = 2;
	binary = get_binary(map, player);
	binary += 2;
	if (ft_strnstr(binary, "11", 2))
	{
		r2 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "01", 2))
		r2 = (*player).reg[map[(*player).pos + posit++ + 1] - 1];
	binary -= 2;
	free(binary);
	map[(*player).pos + r2] = (*player).reg[map[(*player).pos + 2] - 1] / 0x1000000;
	map[(*player).pos + r2 + 1] = (*player).reg[map[(*player).pos + 2] - 1] / 0x10000;
	map[(*player).pos + r2 + 2] = (*player).reg[map[(*player).pos + 2] - 1] / 0x100;
	map[(*player).pos + r2 + 3] = (*player).reg[map[(*player).pos + 2] - 1] % 0x100;
	(*player).pos += posit + 1;
}

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
		r1 = translate(map[(*player).pos + posit + 1], map[(*player).pos + posit + 2], map[(*player).pos + posit + 3], map[(*player).pos + posit + 4]);
		posit += 4;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = translate(map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 1], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 2], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 3]);
		posit += 2;
	}
	free(binary);
	(*player).reg[map[(*player).pos + posit++ + 1] - 1] = r1;
	(*player).pos += posit + 1;
	if (r1 == 0)
		(*player).carry = 1;
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
		r1 = (*player).reg[map[(*player).pos + posit++ + 1]];
	else if (ft_strnstr(binary, "10", 2))
	{
		r1 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = translate(0, 0, map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])] + 1);
		posit += 2;
	}
	binary += 2;
	if (ft_strnstr(binary, "10", 2))
	{
		r2 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "01", 2))
		r2 = (*player).reg[map[(*player).pos + posit++ + 1]];
	binary -= 2;
	free(binary);
	(*player).reg[map[(*player).pos + posit++ + 1] - 1] = translate(map[(*player).pos + r1 + r2 + 1], map[(*player).pos + r1 + r2 + 2], map[(*player).pos + r1 + r2 + 3], map[(*player).pos + r1 + r2 + 4]);
	(*player).pos += posit + 2;
	if (translate(map[(*player).pos + r1 + r2 + 1], map[(*player).pos + r1 + r2 + 2], map[(*player).pos + r1 + r2 + 3], map[(*player).pos + r1 + r2 + 4]) == 0)
		(*player).carry = 1;
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
		r1 = translate(map[(*player).pos + posit + 1], map[(*player).pos + posit + 2], map[(*player).pos + posit + 3], map[(*player).pos + posit + 4]);
		posit += 4;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = translate(0, 0, map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 1], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 2]);
		posit += 2;
	}
	free(binary);
	(*player).reg[map[(*player).pos + posit++ + 1] - 1] = r1;
	(*player).pos += posit + 1;
	if (r1 == 0)
		(*player).carry = 1;
}
