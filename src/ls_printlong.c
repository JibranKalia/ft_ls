/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_printlong.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 08:19:09 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/27 19:15:24 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
#include <langinfo.h>

extern t_ls_flg		g_ls_flg;

static void	print_link(char *path)
{
	char	tmp[4096 + 1];

	ft_bzero(tmp, 4096 + 1);
	readlink(path, tmp, 4096);
	ft_printf(" -> %s", tmp);
}

static int8_t	print_mode(mode_t mode)
{
	int	bits;

	bits = mode & S_IFMT;
	if (bits == S_IFDIR)
		write(1, "d", 1);
	else if (bits == S_IFCHR)
		write(1, "c", 1);
	else if (bits == S_IFBLK)
		write(1, "b", 1);
	else if (bits == S_IFLNK)
		write(1, "l", 1);
	else if (bits == S_IFSOCK)
		write(1, "s", 1);
	else if (bits == S_IFIFO)
		write(1, "f", 1);
	else
		write(1, "-", 1);
	return (0);
}

static int8_t	print_permission(mode_t mode)
{
	write(1, (mode & S_IRUSR) ? "r" : "-", 1);
	write(1, (mode & S_IWUSR) ? "w" : "-", 1);
	write(1, (mode & S_IXUSR) ? "x" : "-", 1);
	write(1, (mode & S_IRGRP) ? "r" : "-", 1);
	write(1, (mode & S_IWGRP) ? "w" : "-", 1);
	write(1, (mode & S_IXGRP) ? "x" : "-", 1);
	write(1, (mode & S_IROTH) ? "r" : "-", 1);
	write(1, (mode & S_IWOTH) ? "w" : "-", 1);
	write(1, (mode & S_IXOTH) ? "x" : "-", 1);
	write(1, " ", 1);
	return (0);
}

#define	SIXMONTHS	((365 / 2) * 86400)
/**
 * ctime output = Thu Nov 24 18:22:48 1986\n\0 
 * Index 4 - Nov
 * Index 8 - 24 
 * Index 11 - 18:22:48
 * Index 20 - 1986
 */

#define MONTH &tmp[4]	//%3.3s
#define DAY &tmp[8]		//%2.2s 
#define TIME &tmp[11]	//%8.8s for full time otherwise %5.5s
#define YEAR &tmp[20]	//%4.4s

static int8_t	print_time(t_ls *file)
{
	time_t		out_time;
	time_t		curr_time;
	char		*tmp;
	static int	d_first = -1;
	if (d_first < 0)
		d_first = (*nl_langinfo(D_MD_ORDER) == 'd');
	if (g_ls_flg.accesstime)
		out_time = file->statinfo.st_atime;
	else if (g_ls_flg.statustime)
		out_time = file->statinfo.st_ctime;
	else if (g_ls_flg.birthtime)
		out_time = file->statinfo.st_birthtime;
	else
		out_time = file->statinfo.st_mtime;
	curr_time = time(0);
	tmp = ctime(&out_time);
	if (g_ls_flg.sectime == 1) {
		if (d_first) {
			/* dd mmm hh:mm:ss yyyy */
			ft_printf(" %2.2s %3.3s %8.8s %4.4s", DAY, MONTH, TIME, YEAR);
		} else {
			/* mmm dd hh:mm:ss yyyy */
			ft_printf(" %3.3s %2.2s %8.8s %4.4s", MONTH, DAY, TIME, YEAR);
		}
	}
	else if (ABS(curr_time - out_time) > SIXMONTHS) {
		if (d_first) {
			ft_printf(" %2.2s %3.3s  %4.4s", DAY, MONTH, YEAR);
		} else {
			ft_printf(" %3.3s %2.2s  %4.4s", MONTH, DAY, YEAR);
		}
	}
	else {
		if (d_first) {
			ft_printf(" %2.2s %3.3s %5.5s", DAY, MONTH, TIME);
		} else {
			ft_printf(" %3.3s %2.2s %5.5s", MONTH, DAY, TIME);
		}
	}
	return (0);
}

int8_t		print_details(t_ls *file, int *padding)
{
	struct passwd	*pwd;
	struct group	*group;

	print_mode(file->statinfo.st_mode);
	print_permission(file->statinfo.st_mode);
	ft_printf(" %*d", padding[0], file->statinfo.st_nlink);
	if ((pwd = getpwuid(file->statinfo.st_uid)) != NULL)
		ft_printf(" %-*s ", padding[1], pwd->pw_name);
	if ((group = getgrgid(file->statinfo.st_gid)) != NULL)
		ft_printf(" %-*s", padding[2], group->gr_name);
	write(1, "  ", 2);
	ft_printf("%*lld", padding[3], file->statinfo.st_size);
	print_time(file);
	ft_printf(" %s", file->name);
	if (S_ISLNK(file->statinfo.st_mode))
		print_link(file->path);
	return (0);
}


int8_t			dir_print(t_arr *files, int blocks, int *padding) 
{
	int i;
	ft_printf("total %d\n", blocks);
	i = -1;
	while (++i < files->end)
	{
		print_details((t_ls *)files->contents[i], padding);
		write(1, "\n", 1);
	}
	return (0);
}

int8_t			fil_print(t_arr *files, int *padding)
{
	int i;
	i = -1;
	while (++i < files->end)
	{
		print_details((t_ls *)files->contents[i], padding);
		write(1, "\n", 1);
	}
	return (0);
}

int8_t			ls_printlong(t_arr *files)
{
	int		i;
	int		blocks;
	int		padding[4];

	DEBUG("PRINT LONG");
	ft_bzero(padding, sizeof(int) * 4);
	i = -1;
	blocks = 0;
	while (++i < files->end)
	{
		find_padding(padding, ((t_ls *)files->contents[i])->statinfo);
		blocks += ((t_ls *)files->contents[i])->statinfo.st_blocks;
	}
	if (((t_ls *)files->contents[0])->parameter_type == enum_dir) {
		dir_print(files, blocks, padding);
	} else {
		fil_print(files, padding);
	}
	return (0);
}
