#pragma once
namespace vEngine{
    HRESULT IsPinConnected(IPin *pPin, BOOL *pResult);
    HRESULT IsPinDirection(IPin *pPin, PIN_DIRECTION dir, BOOL *pResult);
    struct MatchPinDirectionAndConnection
    {
        BOOL            m_bShouldBeConnected;
        PIN_DIRECTION   m_direction;
        MatchPinDirectionAndConnection(PIN_DIRECTION direction, BOOL bShouldBeConnected);
        HRESULT operator()(IPin *pPin, BOOL *pResult);
    };
    HRESULT FindConnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin);
    HRESULT RemoveUnconnectedRenderer(IGraphBuilder *pGraph, IBaseFilter *pRenderer, BOOL *pbRemoved);
    HRESULT AddFilterByCLSID(IGraphBuilder *pGraph, const GUID& clsid, IBaseFilter **ppF, LPCWSTR wszName = NULL);
}
