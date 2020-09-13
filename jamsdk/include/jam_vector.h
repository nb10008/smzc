
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Vector Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_VECTOR_H__
#define __JAM_VECTOR_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 判断两个向量是否相等 */
	JAM_API bool jam_vec_compare(const vec_t* a, const vec_t* b, int32_t count);


	/** 向量置零 */
	JAM_API void jam_vec_zero(vec_t* inout, int32_t count);


	/** 向量取反 */
	JAM_API void jam_vec_inverse(vec_t* inout, int32_t count);


	/** 向量复制 */
	JAM_API void jam_vec_copy(vec_t* out, const vec_t* in, int32_t count);


	/** 向量相加 */
	JAM_API void jam_vec_add(vec_t* out, const vec_t* a, const vec_t* b, int32_t count);


	/** 向量相减 */
	JAM_API void jam_vec_sub(vec_t* out, const vec_t* a, const vec_t* b, int32_t count);


	/** 向量相乘 */
	JAM_API void jam_vec_mul(vec_t* out, const vec_t* a, const vec_t* b, int32_t count);


	/** 向量缩放 */
	JAM_API void jam_vec_scale(vec_t* out, const vec_t* a, float s, int32_t count);


	/** 点积 */
	JAM_API float jam_vec_dot(const vec_t* a, const vec_t* b, int32_t count);


	/** 向量长度平方 */
	JAM_API float jam_vec_length_sqr(const vec_t* a, int32_t count);


	/** 向量长度 */
	#define jam_vec_length(a, c)							((float)jam_sqrt(jam_vec_length_sqr(a, c)))


	/** 向量归一化 */
	JAM_API void jam_vec_normalize(vec_t* out, const vec_t* a, int32_t count);


	/** 向量与矩阵的变换 */
	JAM_API void jam_vec_transform_mat(vec4_t out, const vec_t* a, const mat44_t m, int32_t count);


	/** 向量与四元数的变换 */
	JAM_API void jam_vec3_trans_quat(vec3_t out, const vec3_t v, const quat_t q);


	/** 三维向量叉积 */
	JAM_API void jam_vec3_cross(vec3_t out, const vec3_t a, const vec3_t b);


	/** 四维向量叉积 */
	JAM_API void jam_vec4_cross(vec4_t out, const vec4_t a, const vec4_t b, const vec4_t c);


	/*
	JAM_API void jam_vec3_project(vec3_t out);
	JAM_API void jam_vec3_unproject(vec3_t out);
	*/


	/** 二维向量比较 */
	#define jam_vec2_compare(a, b)							jam_vec_compare(a, b, 2)
	/** 三维向量比较 */
	#define jam_vec3_compare(a, b)							jam_vec_compare(a, b, 3)
	/** 四维向量比较 */
	#define jam_vec4_compare(a, b)							jam_vec_compare(a, b, 4)


	/** 二维向量置零 */
	#define jam_vec2_zero(v)								jam_vec_zero(v, 2)
	/** 三维向量置零 */
	#define jam_vec3_zero(v)								jam_vec_zero(v, 3)
	/** 四维向量置零 */
	#define jam_vec4_zero(v)								jam_vec_zero(v, 4)


	/** 二维向量取反 */
	#define jam_vec2_inverse(v)								jam_vec_inverse(v, 2)
	/** 三维向量取反 */
	#define jam_vec3_inverse(v)								jam_vec_inverse(v, 3)
	/** 四维向量取反 */
	#define jam_vec4_inverse(v)								jam_vec_inverse(v, 4)


	/** 二维向量复制 */
	#define jam_vec2_copy(out, a)							jam_vec_copy(out, a, 2)
	/** 三维向量复制 */
	#define jam_vec3_copy(out, a)							jam_vec_copy(out, a, 3)
	/** 四维向量复制 */
	#define jam_vec4_copy(out, a)							jam_vec_copy(out, a, 4)


	/** 二维向量相加 */
	#define jam_vec2_add(out, a, b)							jam_vec_add(out, a, b, 2)
	/** 三维向量相加 */
	#define jam_vec3_add(out, a, b)							jam_vec_add(out, a, b, 3)
	/** 四维向量相加 */
	#define jam_vec4_add(out, a, b)							jam_vec_add(out, a, b, 4)


	/** 二维向量相减 */
	#define jam_vec2_sub(out, a, b)							jam_vec_sub(out, a, b, 2)
	/** 三维向量相减 */
	#define jam_vec3_sub(out, a, b)							jam_vec_sub(out, a, b, 3)
	/** 四维向量相减 */
	#define jam_vec4_sub(out, a, b)							jam_vec_sub(out, a, b, 4)


	/** 二维向量相乘 */
	#define jam_vec2_mul(out, a, b)							jam_vec_mul(out, a, b, 2)
	/** 三维向量相乘 */
	#define jam_vec3_mul(out, a, b)							jam_vec_mul(out, a, b, 3)
	/** 四维向量相乘 */
	#define jam_vec4_mul(out, a, b)							jam_vec_mul(out, a, b, 4)


	/** 二维向量缩放 */
	#define jam_vec2_scale(out, a, s)						jam_vec_scale(out, a, s, 2)
	/** 三维向量缩放 */
	#define jam_vec3_scale(out, a, s)						jam_vec_scale(out, a, s, 3)
	/** 四维向量缩放 */
	#define jam_vec4_scale(out, a, s)						jam_vec_scale(out, a, s, 4)


	/** 二维向量点积 */
	#define jam_vec2_dot(a, b)								jam_vec_dot(a, b, 2)
	/** 三维向量点积 */
	#define jam_vec3_dot(a, b)								jam_vec_dot(a, b, 3)
	/** 四维向量点积 */
	#define jam_vec4_dot(a, b)								jam_vec_dot(a, b, 4)


	/** 二维向量长度平方 */
	#define jam_vec2_len_sqr(a)								jam_vec_length_sqr(a, 2)
	/** 三维向量长度平方 */
	#define jam_vec3_len_sqr(a)								jam_vec_length_sqr(a, 3)
	/** 四维向量长度平方 */
	#define jam_vec4_len_sqr(a)								jam_vec_length_sqr(a, 4)


	/** 二维向量长度 */
	#define jam_vec2_len(a)									jam_vec_length(a, 2)
	/** 三维向量长度 */
	#define jam_vec3_len(a)									jam_vec_length(a, 3)
	/** 四维向量长度 */
	#define jam_vec4_len(a)									jam_vec_length(a, 4)


	/** 二维向量归一化 */
	#define jam_vec2_norm(out, a)							jam_vec_normalize(out, a, 2)
	/** 三维向量归一化 */
	#define jam_vec3_norm(out, a)							jam_vec_normalize(out, a, 3)
	/** 四维向量归一化 */
	#define jam_vec4_norm(out, a)							jam_vec_normalize(out, a, 4)


	/** 二维向量与矩阵的变换 */
	#define jam_vec2_trans_mat(out, a, m)					jam_vec_transform_mat(out, a, m, 2)
	/** 三维向量与矩阵的变换 */
	#define jam_vec3_trans_mat(out, a, m)					jam_vec_transform_mat(out, a, m, 3)
	/** 四维向量与矩阵的变换 */
	#define jam_vec4_trans_mat(out, a, m)					jam_vec_transform_mat(out, a, m, 4)


	/** 获取两个向量的角度(单位: 弧度) */
	#define jam_vec3_angle(a, b)							((float)jam_acos(jam_vec3_dot(a, b) / (jam_vec3_len(a) * jam_vec3_len(b))))



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_VECTOR_H__ */
