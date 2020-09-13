//--------------------------------------------------------------------------------------------
// Copyright (c) 2010 DACHENG Entertainment All rights reserved.
// filename: GUICameraData.h
// author: Su.Zhang
// actor:
// data: 2010-9-13
// last:
// brief: 界面特效摄像机参数和特效关联
//--------------------------------------------------------------------------------------------

#pragma once

struct tagGUICameraData
{
	INT			id;
	Vector3		V3CameraFrom;
	Vector3		V3CameraLookAt;
	Vector3		V3EffectPos;
	Vector3		V3EffectScale;
	tstring		strEffectName;
	INT			eMode;
};

class GUICameraData
{
public:
	typedef std::map<DWORD,tagGUICameraData> CameraDataMap;
public:
	GUICameraData(void);
	~GUICameraData(void);

	BOOL LoadFromFile();
	const tagGUICameraData* GetCameraData(DWORD id) const;
private:

	CameraDataMap m_mapCameraData;
};
