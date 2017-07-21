/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_sort.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/18 20:24:36 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 10:56:01 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
extern int			g_ls_flags;

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
	if (g_ls_flags & FLG_c)
	{
		((t_ls *)(a))->lstime = ((t_ls *)(a))->statinfo.st_ctimespec;
		((t_ls *)(b))->lstime = ((t_ls *)(b))->statinfo.st_ctimespec;
	}
	else if (g_ls_flags & FLG_U)
	{
		((t_ls *)(a))->lstime = ((t_ls *)(a))->statinfo.st_birthtimespec;
		((t_ls *)(b))->lstime = ((t_ls *)(b))->statinfo.st_birthtimespec;
	}
	else if (g_ls_flags & FLG_u)
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

static int		ls_lexcmp(void *a, void *b)
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
	if (g_ls_flags & FLG_f)
		return ;
	if (g_ls_flags & FLG_t)
		ft_qsort(files->contents, 0, ARR_COUNT(files) - 1, ls_timecmp);
	else
		ft_qsort(files->contents, 0, ARR_COUNT(files) - 1, ls_lexcmp);
	if (g_ls_flags & FLG_r)
		arr_reverse(files);
}

/**
static int		ls_sort(void)
{
	// Select a sort function.
	if (g_ls_flg->reversesort)
	{
		if (g_ls_flg->sizesort)
			sortfcn = revsizecmp;
		else if (!g_ls_flg->timesort)
			sortfcn = revnamecmp;
		else if (g_ls_flg->accesstime)
			sortfcn = revacccmp;
		else if (g_ls_flg->statustime)
			sortfcn = revstatcmp;
		else if (g_ls_flg->birthtime)
			sortfcn = revbirthcmp;
		else		// Use modification time.
			sortfcn = revmodcmp;
	}
	else
	{
		if (g_ls_flg->sizesort)
			sortfcn = sizecmp;
		else if (!g_ls_flg->timesort)
			sortfcn = namecmp;
		else if (g_ls_flg->accesstime)
			sortfcn = acccmp;
		else if (g_ls_flg->statustime)
			sortfcn = statcmp;
		else if (g_ls_flg->birthtime)
			sortfcn = birthcmp;
		else		// Use modification time.
			sortfcn = modcmp;
	}
}
**/


/*
* Ordering for mastercmp:
* If ordering the argv (fts_level = FTS_ROOTLEVEL) return non-directories
* as larger than directories.  Within either group, use the sort function.
* All other levels use the sort function.  Error entries remain unsorted.
*/

/**
static int		mastercmp(const FTSENT **a, const FTSENT **b)
{
	int a_info, b_info;

	a_info = (*a)->fts_info;
	if (a_info == FTS_ERR)
		return (0);
	b_info = (*b)->fts_info;
	if (b_info == FTS_ERR)
		return (0);

	if (a_info == FTS_NS || b_info == FTS_NS)
		return (namecmp(*a, *b));

	if (a_info != b_info &&
			(*a)->fts_level == FTS_ROOTLEVEL && !f_listdir) {
		if (a_info == FTS_D)
			return (1);
		if (b_info == FTS_D)
			return (-1);
	}
	return (sortfcn(*a, *b));
}
**/
