/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dod_gnl.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngulya <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 16:31:57 by ngulya            #+#    #+#             */
/*   Updated: 2017/06/06 16:31:57 by ngulya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/get_next_line.h"

void				doddddd_gnl(t_gn **str, char **line)
{
	*line = ft_strdup((*str)->str);
	free((*str)->str);
	(*str)->str = NULL;
}
