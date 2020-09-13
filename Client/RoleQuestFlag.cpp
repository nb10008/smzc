#include "stdafx.h"
#include "RoleQuestFlag.h"

RoleQuestFlag::RoleQuestFlag(void)
{
	m_curFrame = 0;
	m_frameTime = 0;
}

RoleQuestFlag::~RoleQuestFlag(void)
{
	Destroy();
}

void RoleQuestFlag::Create()
{
	TCHAR szFlag[_MAX_PATH]={0};
	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	for(int i=0;i<4;i++)
	{
        // ½Ó
		_stprintf(szFlag, _T("data\\ui\\mark\\Quest\\loong-renwu-%c.tga"), (TCHAR)(_T('e')+i));
		ResTexture *newTex = IDraw2D::Inst()->NewTexture(szFlag,pIFS);
		m_picArray0.push_back(newTex);

        // ½»
		_stprintf(szFlag, _T("data\\ui\\mark\\Quest\\loong-renwu-%c.tga"), (TCHAR)(_T('a')+i));
		newTex = IDraw2D::Inst()->NewTexture(szFlag, pIFS);
		m_picArray1.push_back(newTex);

        // ¶Ò
        _stprintf(szFlag, _T("data\\ui\\mark\\Quest\\loong-renwu-%c.tga"), (TCHAR)(_T('i')+i));
        newTex = IDraw2D::Inst()->NewTexture(szFlag, pIFS);
        m_picArray2.push_back(newTex);

        // ÖØ
        _stprintf(szFlag, _T("data\\ui\\mark\\Quest\\loong-renwu-%c.tga"), (TCHAR)(_T('m')+i));
        newTex = IDraw2D::Inst()->NewTexture(szFlag, pIFS);
        m_picArray3.push_back(newTex);
	}
}

void RoleQuestFlag::Destroy()
{
	for (int i=0;i<4;i++)
	{
		IDraw2D::Inst()->ReleaseTexture(m_picArray0[i]);
		IDraw2D::Inst()->ReleaseTexture(m_picArray1[i]);
        IDraw2D::Inst()->ReleaseTexture(m_picArray2[i]);
        IDraw2D::Inst()->ReleaseTexture(m_picArray3[i]);
	}
	m_picArray0.clear();
	m_picArray1.clear();
    m_picArray2.clear();
    m_picArray3.clear();
}

void RoleQuestFlag::Update()
{
	float FPS = 5;

	m_frameTime += Kernel::Inst()->GetDeltaTime();

	if(m_frameTime > 1.0f/FPS)
	{
		m_curFrame++;
		m_frameTime -= 1.0f/FPS;
	}

	if(m_curFrame >= 4)
		m_curFrame = 0;

}

void RoleQuestFlag::Draw(POINT pt,float z,float scale,EQuestFlag qf)
{
	if( qf == EQF_Null )
		return;

	ASSERT(m_curFrame < (int)m_picArray0.size());
	ASSERT(m_curFrame < (int)m_picArray1.size());
    ASSERT(m_curFrame < (int)m_picArray2.size());
    ASSERT(m_curFrame < (int)m_picArray3.size());

	ResTexture *pTex = NULL;

	switch(qf) 
	{
	case EQF_Accept:
		pTex = m_picArray0[m_curFrame];
		break;
	case EQF_Finish:
		pTex = m_picArray1[m_curFrame];
		break;
	case EQF_InAcceptability:
		break;
	case EQF_NotFinish:
		break;
    case EQF_RepeatAccept:
        pTex = m_picArray3[m_curFrame];
        break;
    case EQF_RepeatFinish:
        pTex = m_picArray2[m_curFrame];
        break;
	}

	if(pTex)
	{
		pt.x += (LONG)(18.0f * scale);
		pt.y -= (LONG)(43.0f * scale);

		RECT destRC,srcRC;
		float flagSize = 35.0f*scale;
		float centerY = (float)pt.y - 25.0f;

		srcRC.top=srcRC.left=0;
		srcRC.bottom=srcRC.top+71;
		srcRC.right=srcRC.left+71;

		destRC.left=pt.x-(LONG)flagSize;
		destRC.right=destRC.left+(LONG)flagSize;
		destRC.top=(LONG)(centerY - flagSize/2.0f);
		destRC.bottom=(LONG)(centerY + flagSize/2.0f);

		IDraw2D::Inst()->Draw(&destRC, &srcRC, pTex, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, z);
	}
}