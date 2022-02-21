#include "main.h"
#define OUT 1
#define MSG_ARG "Error: argument\n"
#define MSG_FILE "Error: Operation file corrupted\n"

int len(char* s)
{
	return !s || *s =='\0' ? 0 : 1 + len(s+1);
}
void myputs(char *s)
{
	write(OUT, s, len(s));
}

int get_absolut_y(t_env *env, int y)
{
	return y * env->w;
}

int err(char *msg)
{
	if(msg)
		myputs(msg);
	return 1;
}

void clear(FILE *f, t_env *env)
{

	fclose(f);
	if (env->area)
		free(env->area);
}

int	check_area(t_env *env)
{
	if(env->w < 0 || env->w > 300
			|| env->h < 0 || env->h > 300)
		return -EXIT_FAILURE;
	return EXIT_SUCCESS;
}

void myputs_w_l(char *s, int l)
{
	write(OUT, s, l);
}

void print_area(t_env *env)
{
	int i = 0;
	while (i < env->h)
	{
		myputs_w_l(env->area + get_absolut_y(env, i),  env->w);
		myputs("\n");
		i++;
	}
}

#define IS_IN_CENTER 1
#define IS_IN_BORDER 2

//specific micro

typedef struct s_shape {
	float xw;
	float yh;
	float w;
	float h;
	char type;
	char c;
} t_shape;

int	in_rectangle(float x, float y, t_shape shape)
{
	if (((x < shape.xw || (shape.xw + shape.w < x))
		|| (y < shape.yh)) || (shape.yh + shape.h < y))
		return (0);
	if (((x - shape.xw < 1.00000000) || ((shape.xw + shape.w) - x < 1.00000000)) ||
	((y - shape.yh < 1.00000000 || ((shape.yh + shape.h) - y < 1.00000000))))
		return (IS_IN_BORDER);
	return (IS_IN_CENTER);
}

void	draw_rectangle(t_env *env, t_shape shape)
{
	int x;
	int y;
	int	is_it;

	y = 0;
	while (y < env->h)
	{
		x = 0;
		while (x < env->w)
		{
			is_it = in_rectangle((float)x, (float)y, shape);
			if (is_it == IS_IN_BORDER
				|| (shape.type == 'R' &&  is_it == IS_IN_CENTER))
				env->area[get_absolut_y(env, y) + x] = shape.c;
			x++;
		}
		y++;
	}
}

int	consume_rectangles(t_env *env, FILE *f)
{
	int res;
	t_shape shape;
	while (1)
	{
		res = fscanf(f, "%c %f %f %f %f %c\n", &shape.type, &shape.xw, &shape.yh, &shape.w, &shape.h, &shape.c);
		if (res == -1)
			break;
		if (res != 6 || (shape.type != 'r' && shape.type != 'R') || (shape.w <=0 || shape.h <=0))
		{
			clear(f, env);
			return -EXIT_FAILURE;
		}
		draw_rectangle(env, shape);
	}
	return EXIT_SUCCESS;
}

int main(int ac, char** av)
{
	if(ac != 2)
		return err(MSG_ARG);
	FILE *f = fopen(av[1],"r");
	if(!f)
		return err(MSG_FILE);
	t_env env = {0};
	char c = 0;
	int res = fscanf(f, "%d %d %c\n", &env.w, &env.h, &c);
	if(res != 3 || check_area(&env) != EXIT_SUCCESS)
		return err(MSG_FILE);
	env.area = malloc(sizeof(char) * env.h * env.w);
	if(!env.area)
		return err(MSG_FILE);
	memset(env.area, c, env.h * env.w);
	if (consume_rectangles(&env, f) != EXIT_SUCCESS)
		return err(MSG_FILE);
	print_area(&env);
	clear(f, &env);
	return 0;
}