/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtual_machine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 18:06:36 by gtapioca          #+#    #+#             */
/*   Updated: 2020/07/05 20:46:03 by gtapioca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft/includes/libft.h"
#include <unistd.h>
#include <stdio.h>
#include "corewar.h"
#include "libft/libft.h"
#include <fcntl.h>
#include "op.h"

void print_memory(unsigned char *field)
{
	int counter;

	counter = 0;
	while (counter < MEM_SIZE)
	{
		if (counter % 64 == 0)
		{
			if (counter != 0)
			{
				printf("\n");
				printf("%#.4x : ", counter);
			}
			else
				printf("000000 : ");	
		}
		if (field[counter] / 16 > 0)
			printf("%hhx ", field[counter]);
		else
			printf("0%hhx ", field[counter]);
		counter++;
	}
	printf("\n");
}

void memory_allocator_for_vm(t_player_list *player_list, int divider, unsigned char *field)
{
	int upper_boundary;
	int lower_boundary;
	int counter;

	counter = 0;
	upper_boundary = (MEM_SIZE / divider) * player_list->position;
	lower_boundary = (MEM_SIZE / divider) * (player_list->position - 1);
	while (lower_boundary < upper_boundary)
	{
		if (counter < player_list->player->player_header.prog_size)
			field[lower_boundary] = player_list->player->code[counter];
		else
			field[lower_boundary] = 0;
		counter++;
		lower_boundary++;
	}
}

void virtual_machine_creator(t_game_process *game_process,
	t_player_list *player_list)
{
	int				counter;
	int				divider;
	t_player_list	*player_list_buff;
	t_vm_field_memory *vm_field_memory;
	
	vm_field_memory = (t_vm_field_memory *)malloc(sizeof(t_vm_field_memory));
	vm_field_memory->field = (unsigned char *)malloc(MEM_SIZE);
	player_list_buff = player_list;
	while(player_list_buff->next != 0)
		player_list_buff = player_list_buff->next;
	divider = player_list_buff->position;
	while(player_list_buff->prev != 0)
		player_list_buff = player_list_buff->prev;
	while(player_list_buff)
	{
		memory_allocator_for_vm(player_list_buff, divider, vm_field_memory->field);
		player_list_buff = player_list_buff->next;
	}
	print_memory(vm_field_memory->field);
	// play_corewar();
}