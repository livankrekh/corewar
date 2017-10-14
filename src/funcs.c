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

int 		translate(byte r1, byte r2, byte r3, byte r4)
{
	return (r1 * 256 * 256 * 256 + r2 * 256 * 256 + r3 * 256 + r4);
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
		pos = (player->pos + arg) % MEM_SIZE;
	if (flag == 'r')
		res = (short int)translate(0, 0, pos, (pos + 1) % MEM_SIZE);
	else if (flag == 'd')
		res = arg;
	return (res);
}

void		aff(t_players *player, byte *map)
{
	int		reg;

	reg = get_REG(player, player->pos + 2, map);
	if (reg < 32 || reg > 127)
		return ;
	ft_putstr("Player #");
	ft_putnbr(player->num);
	ft_putstr(" says: ");
	ft_putchar(reg);
	ft_putchar('\n');
	player->pos += 3;
}

void		fork_func(t_players *player, byte *map, t_players **stack)
{
	short int	dir;
	t_players	*tmp;

	// dir = (short int)translate(0, 0, map[(player->pos + 1) % MEM_SIZE], map[(player->pos + 2) % MEM_SIZE]) % IDX_MOD;
	dir = (short int)get_TDIR(2, player->pos + 1, map) % IDX_MOD;
	if (dir + player->pos < 0)
		dir = player->pos;
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
	ft_strncpy(tmp->header.prog_name, "fork", 4);
	tmp->comands = NULL;
	tmp->reg = player->reg;
	if (ft_strnstr(player->header.prog_name, "fork", 4))
	{
		tmp->last_live_ptr = player->last_live_ptr;
		tmp->live_ptr = player->live_ptr;
	}
	else
	{
		tmp->last_live_ptr = &(player->last_live);
		tmp->live_ptr = &(player->live);
	}
	tmp->next = NULL;
	tmp->cycles = player->cycles;
	tmp->pos = player->pos + dir;
	tmp->stop = 0;
	tmp->carry = player->carry;
	player->pos += 3;
}

void	lfork_func(t_players *player, byte *map, t_players **stack)
{
	short int	dir;
	t_players	*tmp;

	// dir = (short int)translate(0, 0, map[(player->pos + 1) % MEM_SIZE], map[(player->pos + 2) % MEM_SIZE]);
	dir = (short int)get_TDIR(2, player->pos + 1, map);
	if (dir + player->pos < 0)
		dir = player->pos;
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
	ft_strncpy(tmp->header.prog_name, "fork", 4);
	tmp->comands = NULL;
	tmp->reg = player->reg;
	if (ft_strnstr(player->header.prog_name, "fork", 4))
	{
		tmp->last_live_ptr = player->last_live_ptr;
		tmp->live_ptr = player->live_ptr;
	}
	else
	{
		tmp->last_live_ptr = &(player->last_live);
		tmp->live_ptr = &(player->live);
	}
	tmp->next = NULL;
	tmp->pos = player->pos + dir % MEM_SIZE;
	tmp->stop = 0;
	tmp->cycles = player->cycles;
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
	{
		r1 = get_REG(player, player->pos + posit + 1, map);
		// r1 = (*player).reg[map[((*player).pos + posit++ + 1) % MEM_SIZE] - 1 % REG_NUMBER];
		posit++;
	}
	else if (ft_strnstr(opp, "10", 2))
	{
		r1 = get_TDIR(4, player->pos + posit + 1, map);
		// r1 = translate(map[(*player).pos + 2], map[(*player).pos + 3], map[(*player).pos + 4], map[(*player).pos + 5]);
		posit += 4;
	}
	else if (ft_strnstr(opp, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		// r1 = translate(map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 1], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 2], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 3]);
		posit += 2;
	}
	opp += 2;
	if (ft_strnstr(opp, "01", 2))
	{
		r2 = get_REG(player, player->pos + posit + 1, map);
		// r2 = (*player).reg[map[(*player).pos + posit++ + 1] - 1];
		posit++;
	}
	else if (ft_strnstr(opp, "10", 2))
	{
		r2 = get_TDIR(4, player->pos + posit + 1, map);
		// r2 = translate(map[(*player).pos + posit + 1], map[(*player).pos + posit + 2], map[(*player).pos + posit + 3], map[(*player).pos + posit + 4]);
		posit += 4;
	}
	else if (ft_strnstr(opp, "11", 2))
	{
		r2 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		// r2 = translate(map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 1], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 2], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 3]);
		posit += 2;
	}
	opp -= 2;
	free(opp);
	if ((unsigned char)(map[((*player).pos + posit + 1) % MEM_SIZE] - 1) > REG_NUMBER - 1)
		return ;
	if (flag == 'a')
		player->reg[map[((*player).pos + posit + 1) % MEM_SIZE] - 1] = r1 & r2;
	else if (flag == 'o')
		player->reg[map[(*player).pos + posit + 1] - 1] = r1 | r2;
	else if (flag == 'x')
		player->reg[map[(*player).pos + posit + 1] - 1] = r1 ^ r2;
	player->pos += posit + 2;
	if (flag == 'a' && (r1 & r2) == 0)
		player->carry = 1;
	else if (flag == 'o' && (r1 | r2) == 0)
		player->carry = 1;
	else if (flag == 'x' && (r1 ^ r2) == 0)
		player->carry = 1;
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
			player->pos += jmp % MEM_SIZE;
	}
	else
		player->pos += 5;
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
			player->pos += 5;
			return ;
		}
		i++;
	}
	if (players[i].header.prog_name[0] == '\0' && ft_strnstr(player->header.prog_name, "fork", 4))
	{
		*(player->live_ptr) += 1;
		*(player->last_live_ptr) = *(player->cycles);
	}
	else if (players[i].header.prog_name[0] == '\0')
	{
		(*player).live += 1;
		player->last_live = *(player->cycles);
	}
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
	{
		r1 = get_REG(player, player->pos + posit + 1, map);
		posit++;
		// r1 = (char)(*player).reg[map[(*player).pos + posit++ + 1] - 1];
	}
	else if (ft_strnstr(binary, "10", 2))
	{
		r1 = get_TDIR(2, player->pos + posit + 1, map);
		// r1 = (short int)translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		// r1 = (short int)translate(0, 0, map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])] + 1);
		posit += 2;
	}
	binary += 2;
	if (ft_strnstr(binary, "01", 2))
	{
		r2 = get_REG(player, player->pos + posit + 1, map);
		// r2 = (char)(*player).reg[map[(*player).pos + posit++ + 1] - 1];
		posit++;
	}
	else if (ft_strnstr(binary, "10", 2))
	{
		r2 = get_TDIR(2, player->pos + posit + 1, map);
		// r2 = (short int)translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	binary -= 4;
	free(binary);
	if (player->pos + ((r1 + r2) % IDX_MOD) < 0)
	{
		map[MEM_SIZE + (player->pos + ((r1 + r2) % IDX_MOD))] = (unsigned int)get_REG(player, player->pos + 2, map) / 0x1000000;
		map[MEM_SIZE + (player->pos + ((r1 + r2) % IDX_MOD + 1))] = (unsigned int)get_REG(player, player->pos + 2, map) / 0x10000;
		map[MEM_SIZE + (player->pos + ((r1 + r2) % IDX_MOD + 2))] = (unsigned int)get_REG(player, player->pos + 2, map) / 0x100;
		map[MEM_SIZE + (player->pos + ((r1 + r2) % IDX_MOD + 3))] = (unsigned int)get_REG(player, player->pos + 2, map) % 0x100;
	}
	else
	{
		map[player->pos + ((r1 + r2) % IDX_MOD)] = (unsigned int)get_REG(player, player->pos + 2, map) / 0x1000000;
		map[player->pos + ((r1 + r2) % IDX_MOD + 1)] = (unsigned int)get_REG(player, player->pos + 2, map) / 0x10000;
		map[player->pos + ((r1 + r2) % IDX_MOD + 2)] = (unsigned int)get_REG(player, player->pos + 2, map) / 0x100;
		map[player->pos + ((r1 + r2) % IDX_MOD + 3)] = (unsigned int)get_REG(player, player->pos + 2, map) % 0x100;
	}
	player->pos += posit + 1;
}

void	ldi(t_players *player, byte *map) // not safe
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
		// r1 = (*player).reg[map[(*player).pos + posit++ + 1]];
		posit++;
	}
	else if (ft_strnstr(binary, "10", 2))
	{
		r1 = (short int)get_TDIR(2, player->pos + posit + 1, map);
		// r1 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		// r1 = translate(0, 0, map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])] + 1);
		posit += 2;
	}
	binary += 2;
	if (ft_strnstr(binary, "10", 2))
	{
		r2 = get_TDIR(2, player->pos + posit + 1, map);
		// r2 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "01", 2))
	{
		r2 = get_REG(player, player->pos + posit + 1, map);
		// r2 = (*player).reg[map[(*player).pos + posit++ + 1]];
		posit++;
	}
	binary -= 2;
	free(binary);
	(*player).reg[map[(*player).pos + posit++ + 1] - 1] = translate(map[(*player).pos + r1 + r2 % IDX_MOD], map[(*player).pos + r1 + r2 + 1 % IDX_MOD], map[(*player).pos + r1 + r2 + 2 % IDX_MOD], map[(*player).pos + r1 + r2 + 3 % IDX_MOD]);
	(*player).pos += posit + 1;
}

void	sub(t_players *player, byte *map)
{
	int		r1;
	int		r2;

	r1 = get_REG(player, player->pos + 2, map);
	r2 = get_REG(player, player->pos + 3, map);
	// r1 = (*player).reg[map[(*player).pos + 2] - 1];
	// r2 = (*player).reg[map[(*player).pos + 3] - 1];
	if (map[(player->pos + 4) % MEM_SIZE] < 1 || map[(player->pos + 4) % MEM_SIZE] > REG_NUMBER)
		return ;
	player->reg[map[player->pos + 4] - 1] = r1 - r2;
	player->pos += 5;
	if (r1 - r2 == 0)
		player->carry = 1;
}

void	add(t_players *player, byte *map)
{
	int		r1;
	int		r2;

	r1 = get_REG(player, player->pos + 2, map);
	r2 = get_REG(player, player->pos + 3, map);
	// r1 = (*player).reg[map[(*player).pos + 2] - 1];
	// r2 = (*player).reg[map[(*player).pos + 3] - 1];
	if (map[(player->pos + 4) % MEM_SIZE] < 1 || map[(player->pos + 4) % MEM_SIZE] > REG_NUMBER)
		return ;
	player->reg[map[player->pos + 4] - 1] = r1 + r2;
	player->pos += 5;
	if (r1 + r2 == 0)
		player->carry = 1;
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
		r2 = get_INDIR(player, 'd', player->pos + posit + 1, map) % IDX_MOD;
		// r2 = (short int)translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) % IDX_MOD;
		posit += 2;
	}
	else if (ft_strnstr(binary, "01", 2))
	{
		r2 = get_REG(player, player->pos + posit + 1, map) % IDX_MOD;
		// r2 = (*player).reg[map[(*player).pos + posit++ + 1] - 1] % IDX_MOD;
		posit++;
	}
	binary -= 2;
	free(binary);
	if (player->pos + r2 < 0)
		r2 = MEM_SIZE - r2;
	map[(player->pos + r2) % MEM_SIZE] = (unsigned int)get_REG(player, player->pos + 2, map) / 0x1000000;
	map[(player->pos + r2 + 1) % MEM_SIZE] = (unsigned int)get_REG(player, player->pos + 2, map) / 0x10000;
	map[(player->pos + r2 + 2) % MEM_SIZE] = (unsigned int)get_REG(player, player->pos + 2, map) / 0x100;
	map[(player->pos + r2 + 3) % MEM_SIZE] = (unsigned int)get_REG(player, player->pos + 2, map) % 0x100;
	player->pos += posit + 1;
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
		r1 = get_TDIR(4, player->pos + posit + 1, map);
		// r1 = translate(map[(*player).pos + posit + 1], map[(*player).pos + posit + 2], map[(*player).pos + posit + 3], map[(*player).pos + posit + 4]);
		posit += 4;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		// r1 = translate(map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 1], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 2], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 3]);
		posit += 2;
	}
	free(binary);
	if (map[(player->pos + posit + 1) % MEM_SIZE] < 1 || map[(player->pos + posit + 1) % MEM_SIZE] > REG_NUMBER)
		return ;
	player->reg[map[(player->pos + posit + 1) % MEM_SIZE] - 1] = r1;
	player->pos += ++posit + 1;
	if (r1 == 0)
		player->carry = 1;
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
		// r1 = (*player).reg[map[(*player).pos + posit++ + 1]];
		posit++;
	}
	else if (ft_strnstr(binary, "10", 2))
	{
		r1 = get_TDIR(2, player->pos + posit + 1, map);
		// r1 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		// r1 = translate(0, 0, map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2])] + 1);
		posit += 2;
	}
	binary += 2;
	if (ft_strnstr(binary, "10", 2))
	{
		r2 = get_TDIR(2, player->pos + posit + 1, map);
		// r2 = translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]);
		posit += 2;
	}
	else if (ft_strnstr(binary, "01", 2))
	{
		r2 = get_REG(player, player->pos + posit + 1, map);
		// r2 = (*player).reg[map[(*player).pos + posit++ + 1]];
		posit++;
	}
	binary -= 2;
	free(binary);
	if (map[(player->pos + posit + 1) % MEM_SIZE] < 1 || map[(player->pos + posit + 1) % MEM_SIZE] > REG_NUMBER)
		return ;
	player->reg[map[player->pos + posit + 1] - 1] = get_TDIR(4, (player->pos + r1 + r2) % MEM_SIZE, map);
	player->pos += ++posit + 1;
	if (get_TDIR(4, (player->pos + r1 + r2) % MEM_SIZE, map) == 0)
		player->carry = 1;
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
		// r1 = translate(map[(*player).pos + posit + 1], map[(*player).pos + posit + 2], map[(*player).pos + posit + 3], map[(*player).pos + posit + 4]);
		posit += 4;
	}
	else if (ft_strnstr(binary, "11", 2))
	{
		r1 = get_INDIR(player, 'r', player->pos + posit + 1, map);
		// r1 = translate(0, 0, map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 1], map[(*player).pos + translate(0, 0, map[(*player).pos + posit + 1], map[(*player).pos + posit + 2]) + 2]);
		posit += 2;
	}
	free(binary);
	if (map[(player->pos + posit + 1) % MEM_SIZE] < 1 || map[(player->pos + posit + 1) % MEM_SIZE] > REG_NUMBER)
		return ;
	player->reg[map[player->pos + posit + 1] - 1] = r1;
	player->pos += ++posit + 1;
	if (r1 == 0)
		player->carry = 1;
}
