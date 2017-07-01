/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_sort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/18 20:24:36 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/30 20:05:39 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
extern int	g_ls_flags;

static double	timespec_diff(struct timespec *start, struct timespec *stop)
{
	struct timespec		result;

	if ((stop->tv_nsec - start->tv_nsec) < 0)
	{
		result.tv_sec = stop->tv_sec - start->tv_sec - 1;
		result.tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
	}
	else
	{
		result.tv_sec = stop->tv_sec - start->tv_sec;
		result.tv_nsec = stop->tv_nsec - start->tv_nsec;
	}
	if (result.tv_sec == 0)
		return (result.tv_nsec);
	return (result.tv_sec);
}

static void		which_time(void *a, void *b)
{
	((t_ls_file *)(a))->lstime = (((t_ls_file *)(a))->statinfo.st_mtimespec);
	((t_ls_file *)(b))->lstime = (((t_ls_file *)(b))->statinfo.st_mtimespec);
}

static int		ls_lexcmp(void *a, void *b)
{
	return (ft_strcmp(((t_ls_file *)(a))->path, ((t_ls_file *)(b))->path));
}

static int		ls_timecmp(void *a, void *b)
{
	double		diff;

	which_time(a, b);
	diff = timespec_diff(&((t_ls_file *)(a))->lstime, &((t_ls_file *)(b))->lstime);
	if (diff == 0)
			return (ft_strcmp(((t_ls_file *)(a))->path, ((t_ls_file *)(b))->path));
	return (diff);
}

void			ls_sort(t_arr *files)
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
