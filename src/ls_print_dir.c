/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_dir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/01 17:09:48 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 11:01:32 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern int	g_ls_flags;

int8_t		ls_get_dir(t_arr *files, char *path)
{
	t_dir		*dp;
	t_ls	*tmp;
	int			chk;
	DIR			*dirp;

	dirp = opendir(path);
	CHECK(dirp == NULL, RETURN(-1), "Open Dir Failed");
	while ((dp = readdir(dirp)) != 0)
	{
		if (dp->d_name[0] == '.' && !(g_ls_flags & FLG_a))
			continue ;
		tmp = ft_memalloc(sizeof(t_ls));
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
		if (S_ISDIR(((t_ls *)files->contents[i])->statinfo.st_mode)
				&& !(ft_strcmp(((t_ls *)files->contents[i])->name, ".") == 0
					|| ft_strcmp(((t_ls *)files->contents[i])->name, "..") == 0))
		{
			ft_printf("\n%s:\n", ((t_ls *)files->contents[i])->path);
			ls_print_dir(((t_ls *)files->contents[i])->path);
		}
	}
}

int8_t			ls_print_dir(char *path)
{
	t_arr		*files;
	int		i;
	t_ls	**tmp;

	files = arr_create(sizeof(t_ls), 5);
	MEMCHECK(files);
	files->del = &file_del;
	if (ls_get_dir(files, path) == -1)
	{
		ft_dprintf(2, "ls: %s: %s\n", get_basename(path), strerror(errno));
		arr_del(files);
		return (-1);
	}
	CHECK1(files->end == 0, arr_del(files), RETURN (-1), "Get Dir Failed");
	tmp = (t_ls **)files->contents;
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
