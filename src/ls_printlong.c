/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_printlong.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 08:19:09 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/30 23:07:18 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
#define PATH_MAX 4096

extern t_ls_flg		g_ls_flg;

static int8_t	print_link(char *path)
{
	char link[PATH_MAX + 1];

	ft_bzero(link, PATH_MAX + 1);
	if (readlink(path, link, PATH_MAX) == -1)
		return(ls_warn(get_basename(path)));
	ft_putstr(" -> ");
	ft_putstr(link);
	return (0);
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

static int8_t	print_time(time_t ftime)
{
	static time_t	now;
	char		*tmp;
	
	if (now == 0)
		now = time(NULL);
	tmp = ctime(&ftime);
	if (g_ls_flg.sectime)
		ft_printf(" %2.2s %3.3s %8.8s %4.4s", &tmp[8], &tmp[4], &tmp[11],
				&tmp[20]);
	else if (ftime + SIXMONTHS > now && ftime < now + SIXMONTHS)
	{
		ft_printf(" %3.3s %2.2s %5.5s", &tmp[4], &tmp[8], &tmp[11]);
	}
	else
	{
		ft_printf(" %2.2s %3.3s  %4.4s", &tmp[8], &tmp[4], &tmp[20]);
	}
	return (0);
}

int8_t		print_long(t_ls *file, int *padding)
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
	if (g_ls_flg.accesstime)
		print_time(file->statinfo.st_atime);
	else if (g_ls_flg.statustime)
		print_time(file->statinfo.st_ctime);
	else if (g_ls_flg.birthtime)
		print_time(file->statinfo.st_birthtime);
	else
		print_time(file->statinfo.st_mtime);
	ft_printf(" %s", file->name);
	if (S_ISLNK(file->statinfo.st_mode))
		print_link(file->path);
	return (0);
}

int8_t			ls_printlong(t_arr *files)
{
	int		i;
	int		blocks;
	int		padding[4];

	ft_bzero(padding, sizeof(int) * 4);
	i = -1;
	blocks = 0;
	while (++i < files->end)
	{
		find_padding(padding, ((t_ls *)files->contents[i])->statinfo);
		blocks += ((t_ls *)files->contents[i])->statinfo.st_blocks;
	}
	if (g_ls_flg.longform || g_ls_flg.size)
		ft_printf("total %d\n", blocks);
	i = -1;
	while (++i < files->end)
	{
		print_long((t_ls *)files->contents[i], padding);
		write(1, "\n", 1);
	}
	return (0);
}
