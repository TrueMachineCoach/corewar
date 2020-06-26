/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corewar.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 19:13:16 by gtapioca          #+#    #+#             */
/*   Updated: 2020/06/26 19:59:35 by gtapioca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

typedef struct  s_player
{
	int					ident;
	char				*name;
	char				*comment;
	int					code_size;
	unsigned char		*code;
	// size_t				current_lives_num;
	// size_t				previous_lives_num;
	size_t				last_live;
	struct s_player		*next;
	
}		t_player;


