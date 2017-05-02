/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_extra.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/01 17:09:41 by jkalia            #+#    #+#             */
/*   Updated: 2017/05/01 17:10:53 by jkalia           ###   ########.fr       */
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
