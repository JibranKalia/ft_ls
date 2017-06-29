/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 21:25:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/29 16:13:28 by jkalia           ###   ########.fr       */
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
	FLG_1 = 0x80
};

typedef struct			s_col
{
	int					col;
	int					row;
	int					w_width;
	int					max_len;
	int					max_depth;
	int					file_count;
	struct winsize		w;
}						t_col;

typedef struct			s_ls_file
{
	char				*path;
	char				*name;
	t_stat				statinfo;
	t_timespec			lstime;
}						t_ls_file;

void					file_del(void	*elm);
int8_t					ls_print_dir(char *path);
int8_t					ls_usage(char flag);
void					ls_sort(t_arr *files);
int8_t					ls_l(t_arr *files);
void					find_padding(int *padding, t_stat statinfo);
char					*get_basename(char *path);
int8_t					ls_handle_args(int i, int argc, char **argv);
int8_t					ls_print_col(t_arr	*files);
#endif
