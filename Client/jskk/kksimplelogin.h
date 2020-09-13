/* -------------------------------------------------------------------------
// 文件名		:	kksimplelogin/kksimplelogin.h
// 创建人		:	chenghui
// 创建时间		:	2011-4-18 15:19:34
// 功能描述     :	
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __KKSIMPLELOGIN_KKSIMPLELOGIN_H__
#define __KKSIMPLELOGIN_KKSIMPLELOGIN_H__

// -------------------------------------------------------------------------
// 返回值定义
#define KK_SIMPLELOGIN_SUCCESS			0	// 成功
#define KK_SIMPLELOGIN_FAILED			1	// 未知错误
#define KK_SIMPLELOGIN_KK_NO_EXIST		2	// 快快没启动
#define KK_SIMPLELOGIN_DATA_ERROR		3	// 数据错误
#define KK_SIMPLELOGIN_UNLOGIN			4	// 未登录


// -------------------------------------------------------------------------
// 导出函数定义
EXTERN_C INT STDAPICALLTYPE kkGetActiveUserToken(
	WCHAR* strTokenBuffer,
	ULONG nBufferLength
	);

// -------------------------------------------------------------------------
// $Log: $

#endif /* __KKSIMPLELOGIN_KKSIMPLELOGIN_H__ */
