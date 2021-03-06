#include "./miniRT.h"

int	ft_isspace(char c)
{
	if (c == ' ' || (9 <= c && c <= 13))
		return (1);
	else
		return (0);
}
//精度とnormが微妙
int	ft_atof(char *s, int i, double *result)
{
	double	j;
	double	minus;

	j = 1;
	i = ft_atol(s, i, result);
	minus = 1;
	if (i == 0)
		return (0);
	if (s[i] == '.' && '0' <= s[i + 1] && s[i + 1] <= '9')
	{
		if (*result < 0)
		{
			minus = -1;
			if (*result == -1 && s[i - 1] == '0')
				*result = 0;
			else
				*result = *result * (-1);
		}
		i++;
		while ('0' <= s[i] && s[i] <= '9')
			*result = *result + ((s[i++]) - '0') * (j *= 0.1);
		*result = *result * minus;
	}
	if (*result == INFINITY /*|| *result == NaN*/)
		return (0);
	return (i);
}

//-0.5とかに対応できてない．
int	ft_atol(char *s, int i, double *result)
{
	long	minus;

	minus = 1;
	*result = 0;
	while (ft_isspace(s[i]))
		i++;
	if (s[i] == '+' || s[i] == '-')
		if (s[i++] == '-')
			minus = -1;
	while ('0' <= s[i] && s[i] <= '9')
	{
		if (*result > LONG_MAX / 10 && minus == 1)
			return (0);
		else if (*result == LONG_MAX / 10 && *s > '7' && minus == 1)
			return (0);
		else if (*result > LONG_MAX / 10 && minus == -1)
			return (0);
		else if (*result == LONG_MAX / 10 && *s > '8' && minus == -1)
			return (0);
		*result = ((s[i++]) - '0') + (*result) * 10;
	}
	*result = (*result) * minus;
	if (*result == 0 && minus == -1 && s[i] == '.')
		*result = -1;
	return (i);
}

size_t	ft_strlen(char *s)
{
	size_t	len;

	len = 0;
	if (s == NULL)
		return (0);
	while (s[len] != '\0')
		len++;
	return (len);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*result;
	char	*tmp1;
	char	*tmp2;

	result = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (result == NULL)
		return (NULL);
	tmp1 = s1;
	tmp2 = result;
	if (s1 != NULL)
	{
		while (*s1 != '\0')
			*tmp2++ = *s1++;
	}
	while (*s2 != '\0')
		*tmp2++ = *s2++;
	*tmp2 = '\0';
	free(tmp1);
	s1 = NULL;
	return (result);
}

// int	main(int argc, char *argv[])
// {
// 	double	a;
// 	double	b;

// 	if (argc != 3)
// 		return (0);
// 	ft_atol(argv[1], 0, &a);
// 	ft_atof(argv[2], 0, &b);
// 	printf("a=%ld b=%.30lf\n", (long)a, b);
// 	return (0);
// }

// int main(int argc, char *argv[])
// {
// 	char *tmp = malloc(6);

// 	char *s = ft_strjoin(argv[1], argv[2]);
// 	printf("%s\n", s);
// 	// s = ft_strjoin(NULL, argv[1]);
// 	// printf("%s\n", s);
// }
