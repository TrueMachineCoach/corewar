/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 19:13:16 by gtapioca          #+#    #+#             */
/*   Updated: 2020/07/06 19:33:35 by gtapioca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H
# include <unistd.h>
# include "op.h"

typedef struct s_vm_field_memory
{
	unsigned char *field;
	t_op		*op_tab;
}			t_vm_field_memory; 

typedef struct 			s_player
{
	t_header			player_header;
	int					ident;
	unsigned char		*code;
	u_int64_t			last_live_cycle_number;
}						t_player;

typedef struct s_game_process
{
	u_int64_t	cycle_number;
	u_int64_t	cycle_to_die;
	u_int64_t	dump_cycle;
	u_int64_t	number_of_live_since_last_check;
	u_int64_t	checks_counter;
}				t_game_process;

typedef struct s_player_process
{
	u_int64_t	PC;
	u_int8_t		registers[REG_NUMBER*REG_SIZE];
	u_int64_t	cycles_to_wait;
	struct s_player_process *next;
	
}				t_player_process;

typedef struct			s_player_list
{
	t_player					*player;
	int							position;
	struct s_player_list		*next;
	struct s_player_list		*prev;
}						t_player_list;

void virtual_machine_creator(t_game_process *game_process,
	t_player_list *player_list,  t_op *op_tab);
#endif