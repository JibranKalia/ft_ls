/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 21:25:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/30 20:16:14 by jkalia           ###   ########.fr       */
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
