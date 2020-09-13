
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Time Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_TIME_H__
#define __JAM_TIME_H__

#include "jam_datatype.h"
#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint64_t						datetime_t;


	/** 获取系统当前时间戳 */
	JAM_API datetime_t jam_time_now();


	/** 按指定日期生成一个时间戳 */
	JAM_API datetime_t jam_time_make(int32_t year, int32_t month, int32_t day, 
									int32_t hour, int32_t minute, int32_t second, int32_t millisecond);


	/** 转换C时间戳到当前时间戳 */
	JAM_API datetime_t jam_ctime_to_datetime(time_t time);


	/** 计算两个时间间隔(单位: 毫秒) */
	JAM_API uint64_t jam_time_diff(datetime_t a, datetime_t b);


	/** 年(1 - 4095) */
	JAM_API int32_t jam_time_year(datetime_t datetime);


	/** 月(1 - 12) */
	JAM_API int32_t jam_time_month(datetime_t datetime);


	/** 日(1 - 31) */
	JAM_API int32_t jam_time_day(datetime_t datetime);


	/** 时(0 - 23) */
	JAM_API int32_t jam_time_hour(datetime_t datetime);


	/** 分(0 - 59) */
	JAM_API int32_t jam_time_minute(datetime_t datetime);


	/** 秒(0 - 59) */
	JAM_API int32_t jam_time_second(datetime_t datetime);


	/** 毫秒(0 - 999) */
	JAM_API int32_t jam_time_millisecond(datetime_t datetime);


	/** 判断某年是否为闰年 */
	JAM_API bool jam_time_is_leap_year(int32_t year);


	/** 获取某年总天数 */
	JAM_API int32_t jam_time_day_of_year(int32_t year);


	/** 获取某年某月总天数 */
	JAM_API int32_t jam_time_day_of_month(int32_t year, int32_t month);



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_TIME_H__ */
