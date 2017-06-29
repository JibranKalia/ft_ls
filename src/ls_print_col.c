/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_col.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 14:40:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/29 16:23:19 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

static void		get_col_info(t_ls_file **tmp, t_col *col)
{
	int			i;

	col->max_len = 0;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &col->w);
	i = -1;
	while (++i < col->file_count)
		col->max_len = MAX(col->max_len, (int)ft_strlen(tmp[i]->name));
	++col->max_len;
	col->w_width = col->w.ws_col;
	col->col = (col->w_width / (col->max_len));
	if (col->col == 0)
		col->col = 1;
	if ((col->max_len * col->file_count) < col->w_width)
		col->col = col->file_count;
	col->row = col->file_count / col->col;
	if (col->file_count % col->col)
		++col->row;
	DEBUG("MAX LEN %d", col->max_len);
	DEBUG("COL %d", col->col);
	DEBUG("ROW %d", col->row);
}

int8_t			ls_print_col(t_arr *files)
{
	t_col				*col;
	int					i;
	int					j;
	int					k;

	col = ft_memalloc(sizeof(col));
	col->file_count = files->end;
	get_col_info((t_ls_file **)files->contents, col);
	i = -1;
	while (++i < col->row)
	{
		j = -1;
		while (++j < col->col)
		{
			k = col->row * j + i;
			if (k >= col->file_count)
				break ;
			ft_printf("%-*s", col->max_len, ((t_ls_file*)files->contents[k])->name);
		}
		write(1, "\n", 1);
	}
	free(col);
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
