/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 15:53:54 by gtapioca          #+#    #+#             */
/*   Updated: 2020/06/27 19:47:51 by gtapioca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "corewar.h"
#include "libft/libft.h"
#include <fcntl.h>
#include "get_next_line/get_next_line.h"
#include "op.h"

void read_code(char *str, t_player *player)
{
	str = str + 2192;
	int i;
	int j;

	i = 0;
	j = 0;
	player->code = (unsigned char *)malloc(sizeof(2000));
	printf("code size = %d\n", player->code_size);
	while (i < player->code_size)
	{
		player->code[i] = str[i];
		i++;
	}
	while (j < player->code_size)
	{
		if ((player->code[j]) >= 16)
			printf("%x ", (player->code[j]));
		else
			printf("0%x ", (player->code[j]));
		j++;
		if (j % 8 == 0 && j % 16 != 0)
			printf("  ");
		if (j % 16 == 0)
			printf("\n");
	}
	printf("\n\n");
}

void read_comment(char *str, t_player *player)
{
	int i;

	i = 0;
	player->comment = (char *)malloc(2048);
	while (i < 2048)
	{
		player->comment[i] = str[i + 140];
		i++;
	}
	printf("%s\n", player->comment);
}

void check_nulls_and_code_size(char *str, t_player *player)
{
	int i;
	int j;
	unsigned char *code_size_point;

	i = 139;
	j = 0;
	if (str[132] != 0 && str[133] != 0
		&& str[134] != 0 && str[135] != 0)
	{
		printf("ERROR!: mistake in the separated nulls\n");
		exit(1);
	}
	code_size_point = (unsigned char *)malloc(4);
	while(j < 4)
	{
		code_size_point[j] = str[139-j];
		j++;
	}
	player->code_size = (*((unsigned int *)(code_size_point)));
	free(code_size_point);
}

void set_player_name(char *str, t_player *player)
{
	int i;

	i = 4;
	while (i <= 127 && str[i] != '\0')
	{
		printf("%c", str[i]);
		i++;
	}
	printf("\n");
	player->name = (char *)malloc(128);
	i = 4;
	while (i <= 127)
	{
		player->name[i - 4] = str[i];
		i++;
	}
}

void check_magic_header(char *str)
{
	if ((unsigned char)str[0] != 0x00 && (unsigned char)str[1] != 0xea &&
		(unsigned char)str[2] != 0x83 && (unsigned char)str[3] != 0xf3)
	{
		printf("ERROR!: wrong magic_header");
		exit(1);
	}
}

void players_reader_parse_champions(int fd, t_player *player)
{
	char str[2192 + CHAMP_MAX_SIZE];

	read(fd, str, 3000);
	check_magic_header(str);
	set_player_name(str, player);
	check_nulls_and_code_size(str, player);
	read_comment(str, player);
	read_code(str, player);
}

int main(int argc, char **argv)
{
	t_player *player_mass;
	int fd;
	int i;

	i = 0;
	argv++;
	player_mass = (t_player *)malloc(sizeof(t_player)*(argc - 1));
	while (*argv != 0)
	{
		fd = open(*argv, O_RDONLY);
		players_reader_parse_champions(fd, &(player_mass[i]));
		close(fd);
		argv++;
		i++;
	}
}