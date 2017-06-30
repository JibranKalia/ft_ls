/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_col.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 14:40:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/30 08:15:44 by jkalia           ###   ########.fr       */
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
}

int8_t			ls_print_col(t_arr *files)
{
	t_col				*col;
	int					i;
	int					j;
	int					k;

	col = ft_memalloc(sizeof(t_col));
	MEMCHECK(col);
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
			ft_printf("%-*s", col->max_len,
					((t_ls_file*)files->contents[k])->name);
		}
		write(1, "\n", 1);
	}
	free(col);
	return (0);
}

int8_t				ls_print_simple(t_arr *files)
{
	t_ls_file	**tmp;
	int			i;

	tmp = (t_ls_file **)files->contents;
	i = -1;
	while (++i < files->end)
		ft_printf("%s\n", tmp[i]->name);
	return (0);
}
