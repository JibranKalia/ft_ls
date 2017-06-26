/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_sort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/18 20:24:36 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/25 23:03:28 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
extern uint8_t			g_ls_flags;

static int	ls_lexcmp(void *a, void *b)
{
	return (ft_strcmp(((t_ls_file *)(a))->path, ((t_ls_file *)(b))->path));
}

void		ls_sort(t_arr *files)
{
	ft_qsort(files->contents, 0, ARR_COUNT(files) - 1, ls_lexcmp);
	if (g_ls_flags & FLG_r)
		arr_reverse(files);
}
