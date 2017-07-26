/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_begin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 06:00:08 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/25 18:48:26 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

t_ls_flg		g_ls_flg;
t_ls_data		g_ls_data;
static int8_t	(*g_printfcn)(t_arr *);

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
	if (g_ls_flg.singlecol)
		g_printfcn = ls_printscol;
	else if (g_ls_flg.longform)
		g_printfcn = ls_printlong;
	//else if (g_ls_flg.stream)
	//	g_ls_data.printfcn = printstream;
	else
		g_printfcn = ls_printcol;
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

