/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 09:07:07 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/20 19:50:24 by jkalia           ###   ########.fr       */
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
** The -c and -u options override each other.
* The -d option turns off the -R option.
** singlecol: use single column output
** longform: long listing format
** sortacross: sort across rows, not down columns
** statustime: use time of last mode change
** accesstime: use time of last access
** birthtime: use time of file birth
** recursive: ls subdirectories also
** reversesort: reverse whatever sort is used
** listall: list files beginning with . and ..
** listdot: list files beginning with .
** nosort: don't sort output
** sizesort: sort by size
** sectime: pr*the real time for all files
** timesort: sort by time vice name
** notabs: don't use tab-separated multi-col output
**
** type: add type character for non-regular files
** slash: similar to type, but only for dirs
** stream: stream the output, separate with commas
** color:1;
** humanval: show human-readable file sizes
**
** Case e
** acl: show ACLs in long listing
**
** Case @
** xattr: show extended attributes in long listing
**
** flags: show flags associated with a file
** inode: pr*inode
** kblocks: pr*size in kilobytes
** listdir: list actual directory, not contents
** numericonly: don't convert uid/gid to name
** size: list size in short listing
** whiteout: show whiteout entries
** group: show group
** owner: show owner
**
** seedot:
** nonprint:
*/

/**
enum					e_ls_flags
{
	FLG_l = 0x1,
	FLG_R = 0x2,
	FLG_a = 0x4,
	FLG_r = 0x8,
	FLG_t = 0x10,
	FLG_T = 0x20,
	FLG_f = 0x40,
	FLG_1 = 0x80,
	FLG_C = 0x100,
	FLG_x = 0x200,
	FLG_c = 0x400,
	FLG_U = 0x800,
	FLG_u = 0x1000
};
**/

typedef struct			s_ls_flg
{
	unsigned int singlecol:1;
	unsigned int longform:1;
	unsigned int sortacross:1;
	unsigned int statustime:1;
	unsigned int accesstime:1;
	unsigned int birthtime:1;
	unsigned int recursive:1;
	unsigned int reversesort:1;
	unsigned int listall:1;
	unsigned int listdot:1;
	unsigned int nosort:1;
	unsigned int sizesort:1;
	unsigned int sectime:1;
	unsigned int timesort:1;
	unsigned int notabs:1;

	unsigned int type:1;
	unsigned int slash:1;
	unsigned int stream:1;
	unsigned int color:1;
	unsigned int humanval:1;
	unsigned int acl:1;
	unsigned int xattr:1;

	unsigned int flags:1;
	unsigned int inode:1;
	unsigned int kblocks:1;
	unsigned int listdir:1;
	unsigned int numericonly:1;
	unsigned int size:1;
	unsigned int whiteout:1;
	unsigned int group:1;
	unsigned int owner:1;
	
	unsigned int seedot:1;
	unsigned int nonprint:1;
}						t_ls_flg;

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

typedef struct			s_ls_data
{
	long				blocksize;
}						t_ls_data;

typedef struct			s_ls
{
	char				*path;
	char				*name;
	t_stat				statinfo;
	t_timespec			lstime;
}						t_ls;

void					ls_sort(t_arr *files);

/*
** LS_FLAGS
*/
int8_t					handle_flag_1(int c);

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
