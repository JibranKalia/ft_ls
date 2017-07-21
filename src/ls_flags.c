/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_flags.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/20 18:09:29 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/20 18:27:56 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern t_ls_flg		g_ls_flg;

static int8_t	handle_flag_5(int c)
{
	if (c == 'l')
	{
		g_ls_flg.longform = 1;
		g_ls_flg.singlecol = 0;
		g_ls_flg.stream = 0;
	}
	else if (c == 'c')
	{
		g_ls_flg.statustime = 1;
		g_ls_flg.accesstime = 0;
		g_ls_flg.birthtime = 0;
	}
	else if (c == 'S')
		g_ls_flg.sizesort = 1;
	else if (c == 's')
		g_ls_flg.size = 1;
	else
		return (ls_usage(0));
	return (0);
}

static int8_t	handle_flag_4(int c)
{
	if (c == 'T')
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
		return (handle_flag_5(c));
	return (0);
}

static int8_t	handle_flag_3(int c)
{
	if (c == 'n')
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
	else
		return (handle_flag_4(c));
	return (0);
}

static int8_t	handle_flag_2(int c)
{
	if (c == 'A')
		g_ls_flg.listdot = 1;
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
	else
		return (handle_flag_3(c));
	return (0);
}

int8_t			handle_flag_1(int c)
{
	if (c == 'u')
	{
		g_ls_flg.accesstime = 1;
		g_ls_flg.statustime = 0;
		g_ls_flg.birthtime = 0;
	}
	else if (c == 'U')
	{
		g_ls_flg.birthtime = 1;
		g_ls_flg.statustime = 0;
		g_ls_flg.accesstime = 0;
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
	else
		return (handle_flag_2(c));
	return (0);
}
