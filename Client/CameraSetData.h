#pragma once
#include "BaseData.h"

struct tagCameraData
{
	DWORD dwID;
	DWORD dwLoginCameraPos_x;		//登陆摄像机设置
	DWORD dwLoginCameraPos_y;
	DWORD dwLoginCameraPos_z;
	DWORD dwLoginCameraLookAt_x;	//摄像机朝向
	DWORD dwLoginCameraLookAt_y;
	DWORD dwLoginCameraLookAt_z;
	DWORD dwSelectRoleCameraPos_x;	//选人摄像机设置
	DWORD dwSelectRoleCameraPos_y;
	DWORD dwSelectRoleCameraPos_z;
	DWORD dwSelectRoleCameraLookAt_x;//选人摄像机朝向
	DWORD dwSelectRoleCameraLookAt_y;
	DWORD dwSelectRoleCameraLookAt_z;
	DWORD dwRole1Pos_x;				//角色1位置
	DWORD dwRole1Pos_y;
	DWORD dwRole1Pos_z;
	DWORD dwRole2Pos_x;				//角色2位置
	DWORD dwRole2Pos_y;
	DWORD dwRole2Pos_z;
	DWORD dwRole3Pos_x;				//角色3位置
	DWORD dwRole3Pos_y;
	DWORD dwRole3Pos_z;
	BOOL bIsShake;					//摄像机是否晃动
	DWORD dwCreateRolePos_x;		//创建人物，人物位置
	DWORD dwCreateRolePos_y;
	DWORD dwCreateRolePos_z;
	DWORD dwCreateCameraLookat_x;	//创建人物摄像机朝向
	DWORD dwCreateCameraLookat_y;
	DWORD dwCreateCameraLookat_z;
	DWORD dwCreateCameraNearLookat_x;//近镜头朝向
	DWORD dwCreateCameraNearLookat_y;
	DWORD dwCreateCameraNearLookat_z;
	DWORD dwCreateCamYaw;			//偏向角
	DWORD dwCreateCamPitch;
	DWORD dwCreateCamRoll;
	DWORD dwCreateMAXCamDist;		//距离

};

class CameraSetData : public BaseData
{
public:
	CameraSetData(void);
	~CameraSetData(void);

	static CameraSetData* Inst();

	/** 加载摄像机数据
	*/
	virtual void LoadFromFile();

	const tagCameraData* GetData()
	{
		return &m_pData;
	}

private:
	tagCameraData m_pData;

};
