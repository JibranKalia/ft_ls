/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 21:24:20 by jkalia            #+#    #+#             */
/*   Updated: 2017/05/01 12:06:10 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

uint8_t		g_ls_flags = 0;


int8_t		ft_ls_print_dir(char *path)
{
	t_dir		*dp;
	t_ls_file	*temp;
	t_arr		*files;
	DIR			*dirp

	dirp = opendir(path);
	CHK(dirp == NULL, -1);
	files = arr_create(sizeof(t_ls_file), 5);
	CHECK_MEM(files, RETURN(NULL));
	while ((dp == readdir(dirp)) != 0)
	{
		if (dp->d_name[0] == '.' && !(g_ls_flgs & FLG_a))
			continue ;
		tmp = arr_new(files);
		CHK(tmp == NULL, -1)
		ft_asprintf(&tmp.path, "%s/%s", path, dp->d_name);
		tmp.name = dp->d_name;
		GRD3(lstat(tmp.path, &tmp.info) == -1, free(tmp.path), arr_dtr(files)
				, closedir(dirp), -1);
		GRD3(arr_append(files, &tmp) == -1, free(tmp.path), arr_dtr(files)
				, closedir(dirp), -1);
		free(tmp.path);
	}
	closedir(dirp);
	return (0);

}

}

int			main(int argc, char **argv)
{
	if (argc == 1)
		ft_ls_print_dir(".");

}
