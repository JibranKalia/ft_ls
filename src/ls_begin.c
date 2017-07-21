/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_begin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 06:00:08 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/20 18:05:31 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

t_ls_data		g_data;
t_ls_flg		g_ls_flg;


static int8_t	handle_flag(int c)
{
	int notused;
	if (c == '1')
	{
		g_ls_flg.singlecol = 1;
		g_ls_flg.longform = 0;
		g_ls_flg.stream = 0;
	}
	else if (c == 'B')
		g_ls_flg.nonprint = 0;
	else if (c == 'C')
		g_ls_flg.sortacross = g_ls_flg.longform = g_ls_flg.singlecol = 0;
	else if (c == 'l')
	{
		g_ls_flg.longform = 1;
		g_ls_flg.singlecol = 0;
		g_ls_flg.stream = 0;
	}
	else if (c == 'x')
	{
		g_ls_flg.sortacross = 1;
		g_ls_flg.longform = 0;
		g_ls_flg.singlecol = 0;
	}
		/* The -c and -u options override each other. */
	else if (c == 'c')
	{
		g_ls_flg.statustime = 1;
		g_ls_flg.accesstime = g_ls_flg.birthtime = 0;
	}
	else if (c == 'u')
	{
		g_ls_flg.accesstime = 1;
		g_ls_flg.statustime = g_ls_flg.birthtime = 0;
	}
	else if (c == 'U')
	{
		g_ls_flg.birthtime = 1;
		g_ls_flg.statustime = g_ls_flg.accesstime = 0;
	}
	else if (c == 'F')
	{
		g_ls_flg.type = 1;
		g_ls_flg.slash = 0;
	}
	else if (c == 'R')
		g_ls_flg.recursive = 1;
	else if (c == 'a')
		g_ls_flg.seedot = 1;
	else if (c == 'A')
		g_ls_flg.listdot = 1;
		/* The -d option turns off the -R option. */
	else if (c == 'd')
	{
		g_ls_flg.listdir = 1;
		g_ls_flg.recursive = 0;
	}
	else if (c == 'f')
		g_ls_flg.nosort = 1;
	else if (c == 'h')
		g_ls_flg.humanval = 1;
	else if (c == 'i')
		g_ls_flg.inode = 1;
	else if (c == 'k')
		g_ls_flg.kblocks = 1;
	else if (c == 'm')
	{
		g_ls_flg.stream = 1;
		g_ls_flg.singlecol = 0;
		g_ls_flg.longform = 0;
	}
	else if (c == 'n')
	{
		g_ls_flg.numericonly = 1;
		g_ls_flg.longform = 1;
		g_ls_flg.singlecol = 0;
		g_ls_flg.stream = 0;
	}
	else if (c == 'o')
	{
		g_ls_flg.owner = 1;
		g_ls_flg.longform = 1;
		g_ls_flg.singlecol = 0;
		g_ls_flg.stream = 0;
	}
	else if (c == 'p')
	{
		g_ls_flg.slash = 1;
		g_ls_flg.type = 1;
	}
	else if (c == 'r')
		g_ls_flg.reversesort = 1;
	else if (c == 'S')
		g_ls_flg.sizesort = 1;
	else if (c == 's')
		g_ls_flg.size = 1;
	else if (c == 'T')
		g_ls_flg.sectime = 1;
	else if (c == 't')
		g_ls_flg.timesort = 1;
	else if (c == 'W')
		g_ls_flg.whiteout = 1;
	else if (c == 'v')
		g_ls_flg.nonprint = 0;
	else if (c == 'b')
		g_ls_flg.nonprint = 0;
	else if (c == 'w')
		g_ls_flg.nonprint = 0;
	else if (c == 'e')
		g_ls_flg.acl = 1;
	else if (c == '@')
		g_ls_flg.xattr = 1;
	else if (c == 'O')
		g_ls_flg.flags = 1;
	else
		ls_usage(0);
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
	return (0);
}


int		main(int argc, char **argv)
{

	if ((parse(argc, argv)) == -1)
		return (-1);

	/**
	// Select a print function.
	if (g_ls_flg.singlecol)
		printfcn = printscol;
	else if (g_ls_flg.longform)
		printfcn = printlong;
	else if (g_ls_flg.stream)
		printfcn = printstream;
	else
		printfcn = printcol;

	if (argc)
		traverse(argc, argv, fts_options);
	else
		traverse(1, dotav, fts_options);
	exit(rval);
	**/
}
