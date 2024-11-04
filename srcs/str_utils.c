/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tkashi <tkashi@student.42lausanne.ch>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 17:49:15 by tkashi            #+#    #+#             */
/*   Updated: 2024/05/11 17:49:54 by tkashi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\f');
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	atou(char *str, unsigned int *num)
{
	*num = 0;
	while (*str != '\0' && is_whitespace(*str))
		str++;
	if (!ft_isdigit(*str))
		return (ARG_ERROR);
	while (*str != '\0' && ft_isdigit(*str))
	{
		if (*num > UINT_MAX / 10
			|| (*num == UINT_MAX / 10 && *str - '0' > UINT_MAX % 10))
			return (ARG_ERROR);
		*num = *num * 10 + *str - '0';
		str++;
	}
	while (*str != '\0' && is_whitespace(*str))
		str++;
	if (*str != '\0')
		return (ARG_ERROR);
	return (OK);
}
