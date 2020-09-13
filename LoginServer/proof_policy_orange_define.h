//-----------------------------------------------------------------------------
//!\file proof_policy_xunlei_define.h
//!\author songg
//!
//!\date 2009-06-22
//! last 2008-06-22
//!
//!\brief 台湾游戏橘子登录
//!
//!	Copyright (c) 2004 TENGWU Entertainment All rights reserved.
//-----------------------------------------------------------------------------
#pragma once

#pragma pack(push, 1)

//所有參數都是以單號分隔 ('). 每一個訊息最後都會加上 “\r\n” 作結尾.

//橘子的账号密码统一走guid 采用空格分隔 因为账号名要求可以采用邮箱消息中原有的账号名不够长

#define X_OG_SC_LEN 7 //ServiceCode
#define X_OG_REGION_LEN 3 //Region

#define X_OG_SEND_DIVIDE_SIGN '\'' //发送橘子消息的分隔符

#define X_OG_RECEIVE_DIVIDE_SIGN ';' //接收橘子消息的分隔符

#define X_CLIENT_RECEIVE_DIVIDE_SIGN ',' //接收客户端消息的分隔符


#define X_FROBID_MEMBER_CENTER "ServiceAccount_Locked" //账号被锁


//若為 WrongOTP（遊戲進入強制期，未完成升級的遊戲，仍以桌面捷徑啟動）
//則系統回傳〝請您關閉遊戲登入畫面，並進行GASH帳號升級，由beanfun!登入遊戲，網址為tw.beanfun.gamania.com〞
#define X_FROBID_WRONG_OTP "Wrong_OTP" //One time password is not correct

//〝您已升級為beanfun!帳號，請由beanFun登入〞
#define X_FROBID_WRONG_EVENT_OTP "Wrong_EventOTP"// One time pasword is not correct (for Beanfun event)


#pragma pack(pop)