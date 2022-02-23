#include "validated_mini.h"

int len(char *s)
{
	return !s || *s == '\0' ? 0 : 1 + len(s + 1);
}

int mputs(char *s)
{
	return write(OUT, s, len(s));
}

int puts_w_l(char *s, int l)
{
	return write(OUT, s, l);
}
int get_absolut_y(t_env *env, int y)
{
	return env->w * y;
}

int clear(t_env *env, int code)
{
	if(env->f)
		fclose(env->f);
	if(env->area)
		free(env->area);
	*env = (t_env){0};
	return code;
}

int check_dim(t_env *env)
{
	if(env->h <= 0 || env->w <=0 || env->h > 300 || env->w > 300)
		return -EXIT_FAILURE;
	return EXIT_SUCCESS;
}

int err(char *s)
{
	if(s)
		mputs(s);
	return 1;
}

int print_area(t_env *env)
{
	int y = 0;
	while(y < env->h)
	{
		if(puts_w_l(env->area + get_absolut_y(env, y), env->w) != env->w)
			return -EXIT_FAILURE;
		if(mputs("\n") != 1)
			return -EXIT_FAILURE;
		y++;
	}
	return EXIT_SUCCESS;
}

int in_circle(float x, float y, t_shape shape)
{
	float distance = sqrtf(powf(x - shape.xw, 2.) + powf(y - shape.yh, 2.));
	if (distance <= shape.rad)
	{
		if (shape->rad - distance < UN)
			return IS_BORDER;
		return IS_INSIDE;
	}
	return 0;
}

void draw_circle(t_env *env, t_shape shape)
{
	int x = 0;
	int y = 0;
	int is_in = 0;
	while(y < env->h)
	{
		x = 0;
		while(x < env->w)
		{
			is_in = in_circle((float)x, (float)y, shape);
			if (is_in == IS_BORDER || (shape.type == 'C' && is_in == IS_INSIDE))
				env->area[get_absolut_y(env, y) + x] = env->c;
			x++;
		}
		y++;
	}
}

int consume_circle(t_env *env)
{
	int res = 0;
	t_shape shape;
	while(1)
	{
		res = fscanf(env->f, "%c %f %f %f %c\n", &shape.type, &shape.xw, &shape.yh, &shape.rad, &shape.c);
		if(res == -1)
			break;
		if (res!= 5 || shape.rad <= 0 || (shape.type !='C' && shape.type != 'c'))
			return -EXIT_FAILURE;
		draw_circle(env, shape);
	}
	return EXIT_SUCCESS;
}

int main(int ac, char**av)
{
	if (ac != 2)
		return err(ERR_ARGS);
	t_env env = (t_env){0};
	env.f = fopen(av[1], "r");
	if(env.f)
		return err(ERR_FILE);
	int res = fscanf(env.f, "%d %d %c\n", &env.w, &env.h, &env.c);
	if(res != 3 || check_dim(&env) != EXIT_SUCCESS)
		return clear(&env, err(ERR_FILE));
	env.area = malloc(sizeof(char) * env.w * env.h);
	if (!env.area)
		return clear(&env, err(ERR_FILE));
	memset(env.area, env.c, env.w * env.h);
	if(consume_circle(&env) != EXIT_SUCCESS)
		return clear(&env, err(ERR_FILE));
	if(print_area(&env) != EXIT_SUCCESS)
		return clear(&env, err(ERR_FILE));
	return clear(&env, 0);
}
