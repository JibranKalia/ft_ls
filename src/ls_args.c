/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_args.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 09:07:07 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/29 14:25:22 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern int8_t	g_ls_flags;

static int8_t	handle_files(t_arr *fil)
{
	int		i;

	i = -1;
	while (++i < fil->end)
	{
			ft_printf("%s\n", ((t_ls_file *)fil)->name);
	}

	return (0);
}

static void		handle_naf(t_arr *naf)
{
	int		i;

	ls_sort(naf);
	i = -1;
	while (++i < naf->end)
		ft_dprintf(2, "ls: %s: %s\n", ((t_ls_file *)naf->contents[i])->name, strerror(errno));
}

int8_t			ls_handle_args(int i, int argc, char **argv)
{
	t_arr			*naf;
	t_arr			*dir;
	t_arr			*fil;
	t_ls_file		*tmp;

	fil = arr_create(sizeof(t_ls_file), 10);
	MEMCHECK(fil);
	dir = arr_create(sizeof(t_ls_file), 10);
	MEMCHECK(dir);
	naf = arr_create(sizeof(t_ls_file), 10);
	MEMCHECK(naf);
	fil->del = &file_del;
	naf->del = &file_del;
	dir->del = &file_del;

	while (++i < argc)
	{
		tmp = ft_memalloc(sizeof(t_ls_file));
	MEMCHECK(tmp);
		tmp->path = ft_strdup(argv[i]);
		tmp->name = ft_strdup(get_basename(argv[i]));
		if (stat(argv[i], &tmp->statinfo) == -1)
		{
			arr_push(naf, tmp);
			continue;
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
	//handle_files(fil);
	handle_naf(naf);
	arr_del(dir);
	arr_del(naf);
	arr_del(fil);
	return (0);
}
