
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Mouse Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_MOUSE_H__
#define __JAM_MOUSE_H__


#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	/** 设置鼠标坐标 */
	JAM_API void jam_cursor_pos(int32_t x, int32_t y);


	/** 获取鼠标坐标 X 轴 */
	JAM_API int32_t jam_cursor_x();


	/** 获取鼠标坐标 Y 轴 */
	JAM_API int32_t jam_cursor_y();


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_MOUSE_H__ */
