/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 21:25:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 07:00:29 by jkalia           ###   ########.fr       */
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
# include <sys/ioctl.h>
# include <time.h>
# include <uuid/uuid.h>

typedef struct dirent	t_dir;
typedef struct stat		t_stat;
typedef struct timespec	t_timespec;

typedef struct			ls_flags
{
	int f_l;
	int f_R;
	int f_a;
	int f_r;
	int f_t;
	int f_T;
	int f_f;
	int f_1;
	int f_C;
	int f_x;
	int f_c;
	int f_U;
	int f_u;

	int f_singlecol;		/* use single column output */
	int f_longform;		/* long listing format */
	int f_sortacross;		/* sort across rows, not down columns */
	int f_statustime;		/* use time of last mode change */
	int f_accesstime;		/* use time of last access */
	int f_birthtime;		/* use time of file birth */
	int f_recursive;		/* ls subdirectories also */
	int f_reversesort;	/* reverse whatever sort is used */
	int f_listall;		/* list files beginning with . and .. */
	int f_listdot;		/* list files beginning with . */
	int f_nosort;		/* don't sort output */
	int f_sizesort;		/* sort by size */
	int f_sectime;		/* print the real time for all files */
	int f_timesort;		/* sort by time vice name */
	int f_notabs;		/* don't use tab-separated multi-col output */

	int f_type;		/* add type character for non-regular files */
	int f_slash;		/* similar to f_type, but only for dirs */
	FjG_stream;		/* stream the output, separate with commas */
	int f_color;
	int f_humanval;		/* show human-readable file sizes */

	/* Case Q */
	int f_nonprint;		/* show unprintables as ? */
	int f_octal;		/* show unprintables as \xxx */
	int f_octal_escape;	/* like f_octal but use C escapes if possible */

	/* Case e */
	int f_acl;		/* show ACLs in long listing */

	/* Case @ */
	int f_xattr;		/* show extended attributes in long listing */

	int f_flags;		/* show flags associated with a file */
	int f_inode;		/* print inode */
	int f_kblocks;		/* print size in kilobytes */
	int f_listdir;		/* list actual directory, not contents */
	int f_numericonly;	/* don't convert uid/gid to name */
	int f_size;		/* list size in short listing */
	int f_whiteout;		/* show whiteout entries */
	int f_group;		/* show group */
	int f_owner;		/* show owner */
};

typedef struct			s_col
{
	int					numcol;
	int					numrow;
	int					termwidth;
	int					tabwidth;
	int					colwidth;
	int					max_len;
	int					max_depth;
	int					file_count;
	int					sortacross;
	int					endcol;
	int					chcnt;
	int					cnt;
	struct winsize		win;
}						t_col;

typedef struct			s_ls_file
{
	char				*path;
	char				*name;
	t_stat				statinfo;
	t_timespec			lstime;
}						t_ls_file;

void					ls_sort(t_arr *files);



/*
** LS_ARGS
*/

int8_t					ls_handle_args(int i, int argc, char **argv);

/*
** LS_PRINT
*/

int8_t					ls_print_dir(char *path);
int8_t					ls_print_col(t_arr	*files);
int8_t					ls_print_simple(t_arr *files);
int8_t					ls_print_l(t_arr *files);
int8_t					print_long(t_ls_file *file, int *padding);

/*
** LS_UTIL
*/

void					find_padding(int *padding, t_stat statinfo);
char					*get_basename(char *path);
void					file_del(void	*elm);
int8_t					ls_usage(char flag);
#endif
