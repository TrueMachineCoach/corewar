/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operation_executing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/07 21:14:34 by gtapioca          #+#    #+#             */
/*   Updated: 2020/07/07 21:35:51 by gtapioca         ###   ########.fr       */
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

void players_operations_executing(t_game_process *game_process, t_player_process *player_process,
	t_player_list *player_list, t_vm_field_memory *vm_field_memory)
{
	while (player_process)
	{
		if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[0].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[1].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[2].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[3].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[4].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[5].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[6].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[7].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[8].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[9].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[10].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[11].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[12].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[13].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[14].bytecode_operation)
		{

		}
		else if ((vm_field_memory->field)[player_process->PC] ==
			game_process->op_tab[15].bytecode_operation)
		{

		}
		else
			player_process->PC += 1;
		player_process = player_process->next;
	}
}