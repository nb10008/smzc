
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Math Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MATH_H__
#define __JAM_MATH_H__


#include "jam_datatype.h"
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif


	#define JAM_PI											3.14159265358979323846
	#define JAM_EPSILON										(2.2204460492503131e-016)
	#define JAM_DEG2RAD										0.0174532925199432957692		/* JAM_PI / 180.0f */
	#define JAM_RAD2DEG										57.2957795130823208768464		/* 180.0f / JAM_PI */


	/** 最小值 */
	#define jam_min(a, b)									((a) < (b) ? (a) : (b))


	/** 最大值 */
	#define jam_max(a, b)									((a) > (b) ? (a) : (b))


	/** 交换 a 与 b 的值, t 是临时变量 */
	#define jam_swap(t, a, b)								do{(t) = (a); (a) = (b); (b) = (t);}while(0)


	/** 限定 v 的值在 a 与 b 之间 */
	#define jam_clamp(v, min_v, max_v)						jam_max(min_v, jam_min(max_v, v))


	/** 计算线性插值 */
	#define jam_lerp(min_v, max_v, bal)						((min_v) + (((max_v) - (min_v)) * jam_clamp(bal, 0.0, 1.0)))


	/** 判断是否为奇数 */
	#define jam_is_odd(x)									(1 == ((x) & 1))


	/** 判断是否为偶数 */
	#define jam_is_even(x)									(0 == ((x) & 1))


	/** 平方 */
	#define jam_sqr(x)										((x) * (x))


	/** 立方 */
	#define jam_cube(x)										(jam_sqr(x) * (x))


	/** 平方根 */
	#define jam_sqrt(x)										sqrt(x)


	/** 平方根倒数 */
	#define jam_rsqrt(x)									(1.0 / jam_sqrt(x))


	/** 浮点数取模 */
	#define jam_fmod(x, y)									fmod(x, y)


	/** 小于 x 值的最大整数 */
	#define jam_floor(x)									floor(x)


	/** 大于 x 值的最小整数 */
	#define jam_ceil(x)										ceil(x)


	/** 幂 */
	#define jam_pow(x, y)									pow(x, y)


	/** 指数 */
	#define jam_exp(x)										exp(x)


	/** 对数 */
	#define jam_log(x)										log(x)
	#define jam_log10(x)									log10(x)


	/** 正弦 */
	#define jam_sin(x)										sin(x)


	/** 余弦 */
	#define jam_cos(x)										cos(x)


	/** 正切 */
	#define jam_tan(x)										tan(x)


	/** 双曲正弦 */
	#define jam_sinh(x)										sinh(x)


	/** 双曲余弦 */
	#define jam_cosh(x)										cosh(x)


	/** 双曲正切 */
	#define jam_tanh(x)										tanh(x)


	/** 反正弦 */
	#define jam_asin(x)										asin(x)


	/** 反余弦 */
	#define jam_acos(x)										acos(x)


	/** 反正切 */
	#define jam_atan(x)										atan(x)


	/** 反正切 (y / x) */
	#define jam_atan2(y, x)									atan2(y, x)

	
	/** 角度转弧度 */
	#define jam_deg_to_rad(deg)								((deg) * JAM_DEG2RAD)


	/** 弧度转角度 */
	#define jam_rad_to_deg(rad)								((rad) * JAM_RAD2DEG)


	/** 比较2个浮点数是否相等 */
	#define jam_fcmp(a, b)									(jam_fabs((a) - (b)) < JAM_EPSILON)


	/** 小于2的n次方的最大整数 */
	JAM_API uint32_t jam_next2pow(uint32_t n);


	/** 求一个数的最高位位置 */
	JAM_API int32_t jam_high_bit_pos(uint32_t n);


	/** 绝对值 */
	#define jam_abs(x)										(((x) < 0) ? (-x) : (x))


	/** 整型快速绝对值 */
	JAM_API int32_t jam_labs(int32_t num);


	/** 浮点快速绝对值 */
	JAM_API float jam_fabs(float num);


	/** 随机数 */
	JAM_API int32_t jam_rand();


	/** 在指定范围内产生随机数.( min_v < x < max_v ) */
	#define jam_random(min_v, max_v)						((jam_rand() % jam_labs(max_v - min_v)) + min_v)


	/** 浮点随机数.(0.0f - 1.0f) */
	#define jam_rand_f()									(((float)jam_rand()) / 0x80000000)


	/** 在指定范围内产生浮点随机数.( min_v < x < max_v ) */
	#define jam_random_f(min_v, max_v)						((jam_rand_f() * jam_fabs(max_v - min_v)) + min_v)


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MATH_H__ */
