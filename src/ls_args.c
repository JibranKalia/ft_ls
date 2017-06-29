/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_args.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 09:07:07 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/29 09:07:45 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern int8_t	g_ls_flags;

int8_t		ft_ls_handle_args(int i, int argc, char **argv)
{
	t_arr		*naf;
	t_arr		*dir;
	t_arr		*naf;
	t_files		*tmp;
	int			i;

	files = arr_create(sizeof(t_ls_file), 10);
	dir = arr_create(sizeof(t_ls_file), 10);
	naf = arr_create(sizeof(t_ls_file), 10);

	while (++i < argc)
	{
		if (stat(argv[i], tmp->statinfo) == -1)
		{
			files->name = ft_strdup();
		}

		if (S_IFDIR(tmp-statinfo.st_mode))
	}
	return (0);
}
