/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 15:53:54 by gtapioca          #+#    #+#             */
/*   Updated: 2020/06/30 14:36:47 by gtapioca         ###   ########.fr       */
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

void read_code(int fd, char *str, t_player *player)
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
	if (i == player->code_size && (read(fd, str, 1) > 0))
	{
		printf("ERROR: length of code don't accord to code size");
		exit(1);
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
	printf("\n");
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
	printf("comment - %s\n", player->comment);
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
		player->code_size = (*((unsigned int *)(code_size_point)));
		// free(code_size_point);
		return (0);
	}
}

void set_player_name(char *str, t_player *player)
{
	int i;

	i = 0;
	printf("name - ");
	while (i < PROG_NAME_LENGTH && str[i + 4] != '\0')
	{
		printf("%c", str[i + 4]);
		i++;
	}
	printf("\n");
	player->name = (char *)malloc(i + 1);
	player->name[i] = '\0';
	i = 0;
	while (i < (ft_strlen(player->name)))
	{
		player->name[i] = str[i + 4];
		i++;
	}
}

void check_magic_header(char *str, char **argv)
{
	if ((unsigned char)str[0] != 0x00 | (unsigned char)str[1] != 0xea
		| (unsigned char)str[2] != 0x83 | (unsigned char)str[3] != 0xf3)
	{
		printf("Error: File %s has an invalid header\n", *argv);
		exit(1);
	}
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
	check_magic_header(str, argv);
	set_player_name(str, player);
	if (check_nulls_and_code_size(count, str, player, argv) == 0)
	{
		read_comment(str, player);
		read_code(fd, str, player);
	}
}

int main(int argc, char **argv)
{
	t_player *player_mass;
	int fd;
	int i;

	argv++;
	i = 1;
	player_mass = (t_player *)malloc(sizeof(t_player)*(argc - 1));
	if (argc > 5)
	{
		printf("Too many champions\n");
		exit(1);
	}
	while (*argv != 0)
	{
		fd = open(*argv, O_RDONLY);
		players_reader_parse_champions(fd, &(player_mass[i - 1]), argv);
		player_mass[i - 1].ident = i;
		printf("identifier - %d\n\n", i);
		close(fd);
		argv++;
		i++;
	}
}