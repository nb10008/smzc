
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2009 Jampe.Net.
 *			http://jampe.net/
 *
 *			Color Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_COLOR_H__
#define __JAM_COLOR_H__

#include "jam_math.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef uint32_t				color_t;


	#define jam_argb(a, r, g, b)					jam_make_dword(jam_make_word(a, r), jam_make_word(g, b))
	#define jam_argb_alpha(clr)						jam_high_byte(jam_high_word(clr))
	#define jam_argb_red(clr)						jam_low_byte(jam_high_word(clr))
	#define jam_argb_green(clr)						jam_high_byte(jam_low_word(clr))
	#define jam_argb_blue(clr)						jam_low_byte(jam_low_word(clr))

	#define jam_argb_f(a, r, g, b)					jam_argb(jam_clamp(a, 0.0f, 1.0f) * 255.0f,			\
															 jam_clamp(r, 0.0f, 1.0f) * 255.0f,			\
															 jam_clamp(g, 0.0f, 1.0f) * 255.0f,			\
															 jam_clamp(b, 0.0f, 1.0f) * 255.0f)
	#define jam_argb_alpha_f(clr)					((float)jam_argb_alpha(clr) / 255.0f)
	#define jam_argb_red_f(clr)						((float)jam_argb_red(clr) / 255.0f)
	#define jam_argb_green_f(clr)					((float)jam_argb_green(clr) / 255.0f)
	#define jam_argb_blue_f(clr)					((float)jam_argb_blue(clr) / 255.0f)


	#define jam_rgba(r, g, b, a)					jam_make_dword(jam_make_word(r, g), jam_make_word(b, a))
	#define jam_rgba_red(clr)						jam_high_byte(jam_high_word(clr))
	#define jam_rgba_green(clr)						jam_low_byte(jam_high_word(clr))
	#define jam_rgba_blue(clr)						jam_high_byte(jam_low_word(clr))
	#define jam_rgba_alpha(clr)						jam_low_byte(jam_low_word(clr))

	#define jam_rgba_f(r, g, b, a)					jam_argb(jam_clamp(r, 0.0f, 1.0f) * 255.0f,			\
															 jam_clamp(g, 0.0f, 1.0f) * 255.0f,			\
															 jam_clamp(b, 0.0f, 1.0f) * 255.0f,			\
															 jam_clamp(a, 0.0f, 1.0f) * 255.0f)
	#define jam_rgba_red_f(clr)						((float)jam_rgba_red(clr) / 255.0f)
	#define jam_rgba_green_f(clr)					((float)jam_rgba_green(clr) / 255.0f)
	#define jam_rgba_blue_f(clr)					((float)jam_rgba_blue(clr) / 255.0f)
	#define jam_rgba_alpha_f(clr)					((float)jam_rgba_alpha(clr) / 255.0f)



#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_COLOR_H__ */
