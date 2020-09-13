#include "..\stdafx.h"
#include <DShow.h>
#include "DShowUtil.h"
#include "VMR9Renderer.h"
#include "DShowPlayer.h"
#include "..\system\window_wrap.h"
namespace vEngine
{
    DShowPlayer::DShowPlayer(HWND hwndVideo) :
    m_state(STATE_CLOSED),
    m_hwndVideo(hwndVideo),
    m_hwndEvent(NULL),
    m_EventMsg(0),
    m_pGraph(NULL),
    m_pControl(NULL),
    m_pEvent(NULL),
    m_pSeek(NULL),
    m_pAudio(NULL),
    m_pVideo(NULL),
    m_seekCaps(0),
    m_bMute(FALSE),
    m_lVolume(MAX_VOLUME)
    {
    }

    DShowPlayer::~DShowPlayer()
    {
        TearDownGraph();
    }

    HRESULT DShowPlayer::SetEventWindow(HWND hwnd, UINT msg)
    {
        m_hwndEvent = hwnd;
        m_EventMsg = msg;
        return S_OK;
    }

    HRESULT DShowPlayer::OpenFile(const WCHAR* sFileName)
    {
        HRESULT hr = S_OK;

        IBaseFilter *pSource = NULL;

        hr = InitializeGraph();

        if (SUCCEEDED(hr))
        {
            hr = m_pGraph->AddSourceFilter(sFileName, NULL, &pSource);
        }

        if (SUCCEEDED(hr))
        {
            hr = RenderStreams(pSource);
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pSeek->GetCapabilities(&m_seekCaps);
        }

        if (SUCCEEDED(hr))
        {
            hr = UpdateVolume();
        }

        if (SUCCEEDED(hr))
        {
            m_state = STATE_STOPPED;
        }

        SAFE_RELEASE(pSource);

        return hr;
    }

    HRESULT DShowPlayer::HandleGraphEvent(WindowWrap *pCB)
    {
        if (pCB == NULL)
        {
            return E_POINTER;
        }

        if (!m_pEvent)
        {
            return E_UNEXPECTED;
        }

        long evCode = 0;
        LONG_PTR param1 = 0, param2 = 0;

        HRESULT hr = S_OK;

        while (SUCCEEDED(m_pEvent->GetEvent(&evCode, &param1, &param2, 0)))
        {
            pCB->HandleDShowEvent(evCode, param1, param2);

            hr = m_pEvent->FreeEventParams(evCode, param1, param2);
            if (FAILED(hr))
            {
                break;
            }
        }

        return hr;
    }

    HRESULT DShowPlayer::Play()
    {
        if (m_state != STATE_PAUSED && m_state != STATE_STOPPED)
        {
            return VFW_E_WRONG_STATE;
        }

        assert(m_pGraph);

        HRESULT hr = m_pControl->Run();

        if (SUCCEEDED(hr))
        {
            m_state = STATE_RUNNING;
        }

        return hr;
    }

    HRESULT DShowPlayer::Pause()
    {
        if (m_state != STATE_RUNNING)
        {
            return VFW_E_WRONG_STATE;
        }

        assert(m_pGraph); 

        HRESULT hr = m_pControl->Pause();

        if (SUCCEEDED(hr))
        {
            m_state = STATE_PAUSED;
        }

        return hr;
    }


    HRESULT DShowPlayer::Stop()
    {
        if (m_state != STATE_RUNNING && m_state != STATE_PAUSED)
        {
            return VFW_E_WRONG_STATE;
        }

        assert(m_pGraph); 

        HRESULT hr = m_pControl->Stop();

        if (SUCCEEDED(hr))
        {
            m_state = STATE_STOPPED;
        }

        return hr;
    }

    BOOL DShowPlayer::HasVideo() const
    {
        return (m_pVideo && m_pVideo->HasVideo());
    }

    HRESULT DShowPlayer::UpdateVideoWindow(const LPRECT prc)
    {
        HRESULT hr = S_OK;

        if (m_pVideo)
        {
            hr = m_pVideo->UpdateVideoWindow(m_hwndVideo, prc);
        }

        return hr;
    }

    HRESULT DShowPlayer::Repaint(HDC hdc)
    {
        HRESULT hr = S_OK;

        if (m_pVideo)
        {
            hr = m_pVideo->Repaint(m_hwndVideo, hdc);
        }

        return hr;
    }

    HRESULT DShowPlayer::DisplayModeChanged()
    {
        HRESULT hr = S_OK;

        if (m_pVideo)
        {
            hr = m_pVideo->DisplayModeChanged();
        }

        return hr;
    }

    BOOL DShowPlayer::CanSeek() const
    {
        const DWORD caps = AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanGetDuration;
        return ((m_seekCaps & caps) == caps);
    }

    HRESULT DShowPlayer::SetPosition(REFERENCE_TIME pos)
    {
        if (m_pControl == NULL || m_pSeek == NULL)
        {
            return E_UNEXPECTED;
        }

        HRESULT hr = S_OK;

        hr = m_pSeek->SetPositions(&pos, AM_SEEKING_AbsolutePositioning,
            NULL, AM_SEEKING_NoPositioning);

        if (SUCCEEDED(hr))
        {
            if (m_state == STATE_STOPPED)
            {
                hr = m_pControl->StopWhenReady();
            }
        }

        return hr;
    }

    HRESULT DShowPlayer::GetDuration(LONGLONG *pDuration)
    {
        if (m_pSeek == NULL)
        {
            return E_UNEXPECTED;
        }

        return m_pSeek->GetDuration(pDuration);
    }

    HRESULT DShowPlayer::GetCurrentPosition(LONGLONG *pTimeNow)
    {
        if (m_pSeek == NULL)
        {
            return E_UNEXPECTED;
        }

        return m_pSeek->GetCurrentPosition(pTimeNow);
    }

    HRESULT	DShowPlayer::Mute(BOOL bMute)
    {
        m_bMute = bMute;
        return UpdateVolume();
    }

    HRESULT	DShowPlayer::SetVolume(long lVolume)
    {
        m_lVolume = lVolume;
        return UpdateVolume();
    }

    HRESULT DShowPlayer::UpdateVolume()
    {
        HRESULT hr = S_OK;

        if (m_bAudioStream && m_pAudio)
        {
            if (m_bMute)
            {
                hr = m_pAudio->put_Volume(MIN_VOLUME);
            }
            else
            {
                hr = m_pAudio->put_Volume(m_lVolume);
            }
        }

        return hr;
    }

    HRESULT DShowPlayer::InitializeGraph()
    {
        HRESULT hr = S_OK;

        TearDownGraph();

        hr = CoCreateInstance(
            CLSID_FilterGraph, 
            NULL, 
            CLSCTX_INPROC_SERVER,
            IID_IGraphBuilder,
            (void**)&m_pGraph
            );

        if (SUCCEEDED(hr))
        {
            hr = m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pControl);
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pGraph->QueryInterface(IID_IMediaEventEx, (void**)&m_pEvent);
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pGraph->QueryInterface(IID_IMediaSeeking, (void**)&m_pSeek);
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pGraph->QueryInterface(IID_IBasicAudio, (void**)&m_pAudio);
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pEvent->SetNotifyWindow((OAHWND)m_hwndEvent, m_EventMsg, NULL);
        }

        return hr;
    }

    void DShowPlayer::TearDownGraph()
    {
        if (m_pEvent)
        {
            m_pEvent->SetNotifyWindow((OAHWND)NULL, NULL, NULL);
        }

        SAFE_RELEASE(m_pGraph);
        SAFE_RELEASE(m_pControl);
        SAFE_RELEASE(m_pEvent);
        SAFE_RELEASE(m_pSeek);
        SAFE_RELEASE(m_pAudio);

        delete m_pVideo;
        m_pVideo = NULL;

        m_state = STATE_CLOSED;
        m_seekCaps = 0;

        m_bAudioStream = FALSE;
    }


    HRESULT DShowPlayer::CreateVideoRenderer()
    {
        HRESULT hr = E_FAIL;

        m_pVideo = new VMR9Renderer();

        if (m_pVideo == NULL)
        {
            hr = E_OUTOFMEMORY;
        }

        hr = m_pVideo->AddToGraph(m_pGraph, m_hwndVideo);
        if (FAILED(hr))
        {
            delete m_pVideo;
            m_pVideo = NULL;
        }
        return hr;
    }

    HRESULT	DShowPlayer::RenderStreams(IBaseFilter *pSource)
    {
        HRESULT hr = S_OK;

        BOOL bRenderedAnyPin = FALSE;

        IFilterGraph2 *pGraph2 = NULL;
        IEnumPins *pEnum = NULL;
        IBaseFilter *pAudioRenderer = NULL;

        hr = m_pGraph->QueryInterface(IID_IFilterGraph2, (void**)&pGraph2);

        if (SUCCEEDED(hr))
        {
            hr = CreateVideoRenderer();
        }

        if (SUCCEEDED(hr))
        {
            hr = AddFilterByCLSID(m_pGraph, CLSID_DSoundRender, &pAudioRenderer, L"Audio Renderer");
        }

        if (SUCCEEDED(hr))
        {
            hr = pSource->EnumPins(&pEnum);
        }

        if (SUCCEEDED(hr))
        {
            IPin *pPin = NULL;

            while (S_OK == pEnum->Next(1, &pPin, NULL))
            {
                HRESULT hr2 = pGraph2->RenderEx(pPin, AM_RENDEREX_RENDERTOEXISTINGRENDERERS, NULL);

                pPin->Release();

                if (SUCCEEDED(hr2))
                {
                    bRenderedAnyPin = TRUE;
                }
            }
        }

        if (SUCCEEDED(hr))
        {
            hr = m_pVideo->FinalizeGraph(m_pGraph);
        }

        if (SUCCEEDED(hr))
        {
            BOOL bRemoved = FALSE;
            hr = RemoveUnconnectedRenderer(m_pGraph, pAudioRenderer, &bRemoved);

            if (bRemoved)
            {
                m_bAudioStream = FALSE;
            }
            else
            {
                m_bAudioStream = TRUE;
            }
        }

        SAFE_RELEASE(pEnum);
        SAFE_RELEASE(pAudioRenderer);
        SAFE_RELEASE(pGraph2);

        if (SUCCEEDED(hr))
        {
            if (!bRenderedAnyPin)
            {
                hr = VFW_E_CANNOT_RENDER;
            }
        }

        return hr;
    }
}
