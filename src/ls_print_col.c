/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_col.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 14:40:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/29 15:52:45 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
#include <sys/ioctl.h>

typedef struct	s_col
{
	int					col;
	int					row;
	int					w_width;
	int					max_len;
	int					max_depth;
	int					file_count;
	struct	winsize		w;
}				t_col;

int8_t			ls_print_col(t_arr	*files)
{
	t_ls_file			**tmp;
	t_col				col;
	int					i;
	int					j;
	int					k;

	ft_bzero(&col, sizeof(col));
	col.file_count = files->end;
	tmp = (t_ls_file **)files->contents;
	col.max_len = 0;
	i = -1;
	while (++i < files->end)
	{
		col.max_len = MAX(col.max_len, (int)ft_strlen(tmp[i]->name));
	}
	++col.max_len;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &col.w);
	col.w_width = col.w.ws_col;
	col.col = MAX((col.w_width / (col.max_len)), 1);
	if ((col.max_len * files->end) < col.w_width)
		col.col = col.file_count;
	col.row = col.file_count / col.col;
	if (col.file_count % col.col)
		++col.row;
	i = 0;
	while (i < col.row)
	{
		j = 0;
		while (j < col.col)
		{
			k = col.row * j + i;
			if (k >= col.file_count)
				break ;
			ft_printf("%-*s", col.max_len, tmp[k]->name);
			++j;
		}
		write(1, "\n", 1);
		++i;
	}
	return (0);
}

/**
void column_display(t_entries entries, int file_count, int max_file_len, int target)
{
	struct winsize w;
	int cols;
	int rows;
	char **arr;
	int term_width;
	int i;
	int pos;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	term_width = w.ws_col;
	cols = term_width / (max_file_len + 1);
	if (!cols)
		cols = 1;
	if ((max_file_len + 1) * file_count < term_width)
		cols = file_count;
	rows = file_count / cols;
	if (file_count % cols)
		++rows;
	arr = NULL;
	if (target == IS_DIR)
	{
		MEMCHECK((arr = (char **)ft_memalloc(sizeof(char *) * (file_count + 1))));
		i = 0;
		while (entries.files)
		{
			if (entries.files->has_nonprintable_chars)
				arr[i++] = ft_strdup(entries.files->display_name);
			else
				arr[i++] = ft_strdup(entries.files->name);
			entries.files = entries.files->next;
		}
	}
	pos = 0;
	i = -1;
	while (++i < rows)
	{
		int j = -1;
		pos = i;
		while (++j < cols)
		{
			lprint_handler(1, "%s ", max_file_len, target == IS_DIR ? arr[pos] : entries.file_names[pos]);
			pos += rows;
			if (pos >= file_count)
				break;
		}
		print_handler(1, "\n", 0, NULL);
	}
	if (target == IS_DIR)
		free(arr);
}

**/
