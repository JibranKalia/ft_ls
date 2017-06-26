/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 08:19:09 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/26 08:37:27 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern int8_t	g_ls_flags;

static int8_t		print_mode(mode_t mode)
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

static int8_t		print_permission(mode_t mode)
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
	return (0);
}

static int8_t		print_long(t_ls_file *file)
{
	struct passwd *pwd;


	print_mode(file->statinfo.st_mode);
	print_permission(file->statinfo.st_mode);
	ft_printf("%*d", 3, file->statinfo.st_nlink);
	if ((pwd = getpwuid(file->statinfo.st_uid)) != NULL)
		ft_printf("%*s", 13, pwd->pw_name);

	return (0);
}

int8_t			ft_ls_l(t_arr *files)
{
	int	i;
	
	i = -1;
	while (++i < files->end)
	{
		print_long((t_ls_file *)files->contents[i]);
		write(1, "\n", 1);
	}
	return (0);
}

