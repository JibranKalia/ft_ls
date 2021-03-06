/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_util.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/01 17:09:41 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 11:03:01 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

void		file_del(void *elm)
{
	if (elm == 0)
		return ;
	ft_strdel(&((t_ls *)elm)->name);
	ft_strdel(&((t_ls *)elm)->path);
	ft_bzero(elm, sizeof(t_ls));
}

int8_t		ls_usage(char flag)
{
	if (flag != 0)
		ft_dprintf(2 , "ls: illegal option -- %c\n", flag);
	ft_dprintf(STDOUT_FILENO, "usage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n");
	//ft_dprintf(2 ,"usage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n");
	return (-1);
}

void		find_padding(int *padding, t_stat statinfo)
{
	padding[0] = MAX(padding[0], ft_nbrlen(statinfo.st_nlink));
	padding[1] = MAX(padding[1], (int)ft_strlen(getpwuid(statinfo.st_uid)->pw_name));
	padding[2] = MAX(padding[2], (int)ft_strlen(getgrgid(statinfo.st_gid)->gr_name));
	padding[3] = MAX(padding[3], ft_nbrlen(statinfo.st_size));
}

char	*get_basename(char *path)
{
	char *name;

	name = path;
	while (*path)
	{
		if (*path++ == '/')
			name = path;
	}
	return (name);
}
