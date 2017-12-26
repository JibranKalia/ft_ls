/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_printcol.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 14:40:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/26 12:00:58 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern t_ls_flg		g_ls_flg;
extern t_ls_data	g_data;

static void		get_col_info(t_ls **tmp, t_col *col)
{
	int			i;

	col->max_len = 0;
	col->tabwidth = 8;
	col->termwidth = g_data.termwidth;
	DEBUG("TERMWIDTH = %d", col->termwidth);
	i = -1;
	while (++i < col->file_count)
		col->max_len = MAX(col->max_len, (int)ft_strlen(tmp[i]->name));
	col->colwidth = col->max_len;
	col->colwidth = (col->colwidth + col->tabwidth) & ~(col->tabwidth - 1);
	col->numcol = (col->termwidth / (col->colwidth));
	col->numrow = col->file_count / col->numcol;
	if (col->file_count % col->numcol)
		++col->numrow;
}


int8_t			ls_printcol(t_arr *files)
{
	t_col				*col;
	int					row;
	int					column;
	int					base;

	DEBUG("PRINT COL");
	col = ft_memalloc(sizeof(t_col));
	MEMCHECK(col);
	col->sortacross = g_ls_flg.sortacross;
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
			while ((col->cnt = ((col->chcnt + col->tabwidth) & ~(col->tabwidth - 1))) <= col->endcol)
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

int8_t				ls_printscol(t_arr *files)
{
	t_ls	**tmp;
	int			i;

	DEBUG("PRINT S COL");
	tmp = (t_ls **)files->contents;
	i = -1;
	while (++i < files->end)
		ft_printf("%s\n", tmp[i]->name);
	return (0);
}
