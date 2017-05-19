/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_dir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/01 17:09:48 by jkalia            #+#    #+#             */
/*   Updated: 2017/05/18 20:26:24 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern int8_t	g_ls_flags;

char	*ls_basename(char *path)
{
	char *name;

	name = path;
	while (*path)
	{
		if (*path++ == '/')
			name = path;
	}
	return (name);
}

int8_t		ft_ls_get_dir(t_arr *files, char *path)
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
		CHECK1(tmp == NULL, closedir(dirp), RETURN(-1), "Malloc Failed");
		ft_asprintf(&tmp->path, "%s/%s", path, dp->d_name);
		tmp->name = ft_strdup(dp->d_name);
		chk = lstat(tmp->path, &tmp->statinfo);
		CHECK2(chk == -1, closedir(dirp), arr_del(files), RETURN(-1), "Lstat Failed");
		arr_push(files, tmp);
	}
	closedir(dirp);
	return (0);
}

static inline void	ft_ls_recursive(t_arr *files)
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
			ft_ls_print_dir(((t_ls_file *)files->contents[i])->path);
		}
	}
}

int8_t			ft_ls_print_dir(char *path)
{
	t_arr		*files;
	size_t		i;
	t_ls_file	**tmp;

	i = 0;
	files = arr_create(sizeof(t_ls_file), 100);
	CHECK(files == NULL, RETURN(-1), "Array Create Failed");
	files->del = file_del;
	if (ft_ls_get_dir(files, path) == -1)
	{
		ft_dprintf(2, "ls: %s: %s\n", ls_basename(path), strerror(errno));
		arr_del(files);
		return (-1);
	}
	CHECK1(files->end == 0, arr_del(files), RETURN (-1), "Get Dir Failed");
	ls_sort(files);
	tmp = (t_ls_file **)files->contents;
	while (i < files->end)
	{
		ft_printf("NAME = %s\n", tmp[i++]->name);
//		ft_printf("PATH = %s\n", tmp[i++]->path);
	}
	if (g_ls_flags & FLG_R)
		ft_ls_recursive(files);
	arr_del(files);
	return (0);
}
