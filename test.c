/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 15:53:54 by gtapioca          #+#    #+#             */
/*   Updated: 2020/06/30 18:57:23 by gtapioca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft/includes/libft.h"
#include <unistd.h>
#include <stdio.h>
#include "corewar.h"
#include "libft/libft.h"
#include <fcntl.h>
// #include "get_next_line/get_next_line.h"
#include "op.h"

void read_code(int fd, char *str, t_player *player, int count)
{
	str = str + 2192;
	int i;
	int j;

	i = 0;
	j = 0;
	player->code = (unsigned char *)malloc(count);
	printf("code size = %x\n", player->player_header.prog_size);
	while (i < player->player_header.prog_size)
	{
		player->code[i] = str[i];
		i++;
	}
	while (j < player->player_header.prog_size)
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
	printf("\n");
}

void read_comment(char *str, t_player *player)
{
	int i;

	i = 0;
	while (i < COMMENT_LENGTH)
	{
		player->player_header.comment[i] = str[i + 12 + PROG_NAME_LENGTH];
		i++;
	}
	printf("comment - %s\n", player->player_header.comment);
}

int check_nulls_and_code_size(int count, char *str, t_player *player, char **argv)
{
	int i;
	int j;
	unsigned char *code_size_point;

	i = 139;
	j = 0;
	if (str[4 + PROG_NAME_LENGTH] != 0 | str[5 + PROG_NAME_LENGTH] != 0
		| str[6 + PROG_NAME_LENGTH] != 0 | str[7 + PROG_NAME_LENGTH] != 0
			| str[12 + PROG_NAME_LENGTH + COMMENT_LENGTH] != 0 | str[13 + PROG_NAME_LENGTH + COMMENT_LENGTH] != 0
		  		| str[14 + PROG_NAME_LENGTH + COMMENT_LENGTH] != 0 | str[15 + PROG_NAME_LENGTH + COMMENT_LENGTH] != 0)
	{
		printf("Error : mistake in the separating nulls\n");
		exit(1);
	}
	code_size_point = (unsigned char *)malloc(4);
	while (j < 4)
	{
		code_size_point[j] = str[11 + PROG_NAME_LENGTH - j];
		j++;
	}
	if ((*((unsigned int *)(code_size_point))) > CHAMP_MAX_SIZE)
	{
		printf("Error: File %s has too large a code (%d bytes > %d bytes)\n",
			*argv, (*((unsigned int *)(code_size_point))), CHAMP_MAX_SIZE);
		free(code_size_point);
		exit (1);
	}
	else if (count != (*((unsigned int *)(code_size_point))))
	{
		printf("Error: File %s has a code size that differ from what its header says\n", *argv);
		free(code_size_point);
		exit (1);
	}
	else
	{
		// player->code_size = (*((unsigned int *)(code_size_point)));
		player->player_header.prog_size = (*((unsigned int *)(code_size_point)));
		// free(code_size_point);
		return (0);
	}
}

void set_player_name(char *str, t_player *player)
{
	int i;
	int buff;

	i = 0;
	printf("name - ");
	while (i < PROG_NAME_LENGTH && str[i + 4] != '\0')
	{
		printf("%c", str[i + 4]);
		i++;
	}
	printf("\n");
	buff = i;
	i = 0;
	while (i < buff)
	{
		player->player_header.prog_name[i] = str[i + 4];
		i++;
	}
	player->player_header.prog_name[i] = '\0';
}

void check_magic_header(char *str, char **argv, t_player *player)
{
	char helper[4];

	if ((unsigned char)str[0] != 0x00 | (unsigned char)str[1] != 0xea
		| (unsigned char)str[2] != 0x83 | (unsigned char)str[3] != 0xf3)
	{
		printf("Error: File %s has an invalid header\n", *argv);
		exit(1);
	}
	helper[0] = str[3];
	helper[1] = str[2];
	helper[2] = str[1];
	helper[3] = str[0];	
	player->player_header.magic = *((unsigned int *)helper);
}

void players_reader_parse_champions(int fd, t_player *player, char **argv)
{
	char str[BUFFER_SIZE];
	int c;
	int count;

	count = 0;
	while ((c = read(fd, str, BUFFER_SIZE)))
		count = count + c;
	count = count - PROG_NAME_LENGTH - COMMENT_LENGTH - 16;
	lseek(fd, 0, SEEK_SET);
	read(fd, str, BUFFER_SIZE);
	check_magic_header(str, argv, player);
	set_player_name(str, player);
	if (check_nulls_and_code_size(count, str, player, argv) == 0)
	{
		read_comment(str, player);
		read_code(fd, str, player, count);
	}
}

// char *champion_order_creator()
// {
	
// }

// void position_mass_initializer()
// {
	
// }

// char *flag_checker(char **argv, int argc)
// {
// 	int position_mass[8];
// 	char **brgv;

// 	position_mass_initializer(&position_mass);
// 	brgv = argv;
// 	while (*brgv)
// 	{
// 		if (*brgv == '-')
// 		{
// 			brgv++;
// 			if (*brgv == 'n')
// 				return(champion_order_creator());
// 			if (!ft_strlen(*argv, '-dump'))
// 			{
				
// 			}
			
// 		}
// 		brgv++;
// 	}
// }

int main(int argc, char **argv)
{
	t_game_process *game_process;
	t_player *player_mass;
	int fd;
	int i;
	char *ppp;

	argv++;
	i = 1;
	game_process = (t_game_process *)malloc(sizeof(t_game_process));
	player_mass = (t_player *)malloc(sizeof(t_player)*(argc - 1));
	if (argc > MAX_ARGS_NUMBER + 1)
	{
		printf("Too many champions\n");
		exit(1);
	}
	while (*argv != 0)
	{
		fd = open(*argv, O_RDONLY);
		if (fd < 0)
		{
			printf("Can't read source file %s\n", *argv);
			exit (1);
		}
		players_reader_parse_champions(fd, &(player_mass[i - 1]), argv);
		player_mass[i - 1].ident = i;
		printf("identifier - %d\n\n", i);
		close(fd);
		argv++;
		i++;
	}
	printf("%x\n", player_mass[0].player_header.prog_size);
}