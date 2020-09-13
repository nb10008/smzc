#include "..\stdafx.h"
#include <dshow.h>
#include "DShowUtil.h"
namespace vEngine
{
    HRESULT IsPinConnected(IPin *pPin, BOOL *pResult)
    {
        if (pPin == NULL || pResult == NULL)
        {
            return E_POINTER;
        }

        IPin *pTmp = NULL;
        HRESULT hr = pPin->ConnectedTo(&pTmp);
        if (SUCCEEDED(hr))
        {
            *pResult = TRUE;
        }
        else if (hr == VFW_E_NOT_CONNECTED)
        {
            // The pin is not connected. This is not an error for our purposes.
            *pResult = FALSE;
            hr = S_OK;
        }

        SAFE_RELEASE(pTmp);
        return hr;
    }

    HRESULT IsPinDirection(IPin *pPin, PIN_DIRECTION dir, BOOL *pResult)
    {
        if (pPin == NULL || pResult == NULL)
        {
            return E_POINTER;
        }

        PIN_DIRECTION pinDir;
        HRESULT hr = pPin->QueryDirection(&pinDir);
        if (SUCCEEDED(hr))
        {
            *pResult = (pinDir == dir);
        }
        return hr;
    }

    MatchPinDirectionAndConnection::MatchPinDirectionAndConnection(PIN_DIRECTION direction, BOOL bShouldBeConnected) 
            : m_bShouldBeConnected(bShouldBeConnected), m_direction(direction)
    {
    }

    HRESULT MatchPinDirectionAndConnection::operator()(IPin *pPin, BOOL *pResult)
    {
        assert(pResult != NULL);

        BOOL bMatch = FALSE;
        BOOL bIsConnected = FALSE;

        HRESULT hr = IsPinConnected(pPin, &bIsConnected);
        if (SUCCEEDED(hr))
        {
            if (bIsConnected == m_bShouldBeConnected)
            {
                hr = IsPinDirection(pPin, m_direction, &bMatch);
            }
        }

        if (SUCCEEDED(hr))
        {
            *pResult = bMatch;
        }
        return hr;
    }
    template <class PinPred>
    HRESULT FindMatchingPin(IBaseFilter *pFilter, PinPred FN, IPin **ppPin)
    {
        IEnumPins *pEnum = NULL;
        IPin *pPin = NULL;

        HRESULT hr = pFilter->EnumPins(&pEnum);
        if (FAILED(hr))
        {
            return hr;
        }

        BOOL bFound = FALSE;
        while (S_OK == pEnum->Next(1, &pPin, NULL))
        {
            hr = FN(pPin, &bFound);

            if (FAILED(hr))
            {
                pPin->Release();
                break;
            }
            if (bFound)
            {
                *ppPin = pPin;
                break;
            }

            pPin->Release();
        }

        pEnum->Release();

        if (!bFound)
        {
            hr = VFW_E_NOT_FOUND;
        }

        return hr;
    }

    HRESULT FindConnectedPin(
                                    IBaseFilter *pFilter,   // Pointer to the filter.
                                    PIN_DIRECTION PinDir,   // Direction of the pin to find.
                                    IPin **ppPin            // Receives a pointer to the pin.
                                    )
    {
        return FindMatchingPin(pFilter, MatchPinDirectionAndConnection(PinDir, TRUE), ppPin);
    }

    HRESULT RemoveUnconnectedRenderer(IGraphBuilder *pGraph, IBaseFilter *pRenderer, BOOL *pbRemoved)
    {
        IPin *pPin = NULL;

        BOOL bRemoved = FALSE;

        // Look for a connected input pin on the renderer.

        HRESULT hr = FindConnectedPin(pRenderer, PINDIR_INPUT, &pPin);
        SAFE_RELEASE(pPin);

        // If this function succeeds, the renderer is connected, so we don't remove it.
        // If it fails, it means the renderer is not connected to anything, so
        // we remove it.

        if (FAILED(hr))
        {
            hr = pGraph->RemoveFilter(pRenderer);
            bRemoved = TRUE;
        }

        if (SUCCEEDED(hr))
        {
            *pbRemoved = bRemoved;
        }

        return hr;
    }

    HRESULT AddFilterByCLSID(
                             IGraphBuilder *pGraph,          // Pointer to the Filter Graph Manager.
                             const GUID& clsid,              // CLSID of the filter to create.
                             IBaseFilter **ppF,              // Receives a pointer to the filter.
                             LPCWSTR wszName                 // A name for the filter (can be NULL).
                             )
    {
        if (!pGraph || ! ppF) 
        {
            return E_POINTER;
        }

        *ppF = 0;

        IBaseFilter *pFilter = NULL;
        HRESULT hr = S_OK;

        hr = CoCreateInstance(
            clsid, 
            NULL, 
            CLSCTX_INPROC_SERVER, 
            IID_IBaseFilter, 
            (void**)&pFilter);

        if(FAILED(hr))
            return hr;
        hr = pGraph->AddFilter(pFilter, wszName);

        if(FAILED(hr))
        {
            SAFE_RELEASE(pFilter);
            return hr;
        }
        *ppF = pFilter;
        (*ppF)->AddRef();
        return S_OK;
    }
}
