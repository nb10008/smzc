/* -------------------------------------------------------------------------
// 文件名		:	kksimpleloginhelper.h
// 创建人		:	chenghui
// 创建时间		:	2011-4-19 15:18:37
// 功能描述     :	
//
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef __KKSIMPLELOGINHELPER_H__
#define __KKSIMPLELOGINHELPER_H__

// -------------------------------------------------------------------------
/*
@fn isKKGameInstalled
@brief 用来获取快快安装目录，并确定快快是否安装
@arg strInstallPath
@return 
TRUE，快快已经安装；FALSE，快快没有安装
@remark
@*/
BOOL isKKGameInstalled(std::wstring& strInstallPath);

// -------------------------------------------------------------------------
// $Log: $

#endif /* __KKSIMPLELOGINHELPER_H__ */
