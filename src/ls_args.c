/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_args.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 09:07:07 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/29 09:40:54 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern int8_t	g_ls_flags;

int8_t		ls_handle_args(int i, int argc, char **argv)
{
	t_arr			*naf;
	t_arr			*dir;
	t_arr			*fil;
	t_ls_file		*tmp;

	tmp = ft_memalloc(sizeof(t_ls_file));
	fil = arr_create(sizeof(t_ls_file), 10);
	dir = arr_create(sizeof(t_ls_file), 10);
	naf = arr_create(sizeof(t_ls_file), 10);
	fil->del = file_del;
	naf->del = file_del;
	dir->del = file_del;

	while (++i < argc)
	{
		tmp->path = ft_strdup(argv[i]);
		tmp->name = ft_strdup(get_basename(argv[i]));
		if (stat(argv[i], &tmp->statinfo) == -1)
		{
			arr_push(naf, tmp);
		}
		if (S_ISDIR(tmp->statinfo.st_mode))
		{
			arr_push(dir, tmp);
		}
		else
		{
			arr_push(fil, tmp);
		}
	}
	arr_destroy(dir);
	arr_destroy(naf);
	arr_destroy(fil);
	return (0);
}
