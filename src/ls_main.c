/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 21:24:20 by jkalia            #+#    #+#             */
/*   Updated: 2017/05/01 17:14:13 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

uint8_t		g_ls_flags = 0;

int8_t		parse(int argc, char **argv)
{
	return (0);
}

int			main(int argc, char **argv)
{
	if (argc == 1)
		ft_ls_print_dir(".");
	else
		parse(argc, argv);
	(void)argv;
	return (0);
}
