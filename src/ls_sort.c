/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_sort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/18 20:24:36 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/25 19:04:54 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
extern t_ls_flg		g_ls_flg;

static double		timespec_diff(struct timespec *start, struct timespec *stop)
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

static inline void	which_time(void *a, void *b)
{
	if (g_ls_flg.statustime == 1)
	{
		((t_ls *)(a))->lstime = ((t_ls *)(a))->statinfo.st_ctimespec;
		((t_ls *)(b))->lstime = ((t_ls *)(b))->statinfo.st_ctimespec;
	}
	else if (g_ls_flg.birthtime == 1)
	{
		((t_ls *)(a))->lstime = ((t_ls *)(a))->statinfo.st_birthtimespec;
		((t_ls *)(b))->lstime = ((t_ls *)(b))->statinfo.st_birthtimespec;
	}
	else if (g_ls_flg.accesstime == 1)
	{
		((t_ls *)(a))->lstime = ((t_ls *)(a))->statinfo.st_atimespec;
		((t_ls *)(b))->lstime = ((t_ls *)(b))->statinfo.st_atimespec;
	}
	else
	{
		((t_ls *)(a))->lstime = ((t_ls *)(a))->statinfo.st_mtimespec;
		((t_ls *)(b))->lstime = ((t_ls *)(b))->statinfo.st_mtimespec;
	}
}

int				ls_namecmp(void *a, void *b)
{
	return (ft_strcmp(((t_ls *)(a))->path, ((t_ls *)(b))->path));
}

static int		ls_timecmp(void *a, void *b)
{
	double		diff;

	which_time(a, b);
	diff = timespec_diff(&((t_ls *)(a))->lstime, &((t_ls *)(b))->lstime);
	if (diff == 0)
			return (ft_strcmp(((t_ls *)(a))->path, ((t_ls *)(b))->path));
	return (diff);
}

void			ls_sort(t_arr *files)
{
	if (g_ls_flg.nosort == 1)
		return ;
	if (g_ls_flg.timesort == 1)
		ft_qsort(files->contents, 0, ARR_COUNT(files) - 1, ls_timecmp);
	else
		ft_qsort(files->contents, 0, ARR_COUNT(files) - 1, ls_namecmp);
	if (g_ls_flg.reversesort == 1)
		arr_reverse(files);
}

/**
static int		ls_sort(void)
{
	// Select a sort function.
	if (g_ls_flg->reversesort)
	{
		if (g_ls_flg->sizesort)
			g_sortfcn = revsizecmp;
		else if (!g_ls_flg->timesort)
			g_sortfcn = ls_revnamecmp;
		else if (g_ls_flg->accesstime)
			g_sortfcn = revacccmp;
		else if (g_ls_flg->statustime)
			g_sortfcn = revstatcmp;
		else if (g_ls_flg->birthtime)
			g_sortfcn = revbirthcmp;
		else		// Use modification time.
			g_sortfcn = revmodcmp;
	}
	else
	{
		if (g_ls_flg->sizesort)
			g_sortfcn = sizecmp;
		else if (!g_ls_flg->timesort)
			g_sortfcn = ls_namecmp;
		else if (g_ls_flg->accesstime)
			g_sortfcn = acccmp;
		else if (g_ls_flg->statustime)
			g_sortfcn = statcmp;
		else if (g_ls_flg->birthtime)
			g_sortfcn = birthcmp;
		else		// Use modification time.
			g_sortfcn = modcmp;
	}
}
**/
