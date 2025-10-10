/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymunoz-m <ymunoz-m@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/24 10:07:00 by psapio            #+#    #+#             */
/*   Updated: 2025/01/15 19:40:58 by ymunoz-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	int				a;
	int				b;
	unsigned char	*s1cast;
	unsigned char	*s2cast;

	s1cast = (unsigned char *)s1;
	s2cast = (unsigned char *)s2;
	a = 0;
	b = 0;
	while ((n != 0) && (s1[a] != '\0') && (s2[b] != '\0'))
	{
		if (s1cast[a] != s2cast[b])
			return (s1cast[a] - s2cast[b]);
		a++;
		b++;
		n--;
	}
	if (n == 0)
		return (0);
	return (s1cast[a] - s2cast[b]);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

/*
int	main(void)
{
	char	idea_a[] = "\200";
	char	idea_b[] = "\0";
	int		size;
	int		result;

	size = 15;
	result = ft_strncmp(idea_a, idea_b, size);
	printf ("La tuya: %d\n", result);
	printf ("Original: %d\n", strncmp(idea_a, idea_b, size));
	return (0);
}
*/
