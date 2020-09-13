/********************************************************************
	created:	2010/01/18
	created:	18:1:2010   18:29
	filename: 	e:\loong\trunk\LoongWorld\player_regression_define.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	player_regression_define
	file ext:	h
	author:		Jason
	
	purpose:	
	history:	1、2010-12-21圣魔修改
*********************************************************************/

#ifndef player_regression_define_h__
#define player_regression_define_h__

struct tagPlayRegressionRule
{
	INT _time_low;
	INT _time_hight;
	INT _level_low;
	INT _level_hight;
	DWORD	_item_type;	// 全都是礼包，暂时不需要品级与数量，默认都是1
	tagPlayRegressionRule()
		:_time_low(0),_time_hight(0),_level_low(0),_level_hight(0),_item_type(GT_INVALID)
	{
	}
	tagPlayRegressionRule(const tagPlayRegressionRule & t)
		:_time_low(t._time_low),_time_hight(t._time_hight),_level_low(t._level_low),_level_hight(t._level_hight),_item_type(t._item_type)
	{
	}
	tagPlayRegressionRule & operator = (const tagPlayRegressionRule & t)
	{
		_time_low = t._time_low;
		_time_hight = t._time_hight;
		_level_low = t._level_low;
		_level_hight = t._level_hight;
		_item_type = t._item_type;
		return *this;
	}
};


#endif // player_regression_define_h__
