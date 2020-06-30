/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 19:13:16 by gtapioca          #+#    #+#             */
/*   Updated: 2020/06/30 18:56:32 by gtapioca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COREWAR_H
# define COREWAR_H

#include <unistd.h>
#include "op.h"

typedef struct 			s_player
{
	t_header			player_header;
	int					ident;
	unsigned char		*code;
	u_int8_t			last_live_cycle_number;
	struct s_player		*next;
}						t_player;

typedef struct s_game_process
{
	u_int64_t	cycle_number;
	u_int64_t	cycle_to_die;
	u_int64_t	dump_cycle;
}				t_game_process;

typedef struct s_player_process
{
	u_int64_t	PC;
	uint8_t	registers[REG_NUMBER*REG_SIZE];	
}				t_player_process;

#endif