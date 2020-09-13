
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Image Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_IMAGE_H__
#define __JAM_IMAGE_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_IMG_BMP = 1,
		JAM_IMG_TGA
	}image_type_t;

	typedef handle_t				image_t;


	/** 读取一个图片 */
	JAM_API image_t jam_img_alloc(const char* filename, image_type_t type);


	/** 关闭一个图片 */
	JAM_API void jam_img_free(image_t img);


	/** 获取图片宽度 */
	JAM_API int32_t jam_img_width(image_t img);


	/** 获取图片高度 */
	JAM_API int32_t jam_img_height(image_t img);


	/** 获取像素位数 */
	JAM_API int32_t jam_img_bpp(image_t img);


	/** 将图片保存到磁盘上 */
	JAM_API bool jam_img_save(image_t img, const char* filename);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_IMAGE_H__ */
