#include <stdafx.h>
#include "DofRender.h"
#include "..\ResSys\ResMgr.h"
#include "..\ResSys\ResVertShader.h"
#include "..\ResSys\ResPixelShader.h"
#include "..\Device\IDevice.h"
#include "..\Device\IRenderTexture.h"
#include "..\Device\IPixelShader.h"
#include "..\Kernel.h"
#include "..\SceneGraph\HugeQuadTreeSG.h"
#include "RenderNode.h"
#include "..\Cool3DOption.h"
namespace Cool3D
{
    DofRender* DofRender::Inst()
    {
        static DofRender inst;
        return &inst;
    }

    DofRender::DofRender():
        m_pVS(NULL),
        m_pPS(NULL),
        m_pPostPS(NULL),
        m_pVb(NULL),
        m_pColorTex(NULL),
        m_pDepthTex(NULL),
        m_radius(1.0f)
    {

    }
    DofRender::~DofRender()
    {

    }

    void DofRender::Create()
    {
        if(!IsHWSurpport())
            return;
        m_pPS = ResMgr::Inst()->NewPS(_T("data\\system\\shader\\dof.ps"), MAKELONG(2, 0));
        m_pVS = ResMgr::Inst()->NewVS(_T("data\\system\\shader\\dof.vs"),MAKELONG(2, 0));
        m_pPostPS = ResMgr::Inst()->NewPS(_T("data\\system\\shader\\dof_post.ps"),MAKELONG(3, 0));
        m_pDepthTex = Device()->NewRenderTexture();
        bool bsucceed = m_pDepthTex->Create(Device()->GetClientWidth(), Device()->GetClientHeight(), EPF_R32F, true);
        m_pColorTex = Device()->NewRenderTexture();
        bsucceed = m_pColorTex->Create(Device()->GetClientWidth(), Device()->GetClientHeight(), EPF_A8R8G8B8, false);

        m_vertStream.Create(EVType_PtT, 4);
        Vert_PtT* pVert = (Vert_PtT*)m_vertStream.GetRawBuffer();

        pVert[0].pos = Vector4(-0.5f, -0.5f, 1.0f, 1.0f);
        pVert[1].pos = Vector4(Device()->GetClientWidth()-0.5f, -0.5f, 1.0f, 1.0f);
        pVert[2].pos = Vector4(-0.5f, Device()->GetClientHeight()-0.5f, 1.0f, 1.0f);
        pVert[3].pos = Vector4(Device()->GetClientWidth()-0.5f, Device()->GetClientHeight()-0.5f, 1.0f, 1.0f);
        pVert[0].uv  = Vector2(0.0f, 0.0f);
        pVert[1].uv  = Vector2(1, 0.0f);
        pVert[2].uv  = Vector2(0.0f, 1);
        pVert[3].uv  = Vector2(1, 1);

        m_pVb = Device()->NewVB();
        m_pVb->Create(m_vertStream.GetBufferSize(), EB_DynamicWriteOnly, m_vertStream.GetStride());
        m_pVb->CopyFromVertStream( &m_vertStream );


    }

    void DofRender::Destroy()
    {
        RES_MGR_RELEASE_PS(m_pPS);
        RES_MGR_RELEASE_PS(m_pPostPS);
        RES_MGR_RELEASE_VS(m_pVS);
        SAFE_DELETE(m_pDepthTex);
        SAFE_DELETE(m_pColorTex);
        SAFE_DELETE(m_pVb);
    }

    bool DofRender::MakeDepthMap(HugeQuadTreeSG* pSG)
    {
        if(!Cool3DOption::Inst()->GetOption(Cool3DOption::EO_DepthofField)==Cool3DOption::EDOF_Open)
        {
            return true;
        }

        if(!m_pPS->IsCreated()||!m_pVS->IsCreated())
        {
            hasDepthTex = false;
            return false;
        }
        RenderNode::m_pSpecialPS = m_pPS->GetPS();
        RenderNode::m_pSpecialVS = m_pVS->GetVS();
        pSG->RenderDepthMap(m_pDepthTex);
        hasDepthTex = true;
        RenderNode::m_pSpecialPS = NULL;
        RenderNode::m_pSpecialVS = NULL;
        return true;
    }

    void DofRender::PostProcess()
    {
        if(!Cool3DOption::Inst()->GetOption(Cool3DOption::EO_DepthofField)==Cool3DOption::EDOF_Open)
        {
            return;
        }
        static Vector4 sample_point[] =
        {
            Vector4(1.0f,  0.0f,    0.0f, 0.0f),
            Vector4( 0.5f,  0.8660f, 0.0f, 0.0f),
            Vector4(-0.5f,  0.8660f, 0.0f, 0.0f),
            Vector4(-1.0f,  0.0f,    0.0f, 0.0f),
            Vector4(-0.5f, -0.8660f, 0.0f, 0.0f),
            Vector4( 0.5f, -0.8660f, 0.0f, 0.0f),

             Vector4( 1.5f,  0.8660f, 0.0f, 0.0f),
             Vector4( 0.0f,  1.7320f, 0.0f, 0.0f),
             Vector4(-1.5f,  0.8660f, 0.0f, 0.0f),
             Vector4(-1.5f, -0.8660f, 0.0f, 0.0f),
             Vector4( 0.0f, -1.7320f, 0.0f, 0.0f),
             Vector4( 1.5f, -0.8660f, 0.0f, 0.0f)
        };
        if(!m_pPS->IsCreated()||!m_pVS->IsCreated()||!hasDepthTex)
        {
            return;
        }
        IDevice *pDev = Device();
        StateBlock block;
        block.SaveState(ERS_FFogEnable);		pDev->FEnableFog(false);
        block.SaveState(ERS_FLightEnable);		pDev->FEnableLight(false);
        block.SaveState(ERS_DepthTestEnable);	pDev->EnableDepthTest(false);
        block.SaveState(ERS_DepthWriteEnable);	pDev->EnableDepthWrite(false);
        block.SaveState(ERS_CullMode);			pDev->SetCullMode(ECull_None);
        block.SaveState(ERS_AlphaBlendEnable);	pDev->EnableAlphaBlend(false);
        block.SaveState(ERS_AlphaTestEnable);	pDev->EnableAlphaTest(false);

        pDev->SetSimplerAddress(0,ETAddress_Clamp,ETAddress_Clamp,ETAddress_Clamp);
        pDev->SetSimplerFilter(0,ETFilter_LINEAR,ETFilter_LINEAR);
        pDev->SetSimplerMipFilter(0,ETFilter_NONE);
        pDev->FSetUVTrans(0,EUVT_Disable);

        pDev->SetStreamDefine(Kernel::Inst()->GetRenderSys()->GetStreamDefine(EVType_PtT));

        pDev->SetSimplerAddress(1,ETAddress_Clamp,ETAddress_Clamp,ETAddress_Clamp);
        pDev->SetSimplerFilter(1,ETFilter_LINEAR,ETFilter_LINEAR);
        pDev->SetSimplerMipFilter(1,ETFilter_NONE);
        pDev->FSetUVTrans(1,EUVT_Disable);

        pDev->SetVB(0, m_pVb);
        pDev->SetStreamDefine(Kernel::Inst()->GetRenderSys()->GetStreamDefine(EVType_PtT));
        pDev->SetVertShader(NULL);
        pDev->SetPixelShader(m_pPostPS->GetPS());
        Vector4 scale (m_radius/(float)pDev->GetClientWidth(), m_radius/(float)pDev->GetClientHeight(), 0.0f, 0.0f);
        m_pPostPS->GetPS()->SetVector("g_scale", &scale);
        m_pPostPS->GetPS()->SetVectorArray("sample_point", sample_point, sizeof(sample_point)/sizeof(sample_point[0]));
        pDev->CopyFromBackbuffer(m_pColorTex, NULL, NULL);
        pDev->SetTexture(0, m_pColorTex);
        pDev->SetTexture(1, m_pDepthTex);
        pDev->Draw(EDraw_TriangleStrip, 2);

        pDev->SetTexture(0, (IDeviceTex*)NULL);
        pDev->SetTexture(1, (IDeviceTex*)NULL);
        pDev->SetPixelShader(NULL);
        block.Apply();

    }

    bool DofRender::IsHWSurpport()
    {
        IDevice *pDev = Device();
        return pDev->CapsVertexShaderVersion() >= D3DVS_VERSION(2,0)&&
                pDev->CapsPixelShaderVersion() >= D3DPS_VERSION(3,0);
    }
}