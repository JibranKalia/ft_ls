/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_args.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 09:07:07 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/29 10:14:55 by jkalia           ###   ########.fr       */
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

static int8_t	init_arg_array(t_arr *naf, t_arr *dir, t_arr *fil)
{
	fil = arr_create(sizeof(t_ls_file), 10);
	MEMCHECK(fil);
	dir = arr_create(sizeof(t_ls_file), 10);
	MEMCHECK(dir);
	naf = arr_create(sizeof(t_ls_file), 10);
	MEMCHECK(naf);
	fil->del = &file_del;
	naf->del = &file_del;
	dir->del = &file_del;
	return (0);
}

int8_t			ls_handle_args(int i, int argc, char **argv)
{
	t_arr			*naf;
	t_arr			*dir;
	t_arr			*fil;
	t_ls_file		*tmp;

	DEBUG("argv[%d] = %s", i, argv[i]);

	if (init_arg_array(naf, dir, fil) == -1)
		return (-1);

	tmp = arr_new(dir);
	MEMCHECK(tmp);
	while (++i < argc)
	{
		tmp->path = ft_strdup(argv[i]);
		tmp->name = ft_strdup(get_basename(argv[i]));
		if (stat(argv[i], &tmp->statinfo) == -1)
		{
			arr_push(naf, tmp);
			free(tmp);
			continue;
		}
		if (S_ISDIR(tmp->statinfo.st_mode))
		{
			DEBUG("HERE1");
			arr_push(dir, tmp);
		}
		else
		{
			DEBUG("HERE2");
			arr_push(fil, tmp);
		}
	}
	//handle_files(fil);
	arr_destroy(dir);
	arr_destroy(naf);
	arr_destroy(fil);
	return (0);
}
