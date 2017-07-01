/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_sort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/18 20:24:36 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/30 17:09:43 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
extern int	g_ls_flags;

static void	which_time(void *a, void *b)
{
	((t_ls_file *)(a))->lstime = (((t_ls_file *)(a))->statinfo.st_mtimespec);
	((t_ls_file *)(b))->lstime = (((t_ls_file *)(b))->statinfo.st_mtimespec);
}
static int	ls_lexcmp(void *a, void *b)
{
	return (ft_strcmp(((t_ls_file *)(a))->path, ((t_ls_file *)(b))->path));
}

static int	ls_timecmp(void *a, void *b)
{
	intmax_t	diff;
	intmax_t	ldiff;

	which_time(a, b);
	diff = ((t_ls_file *)(a))->lstime.tv_sec - ((t_ls_file *)(b))->lstime.tv_sec;
	if (diff == 0)
	{
		ldiff = ((t_ls_file *)(a))->lstime.tv_nsec - ((t_ls_file *)(b))->lstime.tv_nsec;
		//if (ldiff == 0)
		//	return (ft_strcmp(((t_ls_file *)(a))->path, ((t_ls_file *)(b))->path));
		return (ldiff);
	}
	return (diff);
}

void		ls_sort(t_arr *files)
{
	if (g_ls_flags & FLG_f)
		return ;
	if (g_ls_flags & FLG_t)
		ft_qsort(files->contents, 0, ARR_COUNT(files) - 1, ls_timecmp);
	else
		ft_qsort(files->contents, 0, ARR_COUNT(files) - 1, ls_lexcmp);
	if (g_ls_flags & FLG_r)
		arr_reverse(files);
}
