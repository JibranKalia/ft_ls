/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_dir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/01 17:09:48 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 10:14:07 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern int	g_ls_flags;

int8_t		ls_get_dir(t_arr *files, char *path)
{
	t_dir		*dp;
	t_ls_file	*tmp;
	int			chk;
	DIR			*dirp;

	dirp = opendir(path);
	CHECK(dirp == NULL, RETURN(-1), "Open Dir Failed");
	while ((dp = readdir(dirp)) != 0)
	{
		if (dp->d_name[0] == '.' && !(g_ls_flags & FLG_a))
			continue ;
		tmp = ft_memalloc(sizeof(t_ls_file));
		MEMCHECK1(tmp, closedir(dirp));
		ft_asprintf(&tmp->path, "%s/%s", path, dp->d_name);
		tmp->name = ft_strdup(dp->d_name);
		chk = lstat(tmp->path, &tmp->statinfo);
		CHECK2(chk == -1, closedir(dirp), arr_del(files), RETURN(-1), "Lstat Failed");
		arr_push(files, tmp);
	}
	closedir(dirp);
	return (0);
}

static inline void	ls_recursive(t_arr *files)
{
	int i;

	i = -1;
	while (++i < files->end)
	{
		if (S_ISDIR(((t_ls_file *)files->contents[i])->statinfo.st_mode)
				&& !(ft_strcmp(((t_ls_file *)files->contents[i])->name, ".") == 0
					|| ft_strcmp(((t_ls_file *)files->contents[i])->name, "..") == 0))
		{
			ft_printf("\n%s:\n", ((t_ls_file *)files->contents[i])->path);
			ls_print_dir(((t_ls_file *)files->contents[i])->path);
		}
	}
}

int8_t			ls_print_dir(char *path)
{
	t_arr		*files;
	int		i;
	t_ls_file	**tmp;

	files = arr_create(sizeof(t_ls_file), 5);
	MEMCHECK(files);
	files->del = &file_del;
	if (ls_get_dir(files, path) == -1)
	{
		ft_dprintf(2, "ls: %s: %s\n", get_basename(path), strerror(errno));
		arr_del(files);
		return (-1);
	}
	CHECK1(files->end == 0, arr_del(files), RETURN (-1), "Get Dir Failed");
	tmp = (t_ls_file **)files->contents;
	i = -1;
	ls_sort(files);
	if (g_ls_flags & FLG_R)
		ls_recursive(files);
	if (g_ls_flags & FLG_l)
		ls_print_l(files);
	else if (g_ls_flags & FLG_1)
		ls_print_simple(files);
	else
		ls_print_col(files);
	arr_del(files);
	return (0);
}

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
