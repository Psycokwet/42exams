#ifndef MAIN_H
# define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>  
#include <string.h>

typedef struct s_env {
	int h;
	int w;
	char *area;



} t_env;

#endif
// clang main.c  -Wall -Werror -W  -Wextra -g3 -fsanitize=address