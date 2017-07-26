/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_handledir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/26 11:03:54 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/26 11:47:52 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern t_ls_flg		g_ls_flg;
extern PRINTLS		g_printfcn;


static int8_t		print_dir(char *path);
static int8_t		get_dir(t_arr *files, char *path)
{
	t_dir		*dp;
	t_ls		*tmp;
	int			chk;
	DIR			*dirp;

	dirp = opendir(path);
	CHECK(dirp == NULL, RETURN(-1), "Open Dir Failed");
	while ((dp = readdir(dirp)) != 0)
	{
		if (dp->d_name[0] == '.' && !(g_ls_flg.seedot))
 		continue ;
		tmp = ft_memalloc(sizeof(t_ls));
		MEMCHECK1(tmp, closedir(dirp));
		ft_asprintf(&tmp->path, "%s/%s", path, dp->d_name);
		tmp->name = ft_strdup(dp->d_name);
		chk = lstat(tmp->path, &tmp->statinfo);
		CHECK2(chk == -1, closedir(dirp), arr_del(files), RETURN(-1), "lstat Failed");
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
			print_dir(((t_ls *)files->contents[i])->path);
		}
	}
}

static int8_t	print_dir(char *path)
{
	t_arr		*files;
	int		i;
	t_ls	**tmp;

	files = arr_create(sizeof(t_ls), 5);
	MEMCHECK(files);
	files->del = &file_del;
	get_dir(files, path);
	CHECK1(files->end == 0, arr_del(files), RETURN (-1), "Get Dir Failed");
	tmp = (t_ls **)files->contents;
	i = -1;
	ls_sort(files);
	if (g_ls_flg.recursive == 1)
		ls_recursive(files);
	g_printfcn(files);
	arr_del(files);
	return (0);
}

int8_t			handle_dir(t_arr *dir)
{
	int		i;

	if (dir->end == 0)
	{
		print_dir(".");
		return (0);
	}
	if (dir->end == 1)
	{
		CHK(print_dir(((t_ls *)dir->contents[0])->path) == -1, -1);
		return (0);
	}
	i = 0;
	while (i < dir->end)
	{
		ft_printf("%s:\n", ((t_ls *)dir->contents[i])->name);
		print_dir(((t_ls *)dir->contents[i])->path);
		if ((++i) < dir->end)
			write(1, "\n", 1);
	}
	return (0);
}
