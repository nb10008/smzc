
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Sound Runtime Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_SOUND_H__
#define __JAM_SOUND_H__

#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef handle_t						sound_t;

	typedef enum
	{
		JAM_SOUND_WAV = 1,
		JAM_SOUND_OGG,
		JAM_SOUND_MP3
	}sound_type_t;

	typedef enum
	{
		JAM_DISTANCE_MODEL = 1,
		JAM_INVERSE_DISTANCE,
		JAM_INVERSE_DISTANCE_CLAMPED,
		JAM_LINEAR_DISTANCE,
		JAM_LINEAR_DISTANCE_CLAMPED,
		JAM_EXPONENT_DISTANCE,
		JAM_EXPONENT_DISTANCE_CLAMPED
	}distance_model_t;

	typedef enum
	{
		JAM_SOUND_NO_ERROR = 0,
		JAM_SOUND_INVALID_NAME,
		JAM_SOUND_INVALID_ENUM,
		JAM_SOUND_INVALID_VALUE,
		JAM_SOUND_INVALID_COMMAND,
		JAM_SOUND_OUT_OF_MEMORY,
		JAM_SOUND_UNKNOWN
	}sound_error_t;



	/** 初始化声音设备 */
	JAM_API bool jam_sound_init();


	/** 关闭声音设备 */
	JAM_API bool jam_sound_cleanup();


	/** 获取最后个错误信息 */
	JAM_API sound_error_t jam_sound_last_error();


	/** 获取驱动名称 */
	JAM_API const char* jam_sound_vendor();


	/** 获取驱动版本 */
	JAM_API const char* jam_sound_version();


	/** 获取驱动厂商 */
	JAM_API const char* jam_sound_renderer();


	/** 获取驱动扩展 */
	JAM_API const char* jam_sound_extensions();


	/** 从文件名打开一个声音句柄 */
	JAM_API sound_t jam_sound_alloc(sound_type_t type, const char* filename);


	/** 从内存打开一个声音句柄, 仅支持 WAVE */
	JAM_API sound_t jam_sound_memory_alloc(sound_type_t type, const byte_t* buffer, size_t len);


	/** 关闭声音句柄 */
	JAM_API void jam_sound_free(sound_t snd);


	/** 获取声音类型 */
	JAM_API sound_type_t jam_sound_type(sound_t snd);


	/** 播放 */
	JAM_API bool jam_sound_play(sound_t snd);


	/** 停止 */
	JAM_API bool jam_sound_stop(sound_t snd);


	/** 暂停 */
	JAM_API bool jam_sound_pause(sound_t snd);


	/** 恢复 */
	JAM_API bool jam_sound_resume(sound_t snd);


	/** 重置 */
	JAM_API bool jam_sound_rewind(sound_t snd);


	/** 设置循环播放 */
	JAM_API bool jam_sound_loop(sound_t snd, bool loop);


	/** 等待声音播放完毕 */
	JAM_API void jam_sound_sync(sound_t snd);


	/** 是否正在播放 */
	JAM_API bool jam_sound_is_playing(sound_t snd);


	/** 是否已停止 */
	JAM_API bool jam_sound_is_stopped(sound_t snd);


	/** 是否已暂停 */
	JAM_API bool jam_sound_is_paused(sound_t snd);


	/** 获取频率 */
	JAM_API int32_t jam_sound_get_frequency(sound_t snd);


	/** 获取位数 */
	JAM_API int32_t jam_sound_get_bits(sound_t snd);


	/** 获取频道 */
	JAM_API int32_t jam_sound_get_channels(sound_t snd);


	/** 设置声源位置 */
	JAM_API void jam_sound_set_source_position(sound_t snd, float x, float y, float z);


	/** 获取声源位置 */
	JAM_API void jam_sound_get_source_position(sound_t snd, float* x, float* y, float* z);


	/** 设置声源速度 */
	JAM_API void jam_sound_set_source_velocity(sound_t snd, float x, float y, float z);


	/** 获取声源速度 */
	JAM_API void jam_sound_get_source_velocity(sound_t snd, float* x, float* y, float* z);


	/** 设置声源方向 */
	JAM_API void jam_sound_set_source_direction(sound_t snd, float x, float y, float z);


	/** 获取声源方向 */
	JAM_API void jam_sound_get_source_direction(sound_t snd, float* x, float* y, float* z);


	/** 设置声源位置是否与听众有联系 */
	JAM_API void jam_sound_set_source_relative(sound_t snd, bool rel);


	/** 获取声源位置是否与听众有联系 */
	JAM_API bool jam_sound_get_source_relative(sound_t snd);


	/** 设置听众位置 */
	JAM_API void jam_sound_set_listener_position(float x, float y, float z);


	/** 获取听众位置 */
	JAM_API void jam_sound_get_listener_position(float* x, float* y, float* z);


	/** 设置听众速度 */
	JAM_API void jam_sound_set_listener_velocity(float x, float y, float z);


	/** 获取听众速度 */
	JAM_API void jam_sound_get_listener_velocity(float* x, float* y, float* z);


	/** 设置听众方向 */
	JAM_API void jam_sound_set_listener_orientation(float at_x, float at_y, float at_z, float up_x, float up_y, float up_z);


	/** 获取听众方向 */
	JAM_API void jam_sound_get_listener_orientation(float* at_x, float* at_y, float* at_z, float* up_x, float* up_y, float* up_z);


	/** 设置声源增益 */
	JAM_API void jam_sound_set_gain(sound_t snd, float gain);


	/** 获取声源增益 */
	JAM_API float jam_sound_get_gain(sound_t snd);


	/** 设置声源最大增益 */
	JAM_API void jam_sound_set_max_gain(sound_t snd, float gain);


	/** 获取声源最大增益 */
	JAM_API float jam_sound_get_max_gain(sound_t snd);


	/** 设置声源最小增益 */
	JAM_API void jam_sound_set_min_gain(sound_t snd, float gain);


	/** 获取声源最小增益 */
	JAM_API float jam_sound_get_min_gain(sound_t snd);


	/** 设置声源频率(默认为1.0) */
	JAM_API void jam_sound_set_pitch(sound_t snd, float pitch);


	/** 获取声源频率 */
	JAM_API float jam_sound_get_pitch(sound_t snd);


	/** 设置声源的rolloff率 */
	JAM_API void jam_sound_set_roll_off(sound_t snd, float rolloff);


	/** 获取声源的rolloff率 */
	JAM_API float jam_sound_get_roll_off(sound_t snd);


	/** 设置距离模型 */
	JAM_API bool jam_sound_set_distance_model(distance_model_t model);


	/** 设置声源音量在距离上减少一半 */
	JAM_API void jam_sound_set_reference_distance(sound_t snd, float dis);


	/** 获取声源声音在距离上减少一半的距离 */
	JAM_API float jam_sound_get_reference_distance(sound_t snd);


	/**
	   设置最大距离
		(used with the Inverse Clamped Distance Model to set the distance 
		where there will no longer be any attenuation of the source)
	 */
	JAM_API void jam_sound_set_max_distance(sound_t snd, float dis);


	/** 获取最大距离 */
	JAM_API float jam_sound_get_max_distance(sound_t snd);


	/** 设置定向圆锥外部的增益 */
	JAM_API void jam_sound_set_cone_outer_gain(sound_t snd, float gain);


	/** 获取定向圆锥外部的增益 */
	JAM_API float jam_sound_get_cone_outer_gain(sound_t snd);


	/** 设置定向圆锥内部的增益 */
	JAM_API void jam_sound_set_cone_inner_gain(sound_t snd, float gain);


	/** 获取定向圆锥内部的增益 */
	JAM_API float jam_sound_get_cone_inner_gain(sound_t snd);


	/** 设置圆锥角度. 默认360度 */
	JAM_API void jam_sound_set_cone_angle(sound_t snd, float angle);


	/** 获取圆锥角度 */
	JAM_API float jam_sound_get_cone_angle(sound_t snd);


	/**
	   设置多普勒因子, 默认: 1.0f
		shift = DOPPLER_FACTOR * freq * (DOPPLER_VELOCITY - listener.velocity) / (DOPPLER_VELOCITY + source.velocity)
	 */
	JAM_API bool jam_sound_set_doppler_factor(float factor);


	/** 设置多普勒速度, 默认: 343.0 m/sec */
	JAM_API bool jam_sound_set_doppler_velocity(float velocity);


	/** 设置声音速度 */
	JAM_API bool jam_sound_set_speed(float speed);


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_SOUND_H__ */
