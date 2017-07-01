/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_begin.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 06:00:08 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 10:24:48 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

t_ls_data		g_data;
t_ls_flags		g_ls_flg;

int		main(int argc, char **argv)
{
	int		ch;
	int		notused;

	//TODO: Use ft_getopt
	while ((ch = getopt(argc, argv, "1@ABCFGHLOPRSTUWabcdefghiklmnopqrstuvwx"))
			!= -1) {
		switch (ch) {
			/*
			* The -1, -C, -x and -l options all override each other so
			* shell aliasing works right.
			*/
			case '1':
				g_ls_flg->singlecol = 1;
				g_ls_flg->longform = 0;
				g_ls_flg->stream = 0;
				break;
			case 'B':
				g_ls_flg->nonprint = 0;
				g_ls_flg->octal = 1;
				g_ls_flg->octal_escape = 0;
				break;
			case 'C':
				g_ls_flg->sortacross = g_ls_flg->longform = g_ls_flg->singlecol = 0;
				break;
			case 'l':
				g_ls_flg->longform = 1;
				g_ls_flg->singlecol = 0;
				g_ls_flg->stream = 0;
				break;
			case 'x':
				g_ls_flg->sortacross = 1;
				g_ls_flg->longform = 0;
				g_ls_flg->singlecol = 0;
				break;
				/* The -c and -u options override each other. */
			case 'c':
				g_ls_flg->statustime = 1;
				g_ls_flg->accesstime = g_ls_flg->birthtime = 0;
				break;
			case 'u':
				g_ls_flg->accesstime = 1;
				g_ls_flg->statustime = g_ls_flg->birthtime = 0;
				break;
			case 'U':
				g_ls_flg->birthtime = 1;
				g_ls_flg->statustime = g_ls_flg->accesstime = 0;
				break;
			case 'F':
				g_ls_flg->type = 1;
				g_ls_flg->slash = 0;
				break;
			case 'G':
				setenv("CLICOLOR", "", 1);
				break;
			case 'R':
				g_ls_flg->recursive = 1;
				break;
			case 'a':
				g_ls_flg->seedot = 1;
			case 'A':
				g_ls_flg->listdot = 1;
				break;
				/* The -d option turns off the -R option. */
			case 'd':
				g_ls_flg->listdir = 1;
				g_ls_flg->recursive = 0;
				break;
			case 'f':
				g_ls_flg->nosort = 1;
			case 'h':
				g_ls_flg->humanval = 1;
				break;
			case 'i':
				g_ls_flg->inode = 1;
				break;
			case 'k':
				g_ls_flg->kblocks = 1;
				break;
			case 'm':
				g_ls_flg->stream = 1;
				g_ls_flg->singlecol = 0;
				g_ls_flg->longform = 0;
				break;
			case 'n':
				g_ls_flg->numericonly = 1;
				g_ls_flg->longform = 1;
				g_ls_flg->singlecol = 0;
				g_ls_flg->stream = 0;
				break;
			case 'o':
				g_ls_flg->owner = 1;
				g_ls_flg->longform = 1;
				g_ls_flg->singlecol = 0;
				g_ls_flg->stream = 0;
				break;
			case 'p':
				g_ls_flg->slash = 1;
				g_ls_flg->type = 1;
				break;
			case 'r':
				g_ls_flg->reversesort = 1;
				break;
			case 'S':
				g_ls_flg->sizesort = 1;
				break;
			case 's':
				g_ls_flg->size = 1;
				break;
			case 'T':
				g_ls_flg->sectime = 1;
				break;
			case 't':
				g_ls_flg->timesort = 1;
				break;
			case 'W':
				g_ls_flg->whiteout = 1;
				break;
			case 'v':
				g_ls_flg->nonprint = 0;
				break;
			case 'b':
				g_ls_flg->nonprint = 0;
				g_ls_flg->octal = 0;
				g_ls_flg->octal_escape = 1;
				break;
			case 'w':
				g_ls_flg->nonprint = 0;
				g_ls_flg->octal = 0;
				g_ls_flg->octal_escape = 0;
				break;
			case 'e':
				g_ls_flg->acl = 1;
				break;
			case '@':
				g_ls_flg->xattr = 1;
				break;
			case 'O':
				g_ls_flg->flags = 1;
				break;
			default:
			case '?':
				usage();
		}
	}

	// If -l or -s, figure out block size.
	if (g_ls_flg->longform || g_ls_flg->size)
	{
		if (g_ls_flg->kblocks)
			data->blocksize = 2;
		else
		{
			(void)getbsize(&notused, &data->blocksize);
			data->blocksize /= 512;
		}
	}

	/* Select a print function. */
	if (g_ls_flg->singlecol)
		printfcn = printscol;
	else if (g_ls_flg->longform)
		printfcn = printlong;
	else if (g_ls_flg->stream)
		printfcn = printstream;
	else
		printfcn = printcol;

	if (argc)
		traverse(argc, argv, fts_options);
	else
		traverse(1, dotav, fts_options);
	exit(rval);
}
