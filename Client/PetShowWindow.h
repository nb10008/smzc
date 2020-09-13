#pragma once

#include "..\WorldDefine\pet_define.h"

//! \class  PetShowWindow
//! \brief  宠物模型展台
//! \author hyu
//! \date   2009-6-17
//! \last   2009-6-17
class PetShowWindow :
    public GUIStatic
{
public:
    PetShowWindow(void);
    virtual ~PetShowWindow(void);

    // 重写基类
    virtual BOOL Init(GUISystem* pSystem, GUIWnd* pFather, XmlElement* pXmlElement);
    virtual VOID Destroy();
    virtual VOID Update();
    virtual VOID Render();
    virtual VOID RefreshViewCoord();
    virtual VOID SetInvisible(BOOL b);
	virtual VOID AddChild(GUIWnd* pChild);//add by xp

    //! \brief  渲染宠物模型
    void Render3D();

    //! \brief  旋转,累积型变量
    void RotateYaw(FLOAT fAngle);

    //! \func   SetPetShow
    //! \param  dwPetTypeID     宠物原型id
    //! \param  eModelType      宠物模型类弄
    //! \brief  设置显示宠物
    void SetPetShow(DWORD dwPetTypeID, EPetModelType eModelType = EPMT_Follow);

	VOID SetModelYaw(float fNum);

protected:

    //! \brief  更新摄像机
    virtual void UpdateCamera();

    //! \brief  加载模型
    EntityNode* LoadEntity(LPCTSTR szFileName);
    void LoadSkin(LPCTSTR szFileName, EntityNode* pEntityNode);

protected:
	tagGUIImage*			m_pNewCanvas;       // RenderTarget
	EntityNode*				m_pPetNode;         // SceneNode
	NullSceneGraph*			m_pSG;              // 用于保存摄像机、光照等信息,不用于渲染
    CameraBase				m_Camera;

private:
    DWORD                   m_dwCurrPetTypeId;  // 当前宠物id
    FLOAT                   m_fModelYaw;

	GUIButton*              m_pBnRotaLeft;
	GUIButton*              m_pBnRotaRight;

};
