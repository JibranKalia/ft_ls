/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ls_traverse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/29 09:07:07 by jkalia            #+#    #+#             */
/*   Updated: 2017/07/27 19:06:57 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>

extern t_ls_flg			g_ls_flg;
extern PRINTLS			g_printfcn;

/**
static void		print_custom_l(t_arr *fil)
{
	int		i;
	int		padding[4];

	ft_bzero(padding, sizeof(int) * 4);
	i = -1;
	while (++i < fil->end)
		find_padding(padding, ((t_ls *)fil->contents[i])->statinfo);
	i = -1;
	while (++i < fil->end)
	{
		print_long((t_ls *)fil->contents[i], padding);
		write(1, "\n", 1);
	}
	write(1, "\n", 1);
}
**/

static void		handle_files(t_arr *fil)
{
	DEBUG("HANDLE FILES");
	if (fil->end > 0)
		ls_sort(fil);
	g_printfcn(fil);
	ft_printf("\n");
}

static void		handle_naf(t_arr *naf)
{
	int		i;

	DEBUG("HANDLE NAF");
	ls_sort(naf);
	i = -1;
	while (++i < naf->end)
		ft_dprintf(2, "ls: %s: %s\n", ((t_ls *)naf->contents[i])->name, strerror(errno));
}

static int8_t	create_arr(t_arr **naf, t_arr **dir, t_arr **fil)
{
	*dir = arr_create(sizeof(t_ls), 10);
	MEMCHECK(*dir);
	*naf = arr_create(sizeof(t_ls), 10);
	MEMCHECK(*naf);
	*fil = arr_create(sizeof(t_ls), 10);
	MEMCHECK(*fil);
	(*fil)->del = &file_del;
	(*naf)->del = &file_del;
	(*dir)->del = &file_del;
	return (0);
}

static int8_t	handle_arr(t_arr *naf, t_arr *dir, t_arr *fil)
{
	bool handleDOT = true;
	if (dir->end == 0 && (naf->end > 0 || fil->end > 0))
		handleDOT = false;
	if (naf->end > 0)
		handle_naf(naf);
	if (fil->end > 0)
		handle_files(fil);
	// Unique case where there is a naf and only one dir. The name of dir has to be printed
	if (naf->end > 0 && dir->end == 1)
		ft_printf("%s:\n", ((t_ls *)dir->contents[0])->path);
	handle_dir(dir, naf->end, fil->end);
	arr_del(dir);
	arr_del(naf);
	arr_del(fil);
	return (0);
}

int8_t			ls_traverse(int i, int argc, char **argv)
{
	t_arr			*naf;
	t_arr			*dir;
	t_arr			*fil;
	t_ls			*tmp;
	
	if( create_arr(&naf, &dir, &fil) == -1)
		return (-1);

	while (i < argc)
	{
		DEBUG("TRAVERSE");
		tmp = ft_memalloc(sizeof(t_ls));
		MEMCHECK(tmp);
		tmp->path = ft_strdup(argv[i]);
		tmp->name = ft_strdup(get_basename(argv[i]));
		if (stat(argv[i], &tmp->statinfo) == -1)
		{
			DEBUG("NAF");
			tmp->parameter_type = enum_naf;
			arr_push(naf, tmp);
		}
		else if (S_ISDIR(tmp->statinfo.st_mode))
		{
			DEBUG("DIR");
			tmp->parameter_type = enum_dir;
			arr_push(dir, tmp);
		}
		else
		{
			DEBUG("FILE");
			tmp->parameter_type = enum_fil;
			arr_push(fil, tmp);
		}
		++i;
	}
	handle_arr(naf, dir, fil);
	return (0);
}
