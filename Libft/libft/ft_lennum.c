/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lennum.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngulya <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/02 15:04:15 by ngulya            #+#    #+#             */
/*   Updated: 2016/12/02 15:05:11 by ngulya           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_lennum(int n)
{
	int	o;

	o = 0;
	if (n < 1)
		o = 1;
	while (n)
	{
		n = n / 10;
		o++;
	}
	return (o);
}
