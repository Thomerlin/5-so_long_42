/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tyago-ri <tyago-ri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/10 17:38:33 by tyago-ri          #+#    #+#             */
/*   Updated: 2021/11/10 13:55:03 by tyago-ri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>

#include <X11/X.h>
#include <X11/keysym.h>
#include <mlx.h>

# define WINDOW_WIDTH 600
# define WINDOW_HEIGHT 300

# define MLX_ERROR 1

# define RED_PIXEL 0xFF0000
# define GREEN_PIXEL 0xFF00
# define WHITE_PIXEL 0xFFFFFF

typedef struct s_img
{
	void *mlx_img;
	char *addr;
	int bpp;
	int line_len;
	int endian;
}	t_img;

typedef struct s_rect
{
	int x;
	int y;
	int width;
	int height;
	int color;
}	t_rect;

typedef struct s_data
{
	void *mlx_ptr;
	void *win_ptr;
	t_img img;
}	t_data;

int handle_keypress(int keysym, t_data *data)
{
	if (keysym == XK_Escape)
	{
		mlx_destroy_window(data->mlx_ptr, data->win_ptr);
		data->win_ptr = NULL;
	}
	return (0);
}

/*void img_pix_put(t_img *img, int x, int y, int color)
{
	char *pixel;

	pixel = img->addr + (y * img->line_len +x * (img->bpp / 8));
	*(int *)pixel = color;
}*/

void img_pix_put(t_img *img, int x, int y, int color)
{
	char *pixel;
	int i;

	i = img->bpp -8;
	pixel = img->addr + (y * img->line_len + x * (img->bpp / 8));
	while (i >= 0)
	{
		if (img->endian != 0)
			*pixel++ = (color >> i) & 0xFF;
		else
			*pixel++ = (color >> (img->bpp -8 -i)) & 0xFF;
		i -= 8;
	}
}

int render_rect(t_img *img, t_rect rect)
{
	int i;
	int j;

	/*if (data->win_ptr == NULL)
		return (1);*/
	i = rect.y;
	while (i < rect.y + rect.height)
	{
		j = rect.x;
		while (j < rect.x + rect.width)
			img_pix_put(img, j++, i, rect.color);
		++i;
	}
	return (0);
}

void render_background(t_img *img, int color)
{
	int i;
	int j;

	/*if (data->win_ptr == NULL)
		return ;*/
	i = 0;
	while (i <WINDOW_HEIGHT)
	{
		j = 0;
		while (j <WINDOW_WIDTH)
			img_pix_put(img, j++, i, color);
		++i;
	}
}

int render(t_data *data)
{
	if (data->win_ptr == NULL)
		return (1);
	render_background(&data->img, WHITE_PIXEL);
	render_rect(&data->img, (t_rect){
		WINDOW_WIDTH - 100, WINDOW_HEIGHT - 100, 100, 100, GREEN_PIXEL
	});
	render_rect(&data->img, (t_rect){
		0, 0, 100, 100, RED_PIXEL
	});

	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.mlx_img, 0, 0);

	return (0);
}

int main(void)
{
	t_data data;
	
	data.mlx_ptr = mlx_init();
	if (data.win_ptr == NULL)
		return (MLX_ERROR);
	data.win_ptr = mlx_new_window(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT, "My window!");
	if (data.win_ptr == NULL)
	{
		free(data.win_ptr);
		return (MLX_ERROR);
	}
	
	//Setup Hooks
	data.img.mlx_img = mlx_new_image(data.mlx_ptr, WINDOW_WIDTH, WINDOW_HEIGHT);

	data.img.addr = mlx_get_data_addr(data.img.mlx_img, &data.img.bpp,
					&data.img.line_len, &data.img.endian);

	mlx_loop_hook(data.mlx_ptr, &render, &data);
	mlx_hook(data.win_ptr, KeyPress, KeyPressMask, &handle_keypress, &data);

	mlx_loop(data.mlx_ptr);

	//exit window
	mlx_destroy_window(data.mlx_ptr, data.win_ptr);
	mlx_destroy_display(data.mlx_ptr);
	free(data.mlx_ptr);
}