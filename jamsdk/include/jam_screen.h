
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Screen Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_SCREEN_H__
#define __JAM_SCREEN_H__

#include "jam_camera.h"
#include "jam_viewport.h"


#ifdef __cplusplus
extern "C" {
#endif


	#define JAM_STYLE_DEFAULT						0
	#define JAM_STYLE_FULLSCREEN					(1 << 0)
	#define JAM_STYLE_NO_CLOSE						(1 << 1)
	#define JAM_STYLE_NO_MAX						(1 << 2)
	#define JAM_STYLE_NO_MIN						(1 << 3)
	#define JAM_STYLE_NO_BORDER						(1 << 4)
	#define JAM_STYLE_NO_SIZE						(1 << 5)



	/** 每帧回调, 参数为贞时间, 单位秒 */
	typedef void (*on_frame_move_t)(float elapse);
	typedef bool (*on_screen_close_t)();


	/** 初始化渲染器主窗口 */
	JAM_API bool jam_screen_init(uint32_t style, int32_t width, int32_t height);


	/** 清理渲染器主窗口 */
	JAM_API bool jam_screen_cleanup();


	/** 关闭主窗口 */
	JAM_API void jam_screen_close();


	/** 渲染画面 */
	JAM_API void jam_screen_loop(on_frame_move_t frame_move, on_screen_close_t on_close);



	/** 设置主窗口标题 */
	JAM_API void jam_screen_set_title(const char* title);


	/** 获取主窗口标题 */
	JAM_API const char* jam_screen_get_title();


	/** 当前主窗口是否被激活 */
	JAM_API bool jam_screen_active();


	/** 当前主窗口样式 */
	JAM_API uint32_t jam_screen_style();


	/** 当前主窗口 X 坐标 */
	JAM_API int32_t jam_screen_x();


	/** 当前主窗口 Y 坐标 */
	JAM_API int32_t jam_screen_y();


	/** 当前主窗口宽度 */
	JAM_API int32_t jam_screen_width();


	/** 当前主窗口高度 */
	JAM_API int32_t jam_screen_height();


	/** 当前主窗口色深 */
	JAM_API int32_t jam_screen_color_depth();


	/** 桌面分辨率宽度 */
	JAM_API int32_t jam_screen_desktop_width();


	/** 桌面分辨率高度 */
	JAM_API int32_t jam_screen_desktop_height();


	/** 设置主窗口鼠标坐标 */
	JAM_API void jam_screen_cursor_pos(int32_t x, int32_t y);


	/** 获取主窗口鼠标坐标 X 轴 */
	JAM_API int32_t jam_screen_cursor_x();


	/** 获取主窗口鼠标坐标 Y 轴 */
	JAM_API int32_t jam_screen_cursor_y();


	/** 获取主窗口鼠标滚轮坐标 */
	JAM_API int32_t jam_screen_cursor_z();


	/** 获取窗口帧数 */
	JAM_API float jam_screen_fps();


	/** 添加一个视口 */
	JAM_API viewport_t jam_screen_viewport_add(camera_t cam, int32_t z_order, float left, float top, float width, float height);


	/** 获取一个视口 */
	JAM_API viewport_t jam_screen_viewport_get(int32_t z_order);


	/** 按位置获取一个视口 */
	JAM_API viewport_t jam_screen_viewport_pos_get(int32_t idx);


	/** 删除一个视口 */
	JAM_API bool jam_screen_viewport_erase(int32_t z_order);


	/** 视口个数 */
	JAM_API uint32_t jam_screen_viewport_size();


	/** 删除所有视口 */
	JAM_API void jam_screen_viewport_clear();


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_SCREEN_H__ */
