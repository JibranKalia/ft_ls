/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 21:24:20 by jkalia            #+#    #+#             */
/*   Updated: 2017/05/01 16:13:23 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

uint8_t		g_ls_flags = 0;

void		file_del(void	*elm)
{
	if (elm == 0)
		return ;

	free(((t_ls_file *)elm)->name);
	free(((t_ls_file *)elm)->path);
	ft_bzero(elm, sizeof(t_ls_file));
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
		CHECK2(chk == -1, file_del(&tmp), closedir(dirp), RETURN(-1), "Lstat Failed");
		arr_push(files, tmp);
	}
	closedir(dirp);
	return (0);
}

int8_t		ft_ls_print_dir(char *path)
{
	t_arr		*files;
	int			chk;
	size_t		i;
	t_ls_file	**tmp;

	i = 0;
	files = arr_create(sizeof(t_ls_file), 100);
	CHECK(files == NULL, RETURN(-1), "Array Create Failed");
	chk = ft_ls_get_dir(files, path);
	CHECK1((chk == -1 || files->end == 0), arr_del(files), RETURN (-1), "Get Dir Failed");
	tmp = (t_ls_file **)files->contents;
	while (i < files->end)
		printf("%s\n", tmp[i++]->name);
	return (0);
}

int			main(int argc, char **argv)
{
	if (argc == 1)
		ft_ls_print_dir(".");
	(void)argv;
	return (0);
}
