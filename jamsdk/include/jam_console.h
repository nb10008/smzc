
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Console Utility Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_CONSOLE_H__
#define __JAM_CONSOLE_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint32_t					csl_color_t;

	#define JAM_FG_RED					(1 << 0)		/** 控制台红色文字 */
	#define JAM_FG_GREEN				(1 << 1)		/** 控制台绿色文字 */
	#define JAM_FG_BLUE					(1 << 2)		/** 控制台蓝色文字 */
	#define JAM_BG_RED					(1 << 3)		/** 控制台红色背景 */
	#define JAM_BG_GREEN				(1 << 4)		/** 控制台绿色背景 */
	#define JAM_BG_BLUE					(1 << 5)		/** 控制台蓝色背景 */



	/** 禁用控制台上的关闭按钮 */
	JAM_API void jam_console_no_close();


	/** 禁用控制台 CTRL + C 中断 */
	JAM_API void jam_console_no_ctrl_c();


	/** 设置控制台标题 */
	JAM_API void jam_console_set_title(const char* title);


	/** 获取控制台标题 */
	JAM_API size_t jam_console_get_title(char* title, size_t len);


	/** 设置控制台字体颜色 */
	JAM_API void jam_console_set_color(csl_color_t color);


	/** 获取控制台字体颜色 */
	JAM_API csl_color_t jam_console_get_color();


	/** 从控制台获取输入信息 */
	JAM_API size_t jam_console_line(char* buffer, size_t len, bool print);



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_CONSOLE_H__ */
