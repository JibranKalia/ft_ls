/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_traverse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/01 11:40:49 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 11:41:13 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

static void		ls_traverse(int argc, char *argv[], int options)
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
