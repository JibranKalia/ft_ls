/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_col.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 14:40:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/01 10:53:24 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern int		g_ls_flags;

static void		get_col_info(t_ls **tmp, t_col *col)
{
	int			i;
	char		*p;

	col->max_len = 0;
	col->tabwidth = 8;
	col->termwidth = 80;
	i = -1;
	while (++i < col->file_count)
		col->max_len = MAX(col->max_len, (int)ft_strlen(tmp[i]->name));
	col->colwidth = col->max_len;
	if ((p = getenv("COLUMNS")) != NULL && *p != '\0')
		col->termwidth = ft_atoi(p);
	else if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &col->win) != -1 && col->win.ws_col > 0)
		col->termwidth = col->win.ws_col;
	col->colwidth = (col->colwidth + col->tabwidth) & ~(col->tabwidth - 1);
	col->numcol = (col->termwidth / (col->colwidth));
	col->numrow = col->file_count / col->numcol;
	if (col->file_count % col->numcol)
		++col->numrow;
}


int8_t			ls_print_col(t_arr *files)
{
	t_col				*col;
	int					row;
	int					column;
	int					base;

	col = ft_memalloc(sizeof(t_col));
	MEMCHECK(col);
	col->sortacross = (g_ls_flags & FLG_x) ? 1 : 0;
	col->file_count = files->end;
	get_col_info((t_ls **)files->contents, col);
	base = 0;
	row = -1;
	while(++row < col->numrow)
	{
		col->endcol = col->colwidth;
		if (!col->sortacross)
			base = row;
		column = -1;
		col->chcnt = 0;
		while (++column < col->numcol)
		{
			col->chcnt += ft_printf("%s", ((t_ls*)files->contents[base])->name);
			if (col->sortacross)
				base++;
			else
				base += col->numrow;
			if (base >= col->file_count)
				break;
			while ((col->cnt = ((col->chcnt + col->tabwidth) & ~(col->tabwidth - 1)))
					<= col->endcol)
			{
				if (col->sortacross && column + 1 >= col->numcol)
					break;
				write(1, "\t", 1);
				col->chcnt = col->cnt;
			}
			col->endcol += col->colwidth;
		}
		write(1, "\n", 1);
	}
	free(col);
	return (0);
}

int8_t				ls_print_simple(t_arr *files)
{
	t_ls	**tmp;
	int			i;

	tmp = (t_ls **)files->contents;
	i = -1;
	while (++i < files->end)
		ft_printf("%s\n", tmp[i]->name);
	return (0);
}
