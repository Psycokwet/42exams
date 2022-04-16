#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
int len(char *s)
{
	return s != NULL && *s!= '\0')? 1 + len(s+1) : 0;
}

#define OUT 1
#define true 1
#define false 0

int putc(char c)
{
	return write (OUT, &c, 1);
}
int puts(char *s)
{
	if(s == NULL)
		return puts("(Null)");
	return(write(OUT, s, len(s)));
}
#define NB "0123456789abcdefg"
int putn_rec(unsigned int, int base, int add)
{
	if(n/base == 0)
		return putc(NB[n]);
	int val = n- ((n/base)*base);
	val += add;
	return putn_rec(n/base, base,0) + putc (NB[val]);
}

int putn(int n, int base)
{
	if(base>10)
		return putn_rec(n, base, 0);
	if (n< 0 && n-1 > n)
		return putc('-') + putn_rec( (n+1)*-1, base, 1);
	if(n<0)
		return putc('-') + putn_rec(n * -1, base, 0);
	return putn_rec(n, base, 0); 
}

int ft_printf(const char* fmt, ...)
{
	va_list ap;
	int d;
	char *s;
	int result = 0;
	if(fmt == NULL)
		return -1;
	va_start(ap, fmt);
	while(*fmt)
	{
		if(*fmt == '%' && fmt [1] != '\0')
		{
			if(fmt[1] == 's')
			{
				s = va_arg(ap, char *);
				result += puts(s);
				fmt++;
			}
			else if(fmt[1] == 'd')
			{
				d = va_arg(ap, int);
				result += putn(d, 10);
				fmt++;
			}
			if(fmt[1] == 'x')
			{
				d = va_arg(ap, int);
				result += putn(d, 16);
				fmt++;
			}
			fmt++;
			continue;
		}
		result += putc(*fmt);
		fmt++;
	}
	return result;
}
