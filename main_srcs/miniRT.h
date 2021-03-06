#ifndef MINIRT_H
# define MINIRT_H
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <math.h>
# include <stdint.h>
# include <limits.h>

# include <X11/Xlib.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <X11/extensions/XShm.h>
# include "./minilibx-linux/mlx.h"

//3D vector
typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

typedef struct s_color
{
	double	r;
	double	g;
	double	b;
}	t_color;

//カメラの回転のことを考えたら，最初から，クオータニオンで持ってた方が多分いいんだけど，
//今回は，必要になったら構造体要素に追加して，変換のプログラム書くことにする
//camera circular struct list.
//p:place, vd:direction vector, vsb:screen base vector,
//vptos:vector form camera place to screan origin,vray:ray vector from camera
//prev means before camera struct and next means next camera struct.
//image: this varinable has every pixel color informatin,
//so, use pixel put to this varinable, we can see the window view.
typedef struct s_cam
{
	int		cnum;
	t_vec3	p;
	t_vec3	vd;
	double	fov;
	t_vec3	vsb1;
	t_vec3	vsb2;
	t_vec3	vptos;
	t_vec3	vray;
	struct s_cam	*prev;
	struct s_cam	*next;
	double	distance; // imageとともに，width * hightでmalloc
	t_color	tmpcolor;
	int		*image;
}	t_cam;

//p:place of light, r:brightneess of light ratio
//vctol: vector from cam to light, you have to chang this for each camera
typedef struct s_light
{
	int		lnum;
	t_vec3	p;
	double	r;
	t_color	color;
	t_vec3	vctol;
	struct s_light	*next;
}	t_light;

//flag initialize -1 first
typedef struct s_amblight
{
	double	r;
	t_color	color;
	int		flag;
}	t_amblight;

//gob means geometric object.
//vno: normalized orientation vector.
//vctoc:vector from camera to object center. you have to chage if camera change.
//dは直径なので注意(rtファイルで与えられるのは直径)
//use h onluy for cylinder hight.
typedef struct s_gob
{
	int		type;
	int		obnum;
	t_vec3	p1;
	t_vec3	p2;
	t_vec3	p3;
	t_vec3	vno;
	double	d;
	double	h;
	t_color	color;
	t_vec3	vctoc;//球の場合，球の中心からカメラへ向かうベクトル
	struct s_gob	*prev;
	struct s_gob	*next;
}	t_gob;

//perror is in stdio.h, strerror is in string.h
//from this struct, we can reach every element used in this program.
typedef struct s_minirt
{
	t_cam		*firstcam;
	t_light		*firstlight;
	t_gob		*firstgob;
	t_amblight	al;
	double		width;
	double		hight;
}	t_minirt;

//in main file
//initialize minirt struct;
void	ft_initialize_minirt(t_minirt *minirt);
//if argc == 2, do this function.
void	ft_type2(t_minirt *minirt, char *argv);


//read RTfile.
//in read RTfile1　読み取りができるとこまでした，read RTfile2以降の関数との組み合わせはまだ．
int		ft_get_info(t_minirt *minirt, char *argv);
char	*ft_read_rtfile(int fd);
int		ft_input_info(t_minirt *minirt, char **line);
int		ft_switch_inputtype(t_minirt *minirt, char *line);

//in read RTfile2(util functions to get each information)
int		get_pv(char *line, int i, t_vec3 *v);
int		get_two_vec(char *line, int i, t_vec3 *v1, t_vec3 *v2);
int		get_three_vec(char *line, int i, t_vec3 *v1, t_vec3 *v2, t_vec3 *v3);
int		get_fov(char *line, int i, double *fov);
int		ft_get_color(char *line, int i, t_color *color);
//in read RTfile3(for camera, window, light)
int		ft_cam_input(t_cam **firstcam, char *line);
int		ft_windowinfo_input(t_minirt *minirt, char *line);
int		ft_light_input(t_light **firstlight, char *line);
int		ft_amblight_input(t_amblight *al, char *line);
//in read RTfile4(object input main, sphere and plane)
int		ft_object_input(t_minirt *minirt, char *line);
int		ft_isobject(char *line);
int		ft_sphere_input(t_gob **firstgob, char *line);
int		ft_plane_input(t_gob **firstgob, char *line);
//in read RTfile5(square, cylinder and triangle)
int		ft_square_input(t_gob **firstgob, char *line);
int		ft_cylinder_input(t_gob **firstgob, char *line);
int		ft_input_SqAndCy_sub(t_gob *new, char *line, int i);
int		ft_triangle_input(t_gob **firstgob, char *line);


//出力関係 testまだ

//出力計算の前準備
int	ft_prepare_print(t_minirt *minirt);
int	ft_cam_prepare(t_cam *firstcam, double width, double hight);
void	ft_light_prepare(t_light *flight, t_cam *cam);
void	ft_obj_prepare(t_gob *firstgob);

//ft_make_screan.c
t_cam	*ft_make_screan_base(t_cam *cam);

//ft_make_ray.c
t_vec3	ft_make_ray(t_cam *cam, double x, double y);
void	ft_show_image(t_minirt *minirt);
int		ft_calcu_color(t_minirt *minirt, double x, double y);
void	ft_print_obj(t_minirt *minirt);
double	ft_sp_color(t_gob *sp, t_cam *cam, t_light *light, t_amblight al);
double	ft_make_sp(t_cam *cam, t_gob *sp);
t_color ft_set_color(double r, double g, double b);
t_color	ft_ambient_light(t_color c_color, t_amblight a);
void	ft_show_image(t_minirt *minirt);
double	ft_pl_color(t_gob *pl, t_cam *cam, t_light *l, t_amblight al);


//light
void 	ft_diffusion_light_sp(t_cam *cam, t_light *l, t_gob *sp, t_vec3 v);
void	ft_diffusion_light_pl(t_cam *cam, t_light *l, t_gob *pl);
t_color	ft_set_diffuse_color1(t_color c_c, t_color l_c, t_color s_c, double cos);
t_color	ft_set_diffuse_color2(t_color c_c, t_color l_c, t_color s_c, double cos);

//影の実装
int	ft_iscross(t_gob *ob, t_light *l, t_cam *cam);
int iscross_sp(t_gob *sp, t_vec3 lp, t_cam *cam);
int	iscross_pl(t_gob *pl, t_vec3 lp, t_cam *cam);


double	ft_quadratic_func(double a, double b, double c);


//utility functions

//function in libft1
int		ft_isspace(char c);
int		ft_atol(char *s, int i, double *result);
int		ft_atof(char *s, int i, double *result);
size_t	ft_strlen(char *s);
char	*ft_strjoin(char *s1, char *s2);
//function in libft2
void	ft_free_array(char **s);
char	**ft_split(char const *s, char c);
//function in libft3
void	*ft_memcpy(void *dst, const void *src, size_t n);
t_cam	*ft_camlstlast(t_cam *lst);
t_light	*ft_lightlstlast(t_light *lst);
t_gob	*ft_oblstlast(t_gob *lst);
int		ft_safe_free1(void *ptr);
//function in libft4
void	ft_clear_minirt(t_minirt *minirt);
void	ft_camlstclear(t_cam *firstcam);
void	ft_lightlstclear(t_light *firstlight);
void	ft_oblstclear(t_gob *firstgob);

//vector util1
t_vec3	ft_linear_transform(t_vec3 v1, t_vec3 v2, double a, double b);
double	ft_inner_product(t_vec3 v1, t_vec3 v2);
t_vec3	ft_cross_product(t_vec3 v1, t_vec3 v2);
double	ft_v_d_len(t_vec3 v);
t_vec3	ft_make_unitvec(t_vec3 v);
//vector util2
t_vec3	ft_set_vecele(double x, double y, double z);
void	ft_put_vector(t_vec3 v);
int		ft_isnormal(t_vec3 *v);
t_vec3	ft_gramschmidt_1(t_vec3 v1, t_vec3 v2);
t_vec3	ft_gramschmidt_2(t_vec3 v1, t_vec3 v2, t_vec3 v3);



//error stdout
void	ft_put_rtfile_error(char **line, int i);

//for test
void	print_minirt_struct(t_minirt *minirt);
void	print_window_al(t_minirt *minirt);
void	print_struct_light(t_light *fisrt);
void	print_struct_cam(t_cam *fisrt);
void	print_struct_gob(t_gob *firstgob);
void	print_color(t_color *color);

void print_prepare_cam(t_cam *first);
void	print_prepare_obj(t_gob *first);
#endif
