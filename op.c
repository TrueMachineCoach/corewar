/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 19:43:05 by gtapioca          #+#    #+#             */
/*   Updated: 2020/07/06 19:13:10 by gtapioca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corewar.h"
#include <stdlib.h>
#include "op.h"
#include <stdio.h>

int pui = 5;

t_op    op_tab[17] =
{
	{"live", 1, {T_DIR}, 1, 10, "alive", 0, 0},
	{"ld", 2, {T_DIR | T_IND, T_REG}, 2, 5, "load", 1, 0},
	{"st", 2, {T_REG, T_IND | T_REG}, 3, 5, "store", 1, 0},
	{"add", 3, {T_REG, T_REG, T_REG}, 4, 10, "addition", 1, 0},
	{"sub", 3, {T_REG, T_REG, T_REG}, 5, 10, "soustraction", 1, 0},
	{"and", 3, {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG}, 6, 6,
		"et (and  r1, r2, r3   r1&r2 -> r3", 1, 0},
	{"or", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 7, 6,
		"ou  (or   r1, r2, r3   r1 | r2 -> r3", 1, 0},
	{"xor", 3, {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG}, 8, 6,
		"ou (xor  r1, r2, r3   r1^r2 -> r3", 1, 0},
	{"zjmp", 1, {T_DIR}, 9, 20, "jump if zero", 0, 1},
	{"ldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 10, 25,
		"load index", 1, 1},
	{"sti", 3, {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG}, 11, 25,
		"store index", 1, 1},
	{"fork", 1, {T_DIR}, 12, 800, "fork", 0, 1},
	{"lld", 2, {T_DIR | T_IND, T_REG}, 13, 10, "long load", 1, 0},
	{"lldi", 3, {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG}, 14, 50,
		"long load index", 1, 1},
	{"lfork", 1, {T_DIR}, 15, 1000, "long fork", 0, 1},
	{"aff", 1, {T_REG}, 16, 2, "aff", 1, 0},
	{0, 0, {0}, 0, 0, 0, 0, 0}
};

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
	t_player_list		*player_list_1;
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
	player_list_1 = player_list;
	// printf("\n\n\n\n\n\n\n\n");
	printf("Introducing contestants...\n");
	while(player_list_1 != 0)
	{
		printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\") !\n",
			player_list_1->position,
			player_list_1->player->player_header.prog_size,
			player_list_1->player->player_header.prog_name,
			player_list_1->player->player_header.comment);
		player_list_1 = player_list_1->next;
	}
	game_process->cycle_to_die = CYCLE_TO_DIE;
	game_process->cycle_to_die = 0;
	virtual_machine_creator(game_process, player_list, op_tab);
	// printf("%s\n", op_tab[11].name);
	// printf("%d", pui);
	// while (*ppp)
	// {
	// 	printf("%s\n", *ppp);
	// 	ppp++;
	// }
}