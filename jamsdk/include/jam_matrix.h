
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Matrix Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MATRIX_H__
#define __JAM_MATRIX_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 比较两个矩阵是否相等 */
	JAM_API bool jam_mat_compare(const mat44_t a, const mat44_t b);


	/** 矩阵置零 */
	JAM_API void jam_mat_zero(mat44_t mat);


	/** 矩阵复制 */
	JAM_API void jam_mat_copy(mat44_t out, const mat44_t in);


	/** 是否为单位矩阵 */
	JAM_API bool jam_mat_is_identity(mat44_t mat);


	/** 设置单位矩阵 */
	JAM_API void jam_mat_set_identity(mat44_t mat);


	/** 矩阵相加 */
	JAM_API void jam_mat_add(mat44_t out, const mat44_t a, const mat44_t b);


	/** 矩阵相减 */
	JAM_API void jam_mat_sub(mat44_t out, const mat44_t a, const mat44_t b);


	/** 矩阵相乘 */
	JAM_API void jam_mat_mul(mat44_t out, const mat44_t a, const mat44_t b);


	/** 矩阵与三维向量相乘 */
	JAM_API void jam_mat_mul_vec3(vec3_t out, const mat44_t a, const vec3_t b);


	/** 矩阵与四维向量相乘 */
	JAM_API void jam_mat_mul_vec4(vec4_t out, const mat44_t a, const vec4_t b);


	/** 矩阵缩放 */
	JAM_API void jam_mat_scale(mat44_t out, const mat44_t a, float s);


	/** 逆矩阵 */
	JAM_API void jam_mat_inverse(mat44_t out, const mat44_t a);


	/** 伴随矩阵 */
	JAM_API void jam_mat_adjoint(mat44_t out, const mat44_t a);


	/** 行列互换 */
	JAM_API void jam_mat_transpose(mat44_t out, const mat44_t mat);


	/** 行列式 */
	JAM_API float jam_mat_determinant(const mat44_t mat);


	/** 设置缩放矩阵 */
	JAM_API void jam_mat_set_scaling(mat44_t out, const vec3_t v);


	/** 设置平移矩阵 */
	JAM_API void jam_mat_set_translation(mat44_t out, const vec3_t v);


	/** 通过旋转轴计算矩阵, 向量必须是归一化的 */
	JAM_API void jam_mat_from_angle(mat44_t out, const vec3_t axis, float angle);


	/** 通过矩阵获取向量和角度 */
	JAM_API void jam_mat_to_angle(vec3_t v, float* angle, const mat44_t mat);


	/** 通过旋转轴计算矩阵 */
	JAM_API void jam_mat_from_yaw_pitch_roll(mat44_t out, float yaw, float pitch, float roll);


	/** 通过矩阵获取旋转轴 */
	JAM_API void jam_mat_to_yaw_pitch_roll(float* yaw, float* pitch, float* roll, const mat44_t mat);


	/** 通过旋转轴计算矩阵, 向量必须是归一化的 */
	JAM_API void jam_mat_from_axes(mat44_t out, const vec3_t x, const vec3_t y, const vec3_t z);


	/** 通过矩阵获取旋转轴 */
	JAM_API void jam_mat_to_axes(vec3_t x, vec3_t y, vec3_t z, const mat44_t mat);


	/** 矩阵转四元数 */
	JAM_API void jam_mat_to_quat(quat_t out, const mat44_t mat);


	/** 获取视角矩阵 */
	JAM_API void jam_mat_from_lookat(mat44_t out, const vec3_t eye, const vec3_t at, const vec3_t up);


	/** 获取透视视景体矩阵 */
	JAM_API void jam_mat_from_perspective(mat44_t out, float w, float h, float nearp, float farp);


	/** 获取透视视景体矩阵 */
	JAM_API void jam_mat_from_perspective_center(mat44_t out, float left, float right, float bottom, float top, float nearp, float farp);


	/** 获取透视视景体矩阵 */
	JAM_API void jam_mat_from_perspective_fov(mat44_t out, float fov, float aspect, float nearp, float farp);


	/** 获取平行视景体矩阵 */
	JAM_API void jam_mat_from_ortho(mat44_t out, float w, float h, float nearp, float farp);


	/** 获取平行视景体矩阵 */
	JAM_API void jam_mat_from_ortho_center(mat44_t out, float left, float right, float bottom, float top, float nearp, float farp);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MATRIX_H__ */
