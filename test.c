/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 15:53:54 by gtapioca          #+#    #+#             */
/*   Updated: 2020/07/04 21:07:03 by gtapioca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include <stdbool.h>
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



void put_in_stack_of_players_helper(t_player_list **player_list, t_player *player, int pos)
{
	t_player_list	*buff1;
	t_player_list	*buff2;
	t_player_list	*player_list_loc;
	int				count;

	count = 0;
	player_list_loc = *player_list;
	if (player_list_loc == NULL)
	{
		*player_list = (t_player_list *)malloc(sizeof(t_player_list));
		(*player_list)->player = player;
		(*player_list)->next = NULL;
		(*player_list)->prev = NULL;
		if (pos > 0)
			(*player_list)->position = pos;
		else
			(*player_list)->position = 0;
		return ;
	}
	if (pos > 0)
	{
		while((player_list_loc)->next != 0 && pos > (player_list_loc)->position)
		{
			(player_list_loc) = (player_list_loc)->next;
			count++;
		}
		count++;
		if ((player_list_loc)->position == pos)
		{
			printf("usage/--\n");
			exit(1);
		}
		if ((player_list_loc)->next != 0 && (player_list_loc)->prev != 0)
		{
			buff1 = (player_list_loc);
			buff2 = (player_list_loc)->prev;
			player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
			(player_list_loc)->prev = buff2;
			(player_list_loc)->next = buff1;
			(player_list_loc)->player = player;
			(player_list_loc)->position = pos;
			buff1->prev = (player_list_loc);
			buff2->next = (player_list_loc);
			return ;
		}
		if ((player_list_loc)->next != 0 && (player_list_loc)->prev == 0)
		{
			buff1 = (player_list_loc);
			player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
			(player_list_loc)->prev = NULL;
			(player_list_loc)->next = buff1;
			(player_list_loc)->player = player;
			(player_list_loc)->position = pos;
			buff1->prev = (player_list_loc);
			(*player_list) = player_list_loc;
			return ;
		}
		if ((player_list_loc)->next == 0 && (player_list_loc)->prev != 0)
		{
			buff2 = (player_list_loc);
			player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
			(player_list_loc)->prev = buff2;
			(player_list_loc)->next = NULL;
			(player_list_loc)->player = player;
			(player_list_loc)->position = pos;
			buff2->next = (player_list_loc);
			return ;
		}
		if ((player_list_loc)->next == 0 && (player_list_loc)->prev == 0)
		{
			if (player_list_loc->position > pos)
			{
				buff2 = (player_list_loc);
				player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
				(player_list_loc)->prev = NULL;
				(player_list_loc)->next =  buff2;
				(player_list_loc)->player = player;
				(player_list_loc)->position = pos;
				buff2->prev = (player_list_loc);
				*player_list = player_list_loc;
				return ;
			}
			else
			{
				buff2 = (player_list_loc);
				player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
				(player_list_loc)->prev = buff2;
				(player_list_loc)->next =  NULL;
				(player_list_loc)->player = player;
				(player_list_loc)->position = pos;
				buff2->next = (player_list_loc);
				return ;
			}
		}
	}
	else
	{
		while((player_list_loc)->next != 0)
		{
			count++;
			if ((player_list_loc)->position > count)
				break ;
			(player_list_loc) = (player_list_loc)->next;
		}
		count++;
		if ((player_list_loc)->next != 0 && (player_list_loc)->prev != 0)
		{
			buff1 = (player_list_loc);
			buff2 = (player_list_loc)->prev;
			player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
			(player_list_loc)->prev = buff2;
			(player_list_loc)->next = buff1;
			(player_list_loc)->player = player;
			(player_list_loc)->position = 0;
			buff1->prev = (player_list_loc);
			buff2->next = (player_list_loc);
			return ;
		}
		if ((player_list_loc)->next != 0 && (player_list_loc)->prev == 0)
		{
			buff1 = (player_list_loc);
			player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
			(player_list_loc)->prev = NULL;
			(player_list_loc)->next = buff1;
			(player_list_loc)->player = player;
			(player_list_loc)->position = 0;
			buff1->prev = (player_list_loc);
			(*player_list) = player_list_loc;
			return ;
		}
		if ((player_list_loc)->next == 0 && (player_list_loc)->prev != 0)
		{
			if (player_list_loc->position > count)
			{
				buff1 = (player_list_loc);
				buff2 = (player_list_loc)->prev;
				player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
				(player_list_loc)->prev = buff2;
				(player_list_loc)->next = buff1;
				(player_list_loc)->player = player;
				(player_list_loc)->position = 0;
				buff1->prev = (player_list_loc);
				buff2->next = (player_list_loc);
				return ;
			}
			else
			{
				buff2 = (player_list_loc);
				player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
				(player_list_loc)->prev = buff2;
				(player_list_loc)->next = NULL;
				(player_list_loc)->player = player;
				(player_list_loc)->position = 0;
				buff2->next = (player_list_loc);
			}
			return;
		}
		if ((player_list_loc)->next == 0 && (player_list_loc)->prev == 0)
		{
			if (player_list_loc->position >= 2)
			{
				buff2 = (player_list_loc);
				player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
				(player_list_loc)->prev = NULL;
				(player_list_loc)->next =  buff2;
				(player_list_loc)->player = player;
				(player_list_loc)->position = 0;
				buff2->prev = (player_list_loc);
				*player_list = player_list_loc;
				return ;
			}
			else
			{
				buff2 = (player_list_loc);
				player_list_loc = (t_player_list *)malloc(sizeof(t_player_list));
				(player_list_loc)->prev = buff2;
				(player_list_loc)->next =  NULL;
				(player_list_loc)->player = player;
				(player_list_loc)->position = 0;
				buff2->next = (player_list_loc);
				return ;
			}
		}
	}
}

void put_in_stack_of_players(int pos, char *player_name, t_player_list **player_list)
{
	t_player	*player;
	int				fd;

	fd = open(player_name, O_RDONLY);
	if (fd < 0)
	{
		printf("Can't read source file %s\n", player_name);
		exit (1);
	}
	player = (t_player *)malloc(sizeof(t_player));
	players_reader_parse_champions(fd, player, &player_name);
	close(fd);
	put_in_stack_of_players_helper(player_list, player, pos);
}

int check_atoi_honest(char *argv)
{
	while (*argv == '\t' || *argv == '\v' || *argv == '\f' ||
			*argv == '\r' || *argv == '\n' || *argv == ' ')
		argv++;
	if (*argv == '-' || *argv == '+')
		argv++;
	while (*argv == '0' && *(argv + 1) == '0')
		argv++;
	while (*argv >= '0' && *argv <= '9')
		argv++;
	if (*argv != '\0')
		return (0);
	return (1);
}

void parse_arguments(char **argv, t_game_process *game_process, t_player_list **player_list)
{
	int count_dump;

	count_dump = 0;
	while (*argv != 0)
	{
		if (ft_strcmp(*argv, "-dump") == 0)
		{
			argv++;
			if (ft_atoi(*argv) >= 0 &&
					check_atoi_honest(*argv) == 1 &&
						count_dump == 0)
			{
				game_process->dump_cycle = ft_atoi(*argv);
				count_dump++;
			}
			else
			{
				printf("usage/--\n");
				exit(1);
			}
		}
		else if (ft_strcmp(*argv, "-n") == 0)
		{
			argv++;
			if (ft_atoi(*argv) > 0 && ft_atoi(*argv) <= 4 &&
					check_atoi_honest(*argv) == 1 &&
						ft_strcmp(*(argv + 1), "-n") != 0)
			{
				argv++;
				put_in_stack_of_players(ft_atoi(*(argv - 1)), *argv, player_list);
			}
			else
			{
				printf("usage/--\n");
				exit(1);
			}
		}
		else
			put_in_stack_of_players(ft_atoi(*argv), *argv, player_list);
		argv++;
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

size_t	ft_count_words(char const *s, char c)
{
	size_t words;

	words = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			words++;
			while (*s && *s != c)
				s++;
		}
	}
	return (words);
}

void free_memory_after_strsplit(char **argv)
{
	char **beginner;

	beginner = argv;
	while (*argv != 0)
	{
		free(*argv);
		argv++;
	}
	free(*argv);
	free(beginner);
}

void memory_allocator_helper(char *str, char **buff)
{
	char **buff_2;
	int i;
	int j;

	j = 0;
	while(*buff != 0)
		buff++;
	buff_2 = ft_strsplit(str, ' ');
	while (buff_2[j] != 0)
	{
		i = 0;
		*buff = (char *)malloc(sizeof(char)*(ft_strlen(buff_2[j]) + 1));
		while (buff_2[j][i] != 0)
		{
			(*buff)[i] = buff_2[j][i];
			i++;		
		}
		(*buff)[i] = 0;
		j++;
		buff++;
	}
	free_memory_after_strsplit(buff_2);
}

char **memory_allocator(char **argv)
{
	int counter;
	int i;
	char **buff;

	i = 0;
	counter = 0;
	while (argv[i] != 0)
	{
		counter += ft_count_words(argv[i], ' ');
		i++;
	}
	// printf("%d\n", counter);
	buff = (char **)malloc(sizeof(char *)*(counter + 1));
	i = 0;
	while (i < counter)
	{
		buff[i] = 0;
		i++;
	}
	buff[counter] = 0;
	i = 0;
	while(argv[i] != 0)
	{
		memory_allocator_helper(argv[i], buff);
		i++;
	}
	// i = 0;
	// while (buff[i] != 0)
	// {
	// 	printf("%s\n", buff[i]);
	// 	i++;
	// }
	// buff[i] = 0;
	return (buff);
}

int main(int argc, char **argv)
{
	t_game_process *game_process;
	t_player		*player_mass;
	t_player_list	*player_list;
	int fd;
	int i;
	int j;
	int c;
	char **ppp;
	char **ppp1;

	argv++;
	ppp = memory_allocator(argv);
	i = 0;
	j = 0;
	player_list = NULL;
	game_process = (t_game_process *)malloc(sizeof(t_game_process));
	// printf("control_point\n");
	parse_arguments(ppp, game_process, &player_list);
	printf("\n\n\n\n\n\n\n\n");
	while(player_list != 0)
	{
		printf("%s\n", player_list->player->player_header.prog_name);
		player_list = player_list->next;
	}
	// while (*ppp)
	// {
	// 	printf("%s\n", *ppp);
	// 	ppp++;
	// }
}