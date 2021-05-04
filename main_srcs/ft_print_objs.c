#include "./miniRT.h"

void	ft_print_obj(t_minirt *minirt)
{
	int i;
	int j;
	double x;
	double y;
	t_gob	*tmp;

	tmp = minirt->firstgob;
	while (1)
	{
		i = 0;
		ft_light_prepare(minirt->firstlight, minirt->firstcam);
		while (i < (int)minirt->width)
		{
			x = i - minirt->width / 2;
			j = 0;
			while (j < (int)minirt->hight)
			{
				minirt->firstgob = tmp;
				y = (-1) * (j - minirt->hight / 2);
				minirt->firstcam->image[i * (int)minirt->hight + j] = ft_calcu_color(minirt, x, y);
				j++;
			}
			i++;
		}
		if (minirt->firstcam->next->cnum == 1)
			break ;
		minirt->firstcam = minirt->firstcam->next;
	}
}

int	ft_prepare_print(t_minirt *rt)
{
	t_gob *tmp;
	//windowサイズとvmの画面サイズを比較して，正しいサイズにwidthとhightを変更する関数用意
	if (!ft_cam_prepare(rt->firstcam, rt->width, rt->hight))
		return (0);
	ft_obj_prepare(rt->firstgob);
	return (1);
}

void	ft_light_prepare(t_light *flight, t_cam *cam)
{
	t_light *tmp1;
	int i;

	tmp1 = flight;
	i = 1;
	while (flight != NULL)
	{
		flight->lnum = i;
		flight->vctol = ft_linear_transform(flight->p, cam->p, 1, -1);
		i++;
		flight = flight->next;
	}
	flight = tmp1;
	return ;
}

int	ft_cam_prepare(t_cam *firstcam, double width, double hight)
{
	t_cam	*tmp1;
	t_cam	*tmp2;
	int		i;

	tmp1 = firstcam;
	i = 1;
	while(firstcam != NULL)
	{
		firstcam->fov = firstcam->fov * M_PI / 180;
		firstcam->vptos = ft_linear_transform
			(firstcam->vd, firstcam->vd, (width / (2 * (tan(firstcam->fov / 2)))), 0);
		firstcam = ft_make_screan_base(firstcam);
		firstcam->cnum = i;
		firstcam->image = (int *)malloc(sizeof(int) * width * hight);
		if (firstcam->image == NULL)
			return (0);
		if (firstcam->next == NULL)
			tmp2 = firstcam;
		firstcam = firstcam->next;
		i++;
	}
	tmp2->next = tmp1;
	tmp1->prev = tmp2;
	return (1);
}

void	ft_obj_prepare(t_gob *firstgob)
{
	t_gob	*tmp1;
	t_gob	*tmp2;
	int		i;

	tmp1 = firstgob;
	i = 1;
	while (firstgob != NULL)
	{
		firstgob->obnum = i;
		i++;
		if (firstgob->next == NULL)
			tmp2 = firstgob;
		firstgob = firstgob->next;
		i++;
	}
	tmp2->next = tmp1;
	return ;
}