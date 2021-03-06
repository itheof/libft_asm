/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvallee <tvallee@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/29 19:44:50 by tvallee           #+#    #+#             */
/*   Updated: 2018/10/04 13:20:37 by tvallee          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "libfts.h"

static void test_puts(void)
{
	int		ret;
	int		puts_ret1;
	int		puts_ret2;
	int		puts_ret3;
	int		out;
	int		p[2];
	char	buf[10000];

	out = dup(1);
	pipe(p);
	dup2(p[1], 1);
	puts_ret1 = ft_puts("aaa");
	puts_ret2 = ft_puts(NULL);
	close(1);
	puts_ret3 = ft_puts("closed");
	dup2(out, 1);
	ret = read(p[0], buf, 10000);
	buf[ret] = 0;
	close(p[0]);
	close(p[1]);
	close(out);
	assert(puts_ret1 > 0);
	assert(puts_ret2 > 0);
	assert(puts_ret3 == EOF);
	assert(strcmp(buf, "aaa\n(null)\n") == 0);
}

static void	test_strlen(void)
{
	char const	*tests[] = {
		"",
		"abcd",
		"hello ceci est une grosse string\n"
	};
	size_t	test;
	size_t	ref;

	for (int i = 0; i < sizeof(tests) / sizeof(*tests); i++)
	{
		test = ft_strlen(tests[i]);
		ref = strlen(tests[i]);
		assert(test == ref);
	}
}

#define STR1 "salut ! :) "
#define STR2 "Moi c'est theof le grand programmeur x86_64"
#define STR3 "lol"

static void	test_strcat(void)
{
	char	buf[256] = STR1;

	assert(ft_strcat(buf, STR2) == buf);
	assert(!strcmp(buf, STR1 STR2));
	*buf = 0;
	assert(ft_strcat(buf, STR3) == buf);
	assert(!strcmp(buf, STR3));
}

#undef STR1
#undef STR2
#undef STR3

typedef int	(*t_isfunc)(int c);
static void test_isfunc(t_isfunc test, t_isfunc ref)
{
	int ret;
	int retref;
	for (int c = 0; c < UCHAR_MAX; c++)
	{
		ret = test(c);
		retref = ref(c);
		if (ret != retref)
			printf("%d | %c: test: %d, ref: %d\n", c, c, ret, retref);
		assert((ret && retref) || (!ret && !retref));
	}
}

static void	test_bzero(void)
{
	char	*str;
	size_t	n;
	char const	magic[] = "DEADBEEF";

	str = malloc(256);
	n = 0;
	while (n < 256)
	{
		memcpy(str + n, magic, 8);
		n += 8;
	}
	ft_bzero(str + 80, 60);
	n = 0;
	while (n < 256)
	{
		if (n >= 80 && n < 140)
			assert(str[n] == 0);
		else
			assert(str[n] == magic[n & 0x7]);
		n++;
	}
	ft_bzero(str, 0);
	assert(*str == 'D');
	ft_bzero(NULL, 0);
}

static void	test_memset(void)
{
	char	buf[256];

	memset(buf, 'a', 256);
	ft_memset(buf, 'd', 0);
	assert(buf[0] == 'a');

	ft_memset(buf, 'd', 3);
	for (int c = 0; c < 3; c++)
		assert(buf[c] == 'd');
	assert(buf[3] == 'a');

	assert(ft_memset(buf + 128, 0, 1) == buf + 128);
	assert(buf[129] == 'a');
	assert(buf[128] == 0);
}

static void	test_memcpy(void)
{
	char	buf[256] = "slt correcteur";

	assert(ft_memcpy(buf, "bjr", 0) == buf);
	assert(strcmp(buf, "slt correcteur") == 0);
	ft_memcpy(buf, "bjr", 3);
	assert(strcmp(buf, "bjr correcteur") == 0);
}

static void	test_strdup(void)
{
	char const	*tests[] = {
		"",
		"abcd",
		"hello ceci est une grosse string\n"
	};
	char	*tmp;
	size_t	test;
	size_t	ref;

	for (int i = sizeof(tests) / sizeof(*tests) - 1; i >= 0; i--)
	{
		tmp = ft_strdup(tests[i]);
		if (tmp)
			assert(!strcmp(tmp, tests[i]));
		free(tmp);
	}
}

static void	test_cat(void)
{
	int		ret;
	int		puts_ret1;
	int		puts_ret2;
	int		out;
	int		p[2];
	char	buf[10000];
	int		writep[2];

	pipe(writep);
	write(writep[1], "slt\n", 4);
	close(writep[1]);

	out = dup(1);
	pipe(p);
	dup2(p[1], 1);
	ft_cat(writep[0]);
	dup2(out, 1);

	close(writep[0]);

	ret = read(p[0], buf, 10000);
	buf[ret] = 0;
	close(p[0]);
	close(p[1]);
	close(out);
	assert(ret == 4);
	assert(strcmp(buf, "slt\n") == 0);
}

static void	test_putchar(char c)
{
	int		ret;
	int		out;
	int		p[2];
	char	buf[2];

	out = dup(1);
	pipe(p);
	dup2(p[1], 1);
	ft_putchar(c);
	dup2(out, 1);
	ret = read(p[0], buf, 2);
	assert(ret == 1);
	buf[ret] = 0;
	close(p[0]);
	close(p[1]);
	close(out);
	assert(buf[0] == c);
}

static void	test_putnbr(char const *nums)
{
	int	num;
	int		ret;
	int		out;
	int		p[2];
	char	buf[10000];

	num = atoi(nums);
	out = dup(1);
	pipe(p);
	dup2(p[1], 1);
	ft_putnbr(num);
	dup2(out, 1);
	ret = read(p[0], buf, 10000);
	buf[ret] = 0;
	close(p[0]);
	close(p[1]);
	close(out);
	assert(!strcmp(buf, nums));
}

static void	test_putstr(char const *str)
{
	int		ret;
	int		out;
	int		p[2];
	char	buf[10000];

	out = dup(1);
	pipe(p);
	dup2(p[1], 1);
	ft_putstr(str);
	dup2(out, 1);
	ret = read(p[0], buf, 10000);
	buf[ret] = 0;
	close(p[0]);
	close(p[1]);
	close(out);
	assert(!strcmp(buf, str));
}

static void	test_strcmp(char const *s1, char const *s2)
{
	assert(strcmp(s1, s2) == ft_strcmp(s1, s2));
}

int		main(void)
{
	test_bzero();
	test_strcat();
	test_isfunc(ft_isalpha, isalpha);
	test_isfunc(ft_isdigit, isdigit);
	test_isfunc(ft_isalnum, isalnum);
	test_isfunc(ft_isascii, isascii);
	test_isfunc(ft_isprint, isprint);
	test_isfunc(ft_toupper, toupper);
	test_isfunc(ft_tolower, tolower);
	test_strlen();
	if (!fork())
	{
		test_puts();
		return 0;
	}
	test_memset();
	test_memcpy();
	test_strdup();
	test_cat();

	test_isfunc(ft_islower, islower);
	test_isfunc(ft_isupper, isupper);
	test_putchar('s');
	test_putchar('l');
	test_putchar('t');
	test_putchar('\n');
	test_putnbr("-2147483648");
	test_putnbr("0");
	test_putnbr("-42");
	test_putnbr("42");
	test_putstr("slt");
	test_strcmp("", "");
	test_strcmp("hello", "");
	test_strcmp("salut", "salut");
	test_strcmp("saluto", "salut");
	test_strcmp("salut", "saluto");
}
