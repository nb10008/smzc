
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Quaternion Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_QUAT_H__
#define __JAM_QUAT_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 四元数相乘 */
	JAM_API void jam_quat_mul(quat_t out, const quat_t a, const quat_t b);


	/** 四元数指数 */
	JAM_API void jam_quat_exp(quat_t out, const quat_t a);


	/** 四元数对数 */
	JAM_API void jam_quat_log(quat_t out, const quat_t a);


	/** 四元数幂 */
	JAM_API void jam_quat_pow(quat_t out, const quat_t a, float t);


	/** 四元数线性插值 */
	JAM_API void jam_quat_slerp(quat_t out, const quat_t a, const quat_t b, float bal);


	/** 通过向量和角度计算四元数, 向量必须是归一化的 */
	JAM_API void jam_quat_from_angle(quat_t out, const vec3_t v, float angle);


	/** 通过四元数获取向量和角度 */
	JAM_API void jam_quat_to_angle(vec3_t v, float* angle, const quat_t q);


	/** 通过旋转轴计算四元数 */
	JAM_API void jam_quat_from_yaw_pitch_roll(quat_t out, float yaw, float pitch, float roll);


	/** 通过四元数获取旋转轴 */
	JAM_API void jam_quat_to_yaw_pitch_roll(float* yaw, float* pitch, float* roll, const quat_t q);


	/** 通过旋转轴计算四元数, 向量必须是归一化的 */
	JAM_API void jam_quat_from_axes(quat_t out, const vec3_t x, const vec3_t y, const vec3_t z);


	/** 通过四元数获取旋转轴 */
	JAM_API void jam_quat_to_axes(vec3_t x, vec3_t y, vec3_t z, const quat_t q);


	/** 四元数转矩阵 */
	JAM_API void jam_quat_to_mat(mat44_t out, const quat_t q);


	/** 四元数共轭 */
	#define jam_quat_conj(out, a)							do{out[0] = -a[0]; out[1] = -a[1]; out[2] = -a[2]; out[3] = a[3];}while(0)


	/** 四元数比较 */
	#define jam_quat_compare(a, b)							jam_vec4_compare(a, b)


	/** 四元数置零 */
	#define jam_quat_zero(v)								jam_vec4_zero(v)


	/** 四元数取反 */
	#define jam_quat_inverse(out, a)						do{jam_quat_norm(out, a); jam_quat_conj(out, out);}while(0)


	/** 四元数复制 */
	#define jam_quat_copy(out, a)							jam_vec4_copy(out, a)


	/** 四元数相加 */
	#define jam_quat_add(out, a, b)							jam_vec4_add(out, a, b)


	/** 四元数相减 */
	#define jam_quat_sub(out, a, b)							jam_vec4_sub(out, a, b)


	/** 四元数点积 */
	#define jam_quat_dot(a, b)								jam_vec4_dot(a, b)


	/** 四元数长度 */
	#define jam_quat_len_sqr(a)								jam_vec4_len_sqr(a)


	/** 四元数长度的平方根 */
	#define jam_quat_len(a)									jam_vec4_len(a)


	/** 四元数归一化 */
	#define jam_quat_norm(out, a)							jam_vec4_norm(out, a)


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_QUAT_H__ */
