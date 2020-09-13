#pragma once

struct IGraphBuilder;
namespace vEngine{
    class BaseVideoRenderer
    {
    public:
        virtual ~BaseVideoRenderer(){};

        virtual BOOL    HasVideo() const = 0;

        virtual HRESULT AddToGraph(IGraphBuilder *pGraph, HWND hwnd) = 0;
        virtual HRESULT FinalizeGraph(IGraphBuilder *pGraph) = 0;

        virtual HRESULT UpdateVideoWindow(HWND hwnd, const LPRECT prc) = 0;
        virtual HRESULT Repaint(HWND hwnd, HDC hdc) = 0;
        virtual HRESULT DisplayModeChanged() = 0;
    };
}
