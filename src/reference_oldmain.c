/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reference_oldmain.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 21:24:20 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/26 16:49:04 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

intmax_t				g_ls_flags = 0;

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
	else if (c == 'T')
		g_ls_flags |= FLG_T;
	else if (c == 'f')
		g_ls_flags |= (FLG_f | FLG_a);
	else if (c == '1')
		g_ls_flags |= FLG_1;
	else if (c == 'C')
		g_ls_flags |= FLG_C;
	else if (c == 'x')
		g_ls_flags |= FLG_x;
	else if (c == 'c')
		g_ls_flags |= FLG_c;
	else if (c == 'U')
		g_ls_flags |= FLG_U;
	else if (c == 'u')
		g_ls_flags |= FLG_u;
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
	if (i < argc)
		CHK(ls_handle_args(--i, argc, argv) == -1, -1);
	else
		CHK(ls_print_dir(".") == -1, -1);
	return (0);
}

int				main(int argc, char **argv)
{
	if (argc == 1)
		CHK(ls_print_dir(".") == -1, -1);
	else if (argc > 1)
		CHK(parse(argc, argv) == -1, -1);
	else
		return (-1);
	return (0);
}
