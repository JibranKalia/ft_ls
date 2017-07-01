/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 21:25:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/30 22:48:30 by jkalia           ###   ########.fr       */
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
typedef struct timespec	t_timespec;

enum					e_ls_flags
{
	FLG_l = 0x1,
	FLG_R = 0x2,
	FLG_a = 0x4,
	FLG_r = 0x8,
	FLG_t = 0x10,
	FLG_T = 0x20,

	FLG_singlecol;		/* use single column output */
	FLG_longform;		/* long listing format */
	FLG_sortacross;		/* sort across rows, not down columns */ 
	FLG_statustime;		/* use time of last mode change */
	FLG_accesstime;		/* use time of last access */
	FLG_birthtime;		/* use time of file birth */
	FLG_recursive;		/* ls subdirectories also */
	FLG_reversesort;	/* reverse whatever sort is used */
	FLG_listall;		/* list files beginning with . and .. */
	FLG_listdot;		/* list files beginning with . */
	FLG_nosort;		/* don't sort output */
	FLG_sizesort;		/* sort by size */
	FLG_sectime;		/* print the real time for all files */
	FLG_timesort;		/* sort by time vice name */
	FLG_notabs;		/* don't use tab-separated multi-col output */

	FLG_type;		/* add type character for non-regular files */
	FLG_slash;		/* similar to f_type, but only for dirs */
	FjG_stream;		/* stream the output, separate with commas */
	FLG_color;
	FLG_humanval;		/* show human-readable file sizes */

	/* Case Q */
	FLG_nonprint;		/* show unprintables as ? */
	FLG_octal;		/* show unprintables as \xxx */
	FLG_octal_escape;	/* like f_octal but use C escapes if possible */

	/* Case e */
	FLG_acl;		/* show ACLs in long listing */

	/* Case @ */
	FLG_xattr;		/* show extended attributes in long listing */

	FLG_flags;		/* show flags associated with a file */
	FLG_inode;		/* print inode */
	FLG_kblocks;		/* print size in kilobytes */
	FLG_listdir;		/* list actual directory, not contents */
	FLG_numericonly;	/* don't convert uid/gid to name */
	FLG_size;		/* list size in short listing */
	FLG_whiteout;		/* show whiteout entries */
	FLG_group;		/* show group */
	FLG_owner;		/* show owner *
};

typedef struct			s_ls_file
{
	char				*path;
	char				*name;
	t_stat				statinfo;
	t_timespec			lstime;
}						t_ls_file;

void					file_del(void	*elm);
int8_t					ft_ls_print_dir(char *path);
int8_t					ls_usage(char flag);
void					ls_sort(t_arr *files);
int8_t					ft_ls_l(t_arr *files);
void					find_padding(int *padding, t_stat statinfo);
#endif
