#pragma once
#include "..\ResSys\VertStream.h"
namespace Cool3D
{
    class ResPixelShader;
    class ResVertShader;
    class IRenderTexture;
    class HugeQuadTreeSG;
    class IVertBuffer;
    class Cool3D_API DofRender
    {
    public:
        static DofRender* Inst();
        void Destroy();
        void Create();
        bool MakeDepthMap(HugeQuadTreeSG* pSG);
        void PostProcess();
        bool IsHWSurpport();
        float GetRadius() const { return m_radius; }
        void SetRadius(float val) { m_radius = val; }
    private:
        DofRender();
        ~DofRender();
        ResPixelShader *m_pPS;
        ResPixelShader *m_pPostPS;
        ResVertShader *m_pVS;
        bool hasDepthTex;
        IRenderTexture *m_pDepthTex;
        IRenderTexture *m_pColorTex;

        IVertBuffer *m_pVb;
        VertStream m_vertStream;
        float m_radius;
    };
}