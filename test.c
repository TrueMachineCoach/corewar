/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gtapioca <gtapioca@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/26 15:53:54 by gtapioca          #+#    #+#             */
/*   Updated: 2020/06/26 21:24:01 by gtapioca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdio.h>
#include "corewar.h"
#include "libft/libft.h"
#include <fcntl.h>
#include "get_next_line/get_next_line.h"

void check_magic_header(unsigned char *str)
{
	printf("%hhu, %hhu, %hhu, %hhu", str[0], str[1], str[2], str[3]);
	if ((unsigned char)str[0] != 0x00 && (unsigned char)str[1] != 0xea &&
		(unsigned char)str[2] != 0x83 && (unsigned char)str[3] != 0xf3)
	{
		printf("\nError\n");
		printf("%c", str[4]);
		exit(1);
	}
}

int main(int argc, char **argv)
{
	int			fd;
	t_player	*player_1;
	unsigned char str[30];

	argv++;
	fd = open(*argv, O_RDONLY);
	player_1 = (t_player *)malloc(sizeof(t_player));
	read(fd, str, 4);
	printf("%s\n", str);
	check_magic_header(str);
	close(fd);
}