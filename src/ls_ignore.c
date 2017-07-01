/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_ignore.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 10:52:33 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 10:54:00 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
* Traverse() walks the logical directory structure specified by the argv list
* in the order specified by the mastercmp() comparison function.  During the
* traversal it passes linked lists of structures to display() which represent
* a superset (may be exact set) of the files to be displayed.
*/

static void		traverse(int argc, char *argv[], int options)
{
	FTS *ftsp;
	FTSENT *p, *chp;
	int ch_options, error;

	if ((ftsp =
				fts_open(argv, options, g_ls_flg->nosort ? NULL : mastercmp)) == NULL)
		err(1, "fts_open");

	display(NULL, fts_children(ftsp, 0));
	if (g_ls_flg->listdir) {
		fts_close(ftsp);
		return;
	}

	/*
	* If not recursing down this tree and don't need stat info, just get
	* the names.
	*/
	ch_options = !g_ls_flg->recursive && options & FTS_NOSTAT ? FTS_NAMEONLY : 0;

	while ((p = fts_read(ftsp)) != NULL)
		switch (p->fts_info) {
			case FTS_DC:
				warnx("%s: directory causes a cycle", p->fts_name);
				if (COMPAT_MODE("bin/ls", "Unix2003")) {
					rval = 1;
				}
				break;
			case FTS_DNR:
			case FTS_ERR:
				warnx("%s: %s", p->fts_name, strerror(p->fts_errno));
				rval = 1;
				break;
			case FTS_D:
				if (p->fts_level != FTS_ROOTLEVEL &&
						p->fts_name[0] == '.' && !g_ls_flg->listdot)
					break;

				/*
				* If already output something, put out a newline as
				* a separator.  If multiple arguments, precede each
				* directory with its name.
				*/

				if (output)
					(void)printf("\n%s:\n", p->fts_path);
				else if (argc > 1) {
					(void)printf("%s:\n", p->fts_path);
					output = 1;
				}
				chp = fts_children(ftsp, ch_options);
				if (COMPAT_MODE("bin/ls", "Unix2003") && ((options & FTS_LOGICAL)!=0)) {
					FTSENT *curr;
					for (curr = chp; curr; curr = curr->fts_link) {
						if (curr->fts_info == FTS_SLNONE)
							curr->fts_number = NO_PRINT;
					}
				}
				display(p, chp);

				if (!g_ls_flg->recursive && chp != NULL)
					(void)fts_set(ftsp, p, FTS_SKIP);
				break;
			case FTS_SLNONE:	/* Same as default unless Unix conformance */
				if (COMPAT_MODE("bin/ls", "Unix2003")) {
					if ((options & FTS_LOGICAL)!=0) {	/* -L was specified */
						warnx("%s: %s", p->fts_name, strerror(p->fts_errno ?: ENOENT));
						rval = 1;
					}
				}
				break;
			default:
				break;
		}
	error = errno;
	fts_close(ftsp);
	errno = error;

	if (errno)
		err(1, "fts_read");
}

/*
* Display() takes a linked list of FTSENT structures and passes the list
* along with any other necessary information to the print function.  P
* points to the parent directory of the display list.
*/

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
**/