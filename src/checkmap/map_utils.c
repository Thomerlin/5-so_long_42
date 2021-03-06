/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: coder <coder@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/17 15:03:44 by tyago-ri          #+#    #+#             */
/*   Updated: 2021/11/17 19:48:12 by coder            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../so_long.h"

int	map_utils(int fd1)
{
	int		size;
	int		count;
	int		verify;
	char	buffer;

	count = 0;
	verify = 0;
	while (true)
	{
		size = read(fd1, &buffer, 1);
		if ((buffer == 'P' || buffer == 'E'))
		{
			count++;
			if (count > 2)
				return (print_string("Dois ou mais \'P\' OR \'E\'"));
		}
		if ((buffer == 'P' || buffer == 'E' || buffer == 'C') && size > 0)
			verify++;
		if (size == 0)
			break ;
	}
	return (verify);
}
