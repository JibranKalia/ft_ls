/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 21:24:20 by jkalia            #+#    #+#             */
/*   Updated: 2017/05/01 18:35:45 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

uint8_t			g_ls_flags = 0;

static int8_t	handle_flag(int c)
{
	if (c == 'l')
		g_ls_flags |= FLG_l;
	else if (c == 'R')
		g_ls_flags |= FLG_R;
	else if (c == 'a')
		g_ls_flags |= FLG_a;
	else if (c == 'r')
		g_ls_flags |= FLG_r;
	else if (c == 't')
		g_ls_flags |= FLG_t;
	else if (c == '1')
		;
	else
		return (ls_usage(c));
	return (0);
}

static int8_t	parse(int argc, char **argv)
{
	int		i;
	int		j;

	i = 1;
	while (i < argc && argv[i][0] == '-')
	{
		if (argv[i][1] == '-')
			break;
		j = 1;
		while (argv[i][j] != 0)
		{
			CHK(handle_flag(argv[i][j]) == -1, -1);
			++j;
		}
		++i;
	}
	//if (i < argc)
	//	GRD(ft_ls_handle_args(i, argc, argv) == -1, -1);
	//else
	CHK(ft_ls_print_dir(".") == -1, -1);
	return (0);
}

int				main(int argc, char **argv)
{
	int		chk;

	if (argc == 1)
	{
		CHK(ft_ls_print_dir(".") == -1, -1);
	}
	else if (argc > 1)
	{
		CHK(parse(argc, argv) == -1, -1);
	}
	else
		return (-1);
	return (0);
}
