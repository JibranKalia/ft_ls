/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_traverse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 09:07:07 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/25 19:10:18 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern t_ls_flg			g_ls_flags;
extern t_ls_data		g_ls_data;

/**
static void		print_custom_l(t_arr *fil)
{
	int		i;
	int		padding[4];

	ft_bzero(padding, sizeof(int) * 4);
	i = -1;
	while (++i < fil->end)
		find_padding(padding, ((t_ls *)fil->contents[i])->statinfo);
	i = -1;
	while (++i < fil->end)
	{
		print_long((t_ls *)fil->contents[i], padding);
		write(1, "\n", 1);
	}
	write(1, "\n", 1);
}
**/

static void		handle_files(t_arr *fil)
{
	
	if (fil->end > 0)
		ls_sort(fil);
	g_ls_data.printfcn(fil);
	/**
	if (g_ls_flg.longform == 1)
		print_custom_l(fil);
	else if (g_ls_flg.singlecol = 1)
		ls_print_simple(fil);
	else
		ls_print_col(fil);
	**/
}

static void		handle_dir(t_arr *dir)
{
	int		i;

	if (dir->end == 0)
		return ;
	if (dir->end == 1)
	{
		ls_print_dir(((t_ls *)dir->contents[0])->path);
		return ;
	}
	i = 0;
	while (i < dir->end)
	{
		ft_printf("%s:\n", ((t_ls *)dir->contents[i])->name);
		ls_print_dir(((t_ls *)dir->contents[i])->path);
		if ((++i) < dir->end)
			write(1, "\n", 1);
	}
}

static void		handle_naf(t_arr *naf)
{
	int		i;

	ls_sort(naf);
	i = -1;
	while (++i < naf->end)
		ft_dprintf(2, "ls: %s: %s\n", ((t_ls *)naf->contents[i])->name, strerror(errno));
}

t_arr*		ls_get_dir(char *path)
{
	t_arr		*files;
	t_dir		*dp;
	t_ls	*tmp;
	int			chk;
	DIR			*dirp;

	files = arr_create(sizeof(t_ls), 10);
	MEMCHECK(files);
	files->del = &file_del;
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

int8_t			ls_traverse(int i, int argc, char **argv)
{
	t_arr			*naf;
	t_arr			*dir;
	t_arr			*fil;
	t_ls		*tmp;

	fil = arr_create(sizeof(t_ls), 10);
	MEMCHECK(fil);
	dir = arr_create(sizeof(t_ls), 10);
	MEMCHECK(dir);
	naf = arr_create(sizeof(t_ls), 10);
	MEMCHECK(naf);
	fil->del = &file_del;
	naf->del = &file_del;
	dir->del = &file_del;

	while (++i < argc)
	{
		tmp = ft_memalloc(sizeof(t_ls));
		MEMCHECK(tmp);
		tmp->path = ft_strdup(argv[i]);
		tmp->name = ft_strdup(get_basename(argv[i]));
		if (stat(argv[i], &tmp->statinfo) == -1)
		{
//			DEBUG("NAF");
			arr_push(naf, tmp);
			continue;
		}
		if (S_ISDIR(tmp->statinfo.st_mode))
		{
//			DEBUG("DIR");
			arr_push(dir, tmp);
		}
		else
		{
//			DEBUG("FILE");
			arr_push(fil, tmp);
		}
	}
	handle_naf(naf);
	handle_files(fil);
	//handle_dir(dir);
	arr_del(dir);
	arr_del(naf);
	arr_del(fil);
	return (0);
}
