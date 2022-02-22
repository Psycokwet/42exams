#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>

#define OUT 1
#define IS_BORDER 1
#define IS_INSIDE 2
#define UN 1.00000000
#define ERR_ARGS "Error: argument\n"
#define ERR_FILE "Error: Operation file corrupted\n"

typedef struct s_env
{
	int h;
	int w;
	char c;
	FILE *f;
	char *area;
} t_env;

typedef struct s_shape
{
	float rad;
	float xw;
	float yh;
	char c;
	char type;
} t_shape

#endif

#endif