/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_col.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 14:40:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 10:14:46 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern int		g_ls_flags;

static void		get_col_info(t_ls_file **tmp, t_col *col)
{
	int			i;
	char		*p;

	col->max_len = 0;
	col->tabwidth = 8;
	col->termwidth = 80;
	i = -1;
	while (++i < col->file_count)
		col->max_len = MAX(col->max_len, (int)ft_strlen(tmp[i]->name));
	col->colwidth = col->max_len;
	if ((p = getenv("COLUMNS")) != NULL && *p != '\0')
		col->termwidth = ft_atoi(p);
	else if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &col->win) != -1 && col->win.ws_col > 0)
		col->termwidth = col->win.ws_col;
	col->colwidth = (col->colwidth + col->tabwidth) & ~(col->tabwidth - 1);
	col->numcol = (col->termwidth / (col->colwidth));
	col->numrow = col->file_count / col->numcol;
	if (col->file_count % col->numcol)
		++col->numrow;
}


int8_t			ls_print_col(t_arr *files)
{
	t_col				*col;
	int					row;
	int					column;
	int					base;

	col = ft_memalloc(sizeof(t_col));
	MEMCHECK(col);
	col->sortacross = (g_ls_flags & FLG_x) ? 1 : 0;
	col->file_count = files->end;
	get_col_info((t_ls_file **)files->contents, col);
	base = 0;
	row = -1;
	while(++row < col->numrow)
	{
		col->endcol = col->colwidth;
		if (!col->sortacross)
			base = row;
		column = -1;
		col->chcnt = 0;
		while (++column < col->numcol)
		{
			col->chcnt += ft_printf("%s", ((t_ls_file*)files->contents[base])->name);
			if (col->sortacross)
				base++;
			else
				base += col->numrow;
			if (base >= col->file_count)
				break;
			while ((col->cnt = ((col->chcnt + col->tabwidth) & ~(col->tabwidth - 1)))
					<= col->endcol)
			{
				if (col->sortacross && column + 1 >= col->numcol)
					break;
				write(1, "\t", 1);
				col->chcnt = col->cnt;
			}
			col->endcol += col->colwidth;
		}
		write(1, "\n", 1);
	}
	free(col);
	return (0);
}

int8_t				ls_print_simple(t_arr *files)
{
	t_ls_file	**tmp;
	int			i;

	tmp = (t_ls_file **)files->contents;
	i = -1;
	while (++i < files->end)
		ft_printf("%s\n", tmp[i]->name);
	return (0);
}

static void		display(FTSENT *p, FTSENT *list)
{
	struct stat *sp;
	DISPLAY d;
	FTSENT *cur;
	NAMES *np;
	off_t maxsize;
	u_int64_t btotal, maxblock;
	u_long lattrlen, maxlen, maxnlink, maxlattr;
	ino_t maxinode;
	int bcfile, maxflags;
	gid_t maxgroup;
	uid_t maxuser;
	size_t flen, ulen, glen;
	char *initmax;
	int entries, needstats;
	const char *user, *group;
	char *flags, *lattr = NULL;
	char buf[STRBUF_SIZEOF(u_quad_t) + 1];
	char ngroup[STRBUF_SIZEOF(uid_t) + 1];
	char nuser[STRBUF_SIZEOF(gid_t) + 1];

	/*
	* If list is NULL there are two possibilities: that the parent
	* directory p has no children, or that fts_children() returned an
	* error.  We ignore the error case since it will be replicated
	* on the next call to fts_read() on the post-order visit to the
	* directory p, and will be signaled in traverse().
	*/

	if (list == NULL)
		return;

	needstats = g_ls_flg->inode || g_ls_flg->longform || g_ls_flg->size;
	flen = 0;
	btotal = 0;
	initmax = getenv("LS_COLWIDTHS");
	/* Fields match -lios order.  New ones should be added at the end. */
	maxlattr = maxblock = maxinode = maxlen = maxnlink =
		maxuser = maxgroup = maxflags = maxsize = 0;
	if (initmax != NULL && *initmax != '\0') {
		char *initmax2, *jinitmax;
		int ninitmax;

		/* Fill-in "::" as "0:0:0" for the sake of scanf. */
		jinitmax = initmax2 = malloc(strlen(initmax) * 2 + 2);
		if (jinitmax == NULL)
			err(1, "malloc");
		if (*initmax == ':')
			strcpy(initmax2, "0:"), initmax2 += 2;
		else
			*initmax2++ = *initmax, *initmax2 = '\0';
		for (initmax++; *initmax != '\0'; initmax++) {
			if (initmax[-1] == ':' && initmax[0] == ':') {
				*initmax2++ = '0';
				*initmax2++ = initmax[0];
				initmax2[1] = '\0';
			} else {
				*initmax2++ = initmax[0];
				initmax2[1] = '\0';
			}
		}
		if (initmax2[-1] == ':')
			strcpy(initmax2, "0");
		ninitmax = sscanf(jinitmax,
#if _DARWIN_FEATURE_64_BIT_INODE
				" %llu : %qu : %lu : %i : %i : %i : %qu : %lu : %lu ",
#else
				" %lu : %qu : %lu : %i : %i : %i : %qu : %lu : %lu ",
#endif
				&maxinode, &maxblock, &maxnlink, &maxuser,
				&maxgroup, &maxflags, &maxsize, &maxlen, &maxlattr);
		g_ls_flg->notabs = 1;
		switch (ninitmax) {
			case 0:
				maxinode = 0;
				/* FALLTHROUGH */
			case 1:
				maxblock = 0;
				/* FALLTHROUGH */
			case 2:
				maxnlink = 0;
				/* FALLTHROUGH */
			case 3:
				maxuser = 0;
				/* FALLTHROUGH */
			case 4:
				maxgroup = 0;
				/* FALLTHROUGH */
			case 5:
				maxflags = 0;
				/* FALLTHROUGH */
			case 6:
				maxsize = 0;
				/* FALLTHROUGH */
			case 7:
				maxlen = 0;
				/* FALLTHROUGH */
			case 8:
				maxlattr = 0;
				/* FALLTHROUGH */
#ifdef COLORLS
				if (!g_ls_flg->color)
#endif
					g_ls_flg->notabs = 0;
				/* FALLTHROUGH */
			default:
				break;
		}
		maxinode = makenines(maxinode);
		maxblock = makenines(maxblock);
		maxnlink = makenines(maxnlink);
		maxsize = makenines(maxsize);
	}
	bcfile = 0;
	flags = NULL;
	for (cur = list, entries = 0; cur; cur = cur->fts_link) {
		if (cur->fts_info == FTS_ERR || cur->fts_info == FTS_NS) {
			warnx("%s: %s",
					cur->fts_name, strerror(cur->fts_errno));
			cur->fts_number = NO_PRINT;
			rval = 1;
			continue;
		}
		/*
		* P is NULL if list is the argv list, to which different rules
		* apply.
		*/

		if (p == NULL) {
			/* Directories will be displayed later. */
			if (cur->fts_info == FTS_D && !g_ls_flg->listdir) {
				cur->fts_number = NO_PRINT;
				continue;
			}
		} else {
			/* Only display dot file if -a/-A set. */
			if (cur->fts_name[0] == '.' && !g_ls_flg->listdot) {
				cur->fts_number = NO_PRINT;
				continue;
			}
		}
		if (cur->fts_namelen > maxlen)
			maxlen = cur->fts_namelen;
		if (g_ls_flg->octal || g_ls_flg->octal_escape) {
			u_long t = len_octal(cur->fts_name, cur->fts_namelen);

			if (t > maxlen)
				maxlen = t;
		}
		if (needstats) {
			sp = cur->fts_statp;
			if (sp->st_blocks > maxblock)
				maxblock = sp->st_blocks;
			if (sp->st_ino > maxinode)
				maxinode = sp->st_ino;
			if (sp->st_nlink > maxnlink)
				maxnlink = sp->st_nlink;
			if (sp->st_size > maxsize)
				maxsize = sp->st_size;

			btotal += sp->st_blocks;
			if (g_ls_flg->longform) {
				if (g_ls_flg->numericonly) {
					(void)snprintf(nuser, sizeof(nuser),
							"%u", sp->st_uid);
					(void)snprintf(ngroup, sizeof(ngroup),
							"%u", sp->st_gid);
					user = nuser;
					group = ngroup;
				} else {
					user = user_from_uid(sp->st_uid, 0);
					group = group_from_gid(sp->st_gid, 0);
				}
				if ((ulen = strlen(user)) > maxuser)
					maxuser = ulen;
				if ((glen = strlen(group)) > maxgroup)
					maxgroup = glen;
				if (g_ls_flg->flags) {
					flags = fflagstostr(sp->st_flags);
					if (flags != NULL && *flags == '\0') {
						free(flags);
						flags = strdup("-");
					}
					if (flags == NULL)
						err(1, "fflagstostr");
					flen = strlen(flags);
					if (flen > (size_t)maxflags)
						maxflags = flen;
				} else
					flen = 0;
				lattr = NULL;
				lattrlen = 0;

				if ((np = malloc(sizeof(NAMES) + lattrlen +
								ulen + glen + flen + 4)) == NULL)
					err(1, "malloc");

				np->user = &np->data[0];
				(void)strcpy(np->user, user);
				np->group = &np->data[ulen + 1];
				(void)strcpy(np->group, group);

				if (S_ISCHR(sp->st_mode) ||
						S_ISBLK(sp->st_mode))
					bcfile = 1;

				if (g_ls_flg->flags) {
					np->flags = &np->data[ulen + glen + 2];
					(void)strcpy(np->flags, flags);
					free(flags);
				}
				cur->fts_pointer = np;
			}
		}
		++entries;
	}

	if (!entries)
		return;

	d.list = list;
	d.entries = entries;
	d.maxlen = maxlen;
	if (needstats) {
		d.bcfile = bcfile;
		d.btotal = btotal;
		(void)snprintf(buf, sizeof(buf), "%qu", (u_int64_t)maxblock);
		d.s_block = strlen(buf);
		d.s_flags = maxflags;
		d.s_lattr = maxlattr;
		d.s_group = maxgroup;
#if _DARWIN_FEATURE_64_BIT_INODE
		(void)snprintf(buf, sizeof(buf), "%llu", maxinode);
#else
		(void)snprintf(buf, sizeof(buf), "%lu", maxinode);
#endif
		d.s_inode = strlen(buf);
		(void)snprintf(buf, sizeof(buf), "%lu", maxnlink);
		d.s_nlink = strlen(buf);
		(void)snprintf(buf, sizeof(buf), "%qu", (u_int64_t)maxsize);
		d.s_size = strlen(buf);
		d.s_user = maxuser;
	}
	printfcn(&d);
	output = 1;

	if (g_ls_flg->longform)
		for (cur = list; cur; cur = cur->fts_link)
			free(cur->fts_pointer);
}
