/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_handledir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/26 11:03:54 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/27 19:01:13 by jkalia           ###   ########.fr       */
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
		tmp->parameter_type = (S_ISDIR(tmp->statinfo.st_mode)) ? enum_dir : enum_fil;
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
	DEBUG("PRINT DIR");
	files = arr_create(sizeof(t_ls), 5);
	MEMCHECK(files);
	files->del = &file_del;
	get_dir(files, path);
	if (files->end > 0)
		ls_sort(files);
	g_printfcn(files);
	if (g_ls_flg.recursive == 1)
		ls_recursive(files);
	arr_del(files);
	return (0);
}

/**
 * Handle Dir needs the lenghts of naf and fil to determine whether to go into dot
 * 
*/

int8_t			handle_dir(t_arr *dir, int naf_len, int fil_len)
{
	int		i;
	if (dir->end == 0 && fil_len == 0 && naf_len == 0)
	{
		DEBUG("HANDLE DIR DOT");
		print_dir(".");
		return (0);
	}
	if (dir->end == 1)
	{
		DEBUG("HANDLE DIR 1");
		CHK(print_dir(((t_ls *)dir->contents[0])->path) == -1, -1);
		return (0);
	}
	i = 0;
	while (i < dir->end)
	{
		ls_sort(dir);
		DEBUG("HANDLE DIR %d", dir->end);
		ft_printf("%s:\n", ((t_ls *)dir->contents[i])->path);
		print_dir(((t_ls *)dir->contents[i])->path);
		if ((++i) < dir->end)
			write(1, "\n", 1);
	}
	return (0);
}
