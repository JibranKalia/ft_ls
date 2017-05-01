/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 21:25:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/05/01 11:51:37 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <dirent.h>
# include <grp.h>
# include <libft.h>
# include <pwd.h>
# include <sys/xattr.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <time.h>
# include <uuid/uuid.h>

typedef struct dirent	t_dir;
typedef struct stat		t_stat;

enum					e_ls_flags
{
	FLG_l = 0x1,
	FLG_R = 0x2,
	FLG_a = 0x4,
	FLG_r = 0x8,
	FLG_t = 0x10
};

typedef struct			s_ls_file
{
	char				*path;
	char				*name;
	t_stat				stat;
}						t_ls_file;

#endif
