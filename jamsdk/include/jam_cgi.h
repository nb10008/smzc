
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Easy CGI Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_CGI_H__
#define __JAM_CGI_H__

#include "jam_datatype.h"
#include <stdarg.h>


#ifdef __cplusplus
extern "C" {
#endif


	typedef void (*on_page_t)();


	/** 运行 FastCGI 程序 */
	JAM_API int32_t jam_cgi_run(on_page_t on_page);


	/** 输出内容到浏览器 */
	JAM_API void jam_cgi_output(const char* format, ...);
	/** 输出内容到浏览器 */
	JAM_API void jam_cgi_voutput(const char* format, va_list argv);


	/** HTTP 协议头 */
	JAM_API void jam_cgi_header();


	/** HTTP 重定向, 之前不能输出任何内容到浏览器 */
	JAM_API void jam_cgi_redirect(const char* url);


	/** 包含一个网页 */
	JAM_API bool jam_cgi_include(const char* file);


	/** 获取页面参数 */
	JAM_API const char* jam_cgi_param(const char* name);


	/** 添加 COOKIE, time 单位: 秒, 默认 0 */
	JAM_API bool jam_cgi_cookie_put(const char* name, const char* value, uint32_t time);


	/** 查询 COOKIE */
	JAM_API const char* jam_cgi_cookie_get(const char* name);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_CGI_H__ */
