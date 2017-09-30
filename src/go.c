/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   go.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: liabanzh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/18 17:55:40 by liabanzh          #+#    #+#             */
/*   Updated: 2017/07/18 17:55:41 by liabanzh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/vm.h"
#include "../includes/op.h"
#include <unistd.h>

void	get_stop(t_players *player, byte *map)
{
	int 	stop[16] = {10, 5, 5, 10, 10, 6, 6, 6, 20, 25, 25, 800, 10, 50, 1000, 2};
	if (map[(*player).pos] > 0 && map[(*player).pos] <= 16)
		(*player).stop = stop[map[(*player).pos] - 1];
}

void	delete_split(char ***split)
{
	int		i;

	i = 0;
	while (*split[i] != NULL)
	{
		free(*split[i]);
		*split[i] = NULL;
		i++;
	}
	free(*split);
	*split = NULL;
}

byte	*get_map(t_players *players, int count)
{
	unsigned char	*map;
	int				i;
	int				j;
	char			*ptr;
	char			*tmp;

	j = 1;
	i = 0;
	map = (unsigned char*)malloc(sizeof(unsigned char) * MEM_SIZE);
	ptr = players[0].comands;
	while (i < MEM_SIZE && j <= count)
	{
		if (i >= (MEM_SIZE / count) * j)
		{
			ptr = players[j].comands;
			j++;
		}
		if (*ptr != '\0')
		{
			tmp = ft_strnew(2);
			map[i] = ft_atoi_base(ft_strncpy(tmp, ptr, 2), 16);
			ptr += 2;
			free(tmp);
		}
		else
			map[i] = 0;
		i++;
	}
	return (map);
}

void	get_command(t_players *player, byte *map, t_players **stack, t_players *players)
{
	if (player->stop == 0)
	{
		if (map[player->pos] == 1)
			live(players, &(map[player->pos + 1]), player);
		else if (map[player->pos] == 2)
			ld(player, map);
		else if (map[player->pos] == 3)
			st(player, map);
		else if (map[player->pos] == 4)
			add(player, map);
		else if (map[player->pos] == 5)
			sub(player, map);
		else if (map[player->pos] == 6)
			and_xor(player, map, 'a');
		else if (map[player->pos] == 7)
			and_xor(player, map, 'o');
		else if (map[player->pos] == 8)
			and_xor(player, map, 'x');
		else if (map[player->pos] == 9)
			zjmp(player, map);
		else if (map[player->pos] == 10)
			ldi(player, map);
		else if (map[player->pos] == 11)
			sti(player, map);
		else if (map[player->pos] == 12)
			fork_func(player, map, stack);
		else if (map[player->pos] == 13)
			lld(player, map);
		else if (map[player->pos] == 14)
			lldi(player, map);
		else if (map[player->pos] == 15)
			lfork_func(player, map, stack);
		else if (map[player->pos] == 16)
			aff(player, map);
		else
		{
			player->pos += 1;
			players->pos = players->pos % MEM_SIZE;
		}
		get_stop(player, map);
	}
	else
		player->stop -= 1;
}

int 	get_alive_players(t_players *players)
{
	int 		res;
	int 		i;
	t_players	*tmp;

	res = 0;
	i = 0;
	tmp = players;
	while (tmp[i].header.name != NULL)
	{
		if (tmp[i].comands != NULL)
			res++;
		i++;
	}
	return (res);
}

void	end_game(t_players *players, byte *map, t_players **stack)
{
	int 		i;
	t_players	*tmp;
	t_players	*tmp2;

	i = 0;
	free(map);
	map = NULL;
	tmp = *stack;
	while (tmp != NULL)
	{
		tmp2 = tmp->next;
		free(tmp);
		tmp = tmp2;
	}
	*stack = NULL;
	while (players[i].comands == NULL && players[i].header.name != NULL)
		i++;
	ft_putstr("Contestant #");
	ft_putnbr(players[i].num * -1);
	ft_putstr(", '");
	ft_putstr(players[i].header.name);
	ft_putstr("', has won!\n");
	exit(1);
}

void	go_vm(t_players *players, int count)
{
	byte		*map;
	t_players	*stack;
	t_players	*tmp;
	int			i;
	int 		cycles;
	int 		DIE;
	int 		cycles_test;
	// WINDOW		*win;
	// WINDOW		*win1;
	// WINDOW		*win2;

	i = 0;
	cycles = 0;
	cycles_test = 0;
	stack = NULL;
	// win1 = NULL;
	// win2 = NULL;
	DIE = CYCLE_TO_DIE;
	// win = initscr();
	map = get_map(players, count);
	// vizualize(map, players, &win1, win);
	// status_bar(&win2, win, players);
	// curs_set(0);
	// getch();
	// cursor_refresh(win1, win2, players, map);
	// getch();
	while (players[i].header.name != NULL)
		get_stop(&(players[i++]), map);
	while (1)
	{
		i = 0;
		while (players[i].header.name != NULL)
			if (players[i].comands != NULL)
				get_command(&(players[i++]), map, &stack, players);
		tmp = stack;
		while (tmp != NULL)
			get_command(tmp, map, &stack, players);
		i = 0;
		if (cycles_test >= DIE)
		{
			while (players[i].header.name != NULL)
			{
				if (get_alive_players(players) == 1)
					end_game(players, map, &stack);
				if (players[i].live + players[i].live_amount == 0)
				{
					// free(players[i].comands);
					players[i].comands = NULL;
				}
				if (players[i].live + players[i].live_amount >= NBR_LIVE)
					DIE -= CYCLE_DELTA; 
				players[i].live = 0;
				players[i++].live_amount = 0;
			}
			cycles_test = 0;
		}
		// refresh_map(win1, map);
		// cursor_refresh(win1, win2, players, map);
		// wrefresh(win1);
		// usleep(1000);
		cycles++;
		cycles_test++;
	}
	// delwin(win1);
	// delwin(win2);
	// delwin(win);
	// endwin();
}

void	start_vm(t_players **tmp, int count)
{
	t_players	players[count + 1];
	int 		i;

	i = 0;
	while (tmp[i] != NULL)
	{
		players[i] = *(tmp[i]);
		free(tmp[i]);
		tmp[i] = NULL;
		i++;
	}
	players[count].header.name = NULL;
	players[count].comands = NULL;
	free(tmp);
	tmp = NULL;
	go_vm(players, count);
}

int		main(void)
{
	t_players		players[3];

	players[0].header.name = "zork2";
	players[0].comands = "0ae400020001020370020014";
	players[0].num = -1;
	players[0].reg = (unsigned int*)malloc(sizeof(unsigned int) * 16);
	players[0].live = 0;
	players[0].live_amount = 0;
	ft_bzero(players[0].reg, 16);
	players[0].reg[0] = -1;
	players[0].stop = 0;
	players[0].carry = 0;
	// players[0].header.name = "big_zork";
	// players[0].comands = "0290000000000203700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d303700100d301000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000101000000010100000001010000000109ff2e";
	// players[0].num = -1;
	// players[0].reg = (unsigned int*)malloc(sizeof(unsigned int) * 16);
	// players[0].live = 0;
	// players[0].live_amount = 0;
	// ft_bzero(players[0].reg, 16);
	// players[0].reg[0] = -1;
	// players[0].stop = 0;
	players[1].header.name = "zork";
	players[1].comands = "0b6801000f00010664010000000001010000000109fffb";
	players[1].num = -1;
	players[1].reg = (unsigned int*)malloc(sizeof(unsigned int) * 16);
	players[1].live = 0;
	players[1].live_amount = 0;
	ft_bzero(players[1].reg, 16);
	players[1].reg[0] = -1;
	players[1].stop = 0;
	// players[0].header.name = "helltrain";
	// players[0].comands = "0b6801018700010b6801018f00010b6801019000010b6801005700010b6801018d00010b6801018600090b6801017f00110b680101a500010b6801018900020b680101cd00010b680101c600090b680101980001029000000000100c0143029000000000020290000000001001004acdc7037002ffe8037002ffdf037002ffd6037002ffcd037002ffc4037002ffbb037002ffb2037002ffa9037002ffa0037002ff97037002ff8e037002ff85037002ff7c037002ff73037002ff6a037002ff61037002ff58037002ff4f037002ff46037002ff3d037002ff34037002ff2b037002ff22037002ff19037002ff10037002ff07037002fefe037002fef5037002feec037002fee3037002feda037002fed1037002fec8037002febf037002feb6037002fead037002fea4037002fe9b037002fe92037002fe89037002fe80037002fe77037002fe6e037002fe65037002fe5c037002fe53037002fe4a037002fe41037002fe38037002fe2f037002fe26037002fe1d037002fe14037002fe0b037002fe0209fee8010040b03f0cfffb02900000000010010034867e09fffb010041eca70900030cfff80100420e4a0c005e010033e5a30c002f0100460bcf0c00840290000000000202900f037003030290000000001001004b3b770290000000001009007801004242460cfe7402900f03700302029017037003030290000000001002900000000010090051010040aea50cff9801003e75620cff6e0290ffff01000202901a0370030302900000000010029000000000100290000000001009001b029003700200020290130370030302900000000010090003037002000f037003ffff0100000000";
	// players[0].num = -2;
	// players[0].live = 0;
	// players[0].reg = (unsigned char*)malloc(sizeof(unsigned char) * 16);
	// ft_bzero(players[0].reg, 16);
	// players[0].reg[0] = players[0].num;
	// players[2].header.name = "skynet";
	// players[2].comands = "03700100070290000001430102900000000002037002ffed0b6802ffe800010b6801004600010b6801003f00010b6801016100010b6801015200010b6801001f00010b68010139000102900000000002029000000000100c000e01000030390c012f0901190100003039037002ffdb037002ffd2037002ffc9037002ffc0037002ffb7037002ffae037002ffa5037002ff9c037002ff93037002ff8a037002ff81037002ff78037002ff6f037002ff66037002ff5d037002ff54037002ff4b037002ff42037002ff39037002ff30037002ff27037002ff1e037002ff15037002ff0c037002ff03037002fefa037002fef1037002fee8037002fedf037002fed6037002fecd037002fec4037002febb037002feb2037002fea9037002fea0037002fe97037002fe8e037002fe85037002fe7c037002fe73037002fe6a037002fe61037002fe58037002fe4f037002fe46037002fe3d037002fe34037002fe2b037002fe22037002fe19037002fe10037002fe07037002fe0109feed01000030390c000609fff8010000303909fffb010000303903700201110370020110037002010f037002010e037002010d037002010c037002010b037002010a037002010903700201080370020107037002010603700201050370020104037002010303700201020370020101037002010003700200ff03700200fe03700200fd03700200fc03700200fb03700200fa03700200f903700200f803700200f703700200f603700200f503700200f403700200f303700200f203700200f103700200f003700200ef03700200ee03700200ed03700200ec03700200eb03700200ea03700200e903700200e803700200e703700200e603700200e503700200e403700200e303700200e203700200e103700200df03700200de03700200dd03700200dc09fef2";
	// players[2].num = -3;
	// players[2].live = 0;
	// players[2].reg = (unsigned char*)malloc(sizeof(unsigned char) * 16);
	// ft_bzero(players[2].reg, 16);
	// players[2].reg[0] = players[2].num;
	// players[3].header.name = "Asombra";
	// players[3].comands = "0c005001000000000c005202900f00000f020290000000010c0290000000280b037001000b037010fffc0100000000037010fed4037002fed3037002fed205540b0c0b0900210290000000001009ffdd029000000000100901ce0290000000001009018b0aa4ffc600010d0290000000001003700d000601000000000cfffb03700d000601000000000c002c02900b540a0b0a0290080c0100070290000001110802900000010e0b0290000000000c0c002c09012b02900c0b54070a03500d07029000000114080290000001120b0290000000000c0c00060901050900030b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c01000000000b540a0b0c0b5407080c0100000000010000000002900b5406070a0290050901f50b02900900000c0302900900000c0602900000001a070290000001ef040290000000000509006a010000000002900b5406070a0290050901f50b0c002902900b5403040302900b540304060290000001fb07029000000018040290000000000509002e010000000002900509001303029005090013060290000001ff0702900000001c0402900000000005090003010000000003700a01ff03700b01fe0901f5";
	// players[3].num = -4;
	// players[3].live = 0;
	// players[3].reg = (unsigned char*)malloc(sizeof(unsigned char) * 16);
	// ft_bzero(players[3].reg, 16);
	// players[3].reg[0] = players[3].num;
	players[2].header.name = NULL;
	players[2].comands = NULL;
	go_vm(players, 1);
	return (0);
}
