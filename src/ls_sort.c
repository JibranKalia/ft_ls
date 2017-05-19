/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_sort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/18 20:24:36 by jkalia            #+#    #+#             */
/*   Updated: 2017/05/18 20:56:17 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

static int	lexcmp(const void *a, const void *b)
{
	return (strcmp(((t_ls_file *)(a))->path, ((t_ls_file *)(b))->path));
}

static int	ls_lexcmp(const void *a, const void *b)
{
	return (ft_strcmp(((t_ls_file *)(a))->path, ((t_ls_file *)(b))->path));
}

void		ls_sort(t_arr *files)
{
	//arr_qsort(files, lexcmp);
	qsort(files->contents, ARR_COUNT(files), sizeof(void *), ls_lexcmp);
}
