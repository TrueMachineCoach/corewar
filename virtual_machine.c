/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virtual_machine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/05 18:06:36 by gtapioca          #+#    #+#             */
/*   Updated: 2020/07/06 19:39:07 by gtapioca         ###   ########.fr       */
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

t_player_process *create_processes(t_player_list *player_list,
	int divider, t_vm_field_memory *vm_field_memory)
{
	t_player_process	*begin;
	t_player_process	*player_process;
	int					counter;

	while (player_list->next)
		player_list = player_list->next;
	while (player_list)
	{
		counter = REG_SIZE - 1;
		if (player_list->next == 0)
		{
			begin = (t_player_process *)malloc(sizeof(t_player_process));
			while(counter > 0)
			{
				begin->registers[counter] = (unsigned char)(255);
				counter--;
			}
			begin->registers[counter] = (unsigned char)(-1 * (player_list->position));
			player_process = begin;
			player_process->next = 0;
			player_process->PC = (MEM_SIZE / divider) * (player_list->position - 1);
			player_process->cycles_to_wait = (vm_field_memory->op_tab)[(vm_field_memory->field)[player_process->PC] - 1].cycles_before_complete;
			printf("%d %llu %llu\n", ((int *)(player_process->registers))[0], player_process->PC, player_process->cycles_to_wait);
		}
		else
		{
			player_process->next = (t_player_process *)malloc(sizeof(t_player_process));
			player_process = player_process->next;
			player_process->next = 0;
			while(counter > 0)
			{
				player_process->registers[counter] = (unsigned char)(255);
				counter--;
			}
			player_process->registers[counter] = (unsigned char)(-1 * (player_list->position));
			player_process->PC = (MEM_SIZE / divider) * (player_list->position - 1);
			player_process->cycles_to_wait = (vm_field_memory->op_tab)[(vm_field_memory->field)[player_process->PC] - 1].cycles_before_complete;
			printf("%d %llu %llu\n", ((int *)(player_process->registers))[0], player_process->PC, player_process->cycles_to_wait);
		}
		player_list = player_list->prev;
	}
	return (begin);
}

void play_corewar(t_game_process *game_process, t_player_list *player_list, int divider,
	t_vm_field_memory *vm_field_memory)
{
	uint64_t			checks_counter;
	t_player_process	*player_process;
	int	cycles_counter_between_checks;

	cycles_counter_between_checks = 0;
	checks_counter = 0;
	player_process = create_processes(player_list, divider, vm_field_memory);
	// while(game_process->cycle_to_die > 0)
	// {
	// 	if (cycles_counter_between_checks == CYCLE_TO_DIE)
	// 	{
	// 		check_alives();
	// 		checks_counter += 1;
	// 	}
	// 	else
	// 		players_operations_compliting();
	// }
}

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
	t_player_list *player_list,  t_op *op_tab)
{
	int				counter;
	int				divider;
	t_player_list	*player_list_buff;
	t_vm_field_memory *vm_field_memory;
	
	vm_field_memory = (t_vm_field_memory *)malloc(sizeof(t_vm_field_memory));
	vm_field_memory->field = (unsigned char *)malloc(MEM_SIZE);
	vm_field_memory->op_tab = op_tab;
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
	play_corewar(game_process, player_list, divider, vm_field_memory);
}