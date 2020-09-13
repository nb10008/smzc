
/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *		Copyright (c) 2004-2008 Jampe.Net.
 *			http://jampe.net/
 *
 *			Keyboard Function
 *
 *						by Jampe
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef __JAM_KEYBOARD_H__
#define __JAM_KEYBOARD_H__


#include "jam_datatype.h"


#ifdef __cplusplus
extern "C" {
#endif


	typedef enum
	{
		JAM_KEY_UNKNOWN				=	0x00,		/* Unknown Button */
		JAM_KEY_LBUTTON				=	0x01,		/* Mouse Left Button */   
		JAM_KEY_RBUTTON				=	0x02,		/* Mouse Right Button */  
		JAM_KEY_MBUTTON				=	0x03,		/* Mouse Middle Button */ 

		JAM_KEY_ESCAPE				=	0x10,		/* ESC */				 
		JAM_KEY_F1					=	0x11,		/* F1 */				  
		JAM_KEY_F2					=	0x12,		/* F2 */				  
		JAM_KEY_F3					=	0x13,		/* F3 */				  
		JAM_KEY_F4					=	0x14,		/* F4 */				  
		JAM_KEY_F5					=	0x15,		/* F5 */				  
		JAM_KEY_F6					=	0x16,		/* F6 */				  
		JAM_KEY_F7					=	0x17,		/* F7 */				  
		JAM_KEY_F8					=	0x18,		/* F8 */				  
		JAM_KEY_F9					=	0x19,		/* F9 */				  
		JAM_KEY_F10					=	0x1A,		/* F10 */				 
		JAM_KEY_F11					=	0x1B,		/* F11 */				 
		JAM_KEY_F12					=	0x1C,		/* F12 */				 

		JAM_KEY_BACK				=	0x20,		/* Back Space */		  
		JAM_KEY_TAB					=	0x21,		/* TAB */				 
		JAM_KEY_ENTER				=	0x22,		/* Enter */			   
		JAM_KEY_SHIFT				=	0x23,		/* SHIFT */			   
		JAM_KEY_LSHIFT				=	0x24,		/* Left Shift */		  
		JAM_KEY_RSHIFT				=	0x25,		/* Right Shift */		 
		JAM_KEY_CTRL				=	0x26,		/* CTRL */				
		JAM_KEY_LCTRL				=	0x27,		/* Left CTRL */		   
		JAM_KEY_RCTRL				=	0x28,		/* Right CTRL */		  
		JAM_KEY_ALT					=	0x29,		/* ALT */				 
		JAM_KEY_LALT				=	0x2A,		/* Left ALT */			
		JAM_KEY_RALT				=	0x2B,		/* Right ALT */		   
		JAM_KEY_PAUSE				=	0x2C,		/* Pause Break */		 
		JAM_KEY_SPACE				=	0x2D,		/* Space */			   

		JAM_KEY_0					=	0x30,		/* 0 */				   
		JAM_KEY_1					=	0x31,		/* 1 */				   
		JAM_KEY_2					=	0x32,		/* 2 */				   
		JAM_KEY_3					=	0x33,		/* 3 */				   
		JAM_KEY_4					=	0x34,		/* 4 */				   
		JAM_KEY_5					=	0x35,		/* 5 */				   
		JAM_KEY_6					=	0x36,		/* 6 */				   
		JAM_KEY_7					=	0x37,		/* 7 */				   
		JAM_KEY_8					=	0x38,		/* 8 */				   
		JAM_KEY_9					=	0x39,		/* 9 */				   

		JAM_KEY_A					=	0x41,		/* A */				   
		JAM_KEY_B					=	0x42,		/* B */				   
		JAM_KEY_C					=	0x43,		/* C */				   
		JAM_KEY_D					=	0x44,		/* D */				   
		JAM_KEY_E					=	0x45,		/* E */				   
		JAM_KEY_F					=	0x46,		/* F */				   
		JAM_KEY_G					=	0x47,		/* G */				   
		JAM_KEY_H					=	0x48,		/* H */				   
		JAM_KEY_I					=	0x49,		/* I */				   
		JAM_KEY_J					=	0x4A,		/* J */				   
		JAM_KEY_K					=	0x4B,		/* K */				   
		JAM_KEY_L					=	0x4C,		/* L */				   
		JAM_KEY_M					=	0x4D,		/* M */				   
		JAM_KEY_N					=	0x4E,		/* N */				   
		JAM_KEY_O					=	0x4F,		/* O */				   
		JAM_KEY_P					=	0x50,		/* P */				   
		JAM_KEY_Q					=	0x51,		/* Q */				   
		JAM_KEY_R					=	0x52,		/* R */				   
		JAM_KEY_S					=	0x53,		/* S */				   
		JAM_KEY_T					=	0x54,		/* T */				   
		JAM_KEY_U					=	0x55,		/* U */				   
		JAM_KEY_V					=	0x56,		/* V */				   
		JAM_KEY_W					=	0x57,		/* W */				   
		JAM_KEY_X					=	0x58,		/* X */				   
		JAM_KEY_Y					=	0x59,		/* Y */				   
		JAM_KEY_Z					=	0x5A,		/* Z */				   

		JAM_KEY_NUM0				=	0x60,		/* 0 */				   
		JAM_KEY_NUM1				=	0x61,		/* 1 */				   
		JAM_KEY_NUM2				=	0x62,		/* 2 */				   
		JAM_KEY_NUM3				=	0x63,		/* 3 */				   
		JAM_KEY_NUM4				=	0x64,		/* 4 */				   
		JAM_KEY_NUM5				=	0x65,		/* 5 */				   
		JAM_KEY_NUM6				=	0x66,		/* 6 */				   
		JAM_KEY_NUM7				=	0x67,		/* 7 */				   
		JAM_KEY_NUM8				=	0x68,		/* 8 */				   
		JAM_KEY_NUM9				=	0x69,		/* 9 */				   
		JAM_KEY_MULTIPLY			=	0x6A,		/* * */				   
		JAM_KEY_ADD					=	0x6B,		/* + */				   
		JAM_KEY_SUBTRACT			=	0x6C,		/* - */				   
		JAM_KEY_DECIMAL				=	0x6D,		/* . */				   
		JAM_KEY_DIVIDE				=	0x6E,		/* / */				   

		JAM_KEY_SEMICOLON			=	0x70,		/* ; */				   
		JAM_KEY_PLUS				=	0x71,		/* + */				   
		JAM_KEY_COMMA				=	0x72,		/* , */				   
		JAM_KEY_MINUS				=	0x73,		/* - */				   
		JAM_KEY_PERIOD				=	0x74,		/* . */
		JAM_KEY_SLASH				=	0x75,		/* / */
		JAM_KEY_UNDEE				=	0x76,		/* ~ */
		JAM_KEY_LSBRACKET			=	0x77,		/* [ */
		JAM_KEY_RSBRACKET			=	0x78,		/* ] */
		JAM_KEY_BACKSLASH			=	0x79,		/* \ */
		JAM_KEY_QUOTATION			=	0x7A,		/* ' */

		JAM_KEY_PAGEUP				=	0x80,		/* Page UP */
		JAM_KEY_PAGEDOWN			=	0x81,		/* Page Down */
		JAM_KEY_END					=	0x82,		/* END */
		JAM_KEY_HOME				=	0x83,		/* HOME */
		JAM_KEY_LEFT				=	0x84,		/* LEFT */
		JAM_KEY_UP					=	0x85,		/* UP */
		JAM_KEY_RIGHT				=	0x86,		/* RIGHT */
		JAM_KEY_DOWN				=	0x87,		/* DOWN */
		JAM_KEY_SNAPSHOT			=	0x88,		/* Print Screen */
		JAM_KEY_INSERT				=	0x89,		/* Insert */
		JAM_KEY_DELETE				=	0x8A,		/* Delete */

		JAM_KEY_CAPS				=	0x90,		/* Caps Lock */
		JAM_KEY_NUMLOCK				=	0x91,		/* Num Lock */
		JAM_KEY_SCROLL				=	0x92,		/* Scroll Lock */

		JAM_KEY_LWIN				=	0xA0,		/* Left Win Key */
		JAM_KEY_RWIN				=	0xA1,		/* Right Win Key */
		JAM_KEY_APPS				=	0xA2		/* Win键与CTRL之间的键(叫什么我也不知道) */
	}keyboard_t;


	/** 键是否被按下 */
	JAM_API bool jam_key_is_down(keyboard_t key);


	/** 键是否被按住(前一帧没按, 当前帧按了) */
	JAM_API bool jam_key_is_press(keyboard_t key);


	/** 键是否被松开(前一帧按了, 当前帧没按) */
	JAM_API bool jam_key_is_release(keyboard_t key);


	/** 键是否被锁住(用于检测NUM LOCK, CAPS LOCK, SCROLL LOCK灯是否亮起) */
	JAM_API bool jam_key_is_locked(keyboard_t key);


	/** 更新按键状态 */
	JAM_API void jam_key_update();


#ifdef __cplusplus
}	/* extern "C" */
#endif

#endif	/* __JAM_KEYBOARD_H__ */
