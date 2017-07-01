/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 09:07:07 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 09:45:44 by jkalia           ###   ########.fr       */
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

/*
** f_singlecol: use single column output
** f_longform: long listing format
** f_sortacross: sort across rows, not down columns
** f_statustime: use time of last mode change
** f_accesstime: use time of last access
** f_birthtime: use time of file birth
** f_recursive: ls subdirectories also
** f_reversesort: reverse whatever sort is used
** f_listall: list files beginning with . and ..
** f_listdot: list files beginning with .
** f_nosort: don't sort output
** f_sizesort: sort by size
** f_sectime: pr*the real time for all files
** f_timesort: sort by time vice name
** f_notabs: don't use tab-separated multi-col output
**
** f_type: add type character for non-regular files
** f_slash: similar to f_type, but only for dirs
** f_stream: stream the output, separate with commas
** f_color:1;
** f_humanval: show human-readable file sizes
**
** Case e
** f_acl: show ACLs in long listing
**
** Case @
** f_xattr: show extended attributes in long listing
**
** f_flags: show flags associated with a file
** f_inode: pr*inode
** f_kblocks: pr*size in kilobytes
** f_listdir: list actual directory, not contents
** f_numericonly: don't convert uid/gid to name
** f_size: list size in short listing
** f_whiteout: show whiteout entries
** f_group: show group
** f_owner: show owner
*/

typedef struct			s_ls_flags
{
	unsigned int f_singlecol:1;
	unsigned int f_longform:1;
	unsigned int f_sortacross:1;
	unsigned int f_statustime:1;
	unsigned int f_accesstime:1;
	unsigned int f_birthtime:1;
	unsigned int f_recursive:1;
	unsigned int f_reversesort:1;
	unsigned int f_listall:1;
	unsigned int f_listdot:1;
	unsigned int f_nosort:1;
	unsigned int f_sizesort:1;
	unsigned int f_sectime:1;
	unsigned int f_timesort:1;
	unsigned int f_notabs:1;

	unsigned int f_type:1;
	unsigned int f_slash:1;
	unsigned int f_stream:1;
	unsigned int f_color:1;
	unsigned int f_humanval:1;
	unsigned int f_acl:1;
	unsigned int f_xattr:1;

	unsigned int f_flags:1;
	unsigned int f_inode:1;
	unsigned int f_kblocks:1;
	unsigned int f_listdir:1;
	unsigned int f_numericonly:1;
	unsigned int f_size:1;
	unsigned int f_whiteout:1;
	unsigned int f_group:1;
	unsigned int f_owner:1;
}						t_ls_flags;

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

typedef struct			s_ls_main
{
}						t_ls_main

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
