/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_print_col.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 14:40:22 by jkalia            #+#    #+#             */
/*   Updated: 2017/06/30 16:44:19 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern int8_t	g_ls_flags;

static void		get_col_info(t_ls_file **tmp, t_col *col)
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


/**
while (++j < col->col)
{
	k = col->row * j + i;
	if (k >= col->file_count)
		break ;
	ft_printf("%-*s", col->max_len,
			((t_ls_file*)files->contents[k])->name);
}
**/

int8_t			ls_print_col(t_arr *files)
{
	t_col				*col;
	int					row;
	int					column;
	int					base;
	int					endcol;
	int					chcnt;
	int					cnt;

	col = ft_memalloc(sizeof(t_col));
	MEMCHECK(col);
//	col->sortacross = (g_ls_flags & FLG_X) ? 1 : 0;
	col->sortacross = 0;
	col->file_count = files->end;
	get_col_info((t_ls_file **)files->contents, col);
	DEBUG("num row = %d", col->numrow);
	DEBUG("num col = %d", col->numcol);
	//DEBUG("filecount = %d", col->file_count);
	DEBUG("col width = %d", col->colwidth);
	//DEBUG("Sort style = %d", col->sortacross);
	base = 0;
	row = 0;
	for (row = 0; row < col->numrow; ++row)
	{
		endcol = col->colwidth;
		if (col->sortacross)
			base = row;
		for (column = 0, chcnt = 0; column < col->numcol; ++column)
		{
//			chcnt += ft_printf("%d %s", base, ((t_ls_file*)files->contents[base])->name);
			chcnt += ft_printf("%s", ((t_ls_file*)files->contents[base])->name);
			if (col->sortacross)
				base++;
			else
				base += col->numrow;
			if (base >= col->file_count)
				break;
			/**
			while ((cnt = ((chcnt + col->tabwidth) & ~(col->tabwidth - 1)))
					<= endcol)
			{
				if (col->sortacross && column + 1 >= col->numcol)
					break;
				ft_putchar('\t');
				chcnt = cnt;
			}
			endcol += col->colwidth;
			//write(1, "\t", 1);
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
