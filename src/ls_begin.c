/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_begin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 06:00:08 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/20 19:14:12 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

t_ls_flg		g_ls_flg;

int8_t			handle_flag(int c)
{
	if (c == '1')
	{
		g_ls_flg.singlecol = 1;
		g_ls_flg.longform = 0;
		g_ls_flg.stream = 0;
	}
	else if (c == 'B')
		g_ls_flg.nonprint = 0;
	else if (c == 'C')
	{
		g_ls_flg.sortacross = 0;
		g_ls_flg.longform = 0;
		g_ls_flg.singlecol = 0;
	}
	else if (c == 'x')
	{
		g_ls_flg.sortacross = 1;
		g_ls_flg.longform = 0;
		g_ls_flg.singlecol = 0;
	}
	else
		return (handle_flag_1(c));
	return (0);
}

static int8_t	parse(int argc, char **argv)
{
	int		i;
	int		j;

	int notused;
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

	ls_traverse(i, argc, argv);
	return (0);
}

int				main(int argc, char **argv)
{

	if ((parse(argc, argv)) == -1)
		return (-1);
	return (0);
}

/**
	if (g_ls_flg.longform || g_ls_flg.size)
	{
		if (g_ls_flg.kblocks)
			g_data.blocksize = 2;
		else
		{
			(void)getbsize(&notused, &g_data.blocksize);
			g_data.blocksize /= 512;
		}
	}
**/

/**
	if (g_ls_flg.singlecol)
		printfcn = printscol;
	else if (g_ls_flg.longform)
		printfcn = printlong;
	else if (g_ls_flg.stream)
		printfcn = printstream;
	else
		printfcn = printcol;
	**/
