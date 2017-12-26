/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_begin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 06:00:08 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/26 17:01:19 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

t_ls_flg		g_ls_flg;
t_ls_data		g_data;
PRINTLS			g_printfcn;

static int8_t	parse(int argc, char **argv)
{
	int		i;
	int		j;

	i = 1;
	while (i < argc && argv[i][0] == '-')
		{
		// Double hyphen. insure its not --a
		if (argv[i][1] == '-' && argv[i][2] == 0){
			++i;
			break;
		}
		// if there is nothing following a hypen
		if (argv[i][1] == 0) {
			break;
		}
		j = 1;
		while (argv[i][j] != 0)
		{
			CHK(handle_flag(argv[i][j]) == -1, -1);
			++j;
		}
		++i;
	}
	if (g_ls_flg.singlecol)
		g_printfcn = &ls_printscol;
	else if (g_ls_flg.longform)
		g_printfcn = &ls_printlong;
	//else if (g_ls_flg.stream)
	//	g_ls_data.printfcn = printstream;
	else
		g_printfcn = &ls_printcol;
	ls_traverse(i, argc, argv);
	return (0);
}

/*
**	Terminal defaults to -Cq, non-terminal defaults to -1.
*/

int				main(int argc, char **argv)
{
	if (isatty(STDOUT_FILENO))
	{
		g_data.termwidth = 80;
	//	if ((p = getenv("COLUMNS")) != NULL && *p != '\0')
	//		g_data.termwidth = ft_atoi(p);
		if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &g_data.win) != -1 && g_data.win.ws_col > 0)
			g_data.termwidth = g_data.win.ws_col;
		g_ls_flg.nonprint = 1;
	}
	else
	{
		g_ls_flg.singlecol = 1;
//		p = getenv("COLUMNS");
//		if (p)
//			g_data.termwidth = atoi(p);
	}
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

