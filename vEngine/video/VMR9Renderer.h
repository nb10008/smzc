#pragma once
#include "BaseVideoRenderer.h"
struct IVMRWindowlessControl9;
namespace vEngine{
    class VMR9Renderer :
        public BaseVideoRenderer
    {
        IVMRWindowlessControl9 *m_pWindowless;
    public:
        VMR9Renderer(void);
        virtual ~VMR9Renderer(void);

        BOOL    HasVideo() const { return (m_pWindowless != NULL); }

        HRESULT AddToGraph(IGraphBuilder *pGraph, HWND hwnd);
        HRESULT FinalizeGraph(IGraphBuilder *pGraph);

        HRESULT UpdateVideoWindow(HWND hwnd, const LPRECT prc);
        HRESULT Repaint(HWND hwnd, HDC hdc);
        HRESULT DisplayModeChanged();
    };
}
