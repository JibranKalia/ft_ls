/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_extra.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/01 17:09:41 by jkalia            #+#    #+#             */
/*   Updated: 2017/05/01 17:48:01 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

void		file_del(void	*elm)
{
	if (elm == 0)
		return ;

	free(((t_ls_file *)elm)->name);
	free(((t_ls_file *)elm)->path);
	ft_bzero(elm, sizeof(t_ls_file));
}

int8_t		ls_usage(char flag)
{
	ft_dprintf(2 , "ls: illegal option -- %c\n", flag);
	ft_dprintf(STDOUT_FILENO, "usage: ft_ls [-Ralrt] [file ...]\n");
	//ft_dprintf(2 ,"usage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n");
	return (-1);
}
