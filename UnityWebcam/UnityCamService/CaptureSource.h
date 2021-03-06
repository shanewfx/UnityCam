#pragma once

#include "Globals.h"
#include "ICamSource.h"

#include <chrono>
#include <functional>

class CaptureSharedMemory;

extern const AMOVIESETUP_FILTER sudCaptureSource;
extern const AMOVIESETUP_PIN sudCaptureSourceOut;

class CaptureSource :
	public CSource, public IQualityControl, public ICamSource
{
public:

    static CUnknown * WINAPI CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

private:

	CaptureSource(LPUNKNOWN lpunk, HRESULT* phr);

public:

	virtual ~CaptureSource(void);

	DECLARE_IUNKNOWN;

    virtual STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	virtual STDMETHODIMP Notify(IBaseFilter *pSelf, Quality q);

	virtual STDMETHODIMP SetSink(IQualityControl *piqc);

private:
	HRESULT Init(CaptureSharedMemory* pReceiver, int fps);

	class CaptureStream : public CSourceStream, public IKsPropertySet, public IAMStreamConfig
	{
	public:

		CaptureStream(CaptureSource* pOwner, CaptureSharedMemory* pReceiver, int fps, HRESULT* phr);

		virtual ~CaptureStream();

		DECLARE_IUNKNOWN;

		virtual STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

		virtual STDMETHODIMP QuerySupported(REFGUID rguidPropSet, ULONG ulId, PULONG pulTypeSupport);

		virtual STDMETHODIMP Get(REFGUID rguidPropSet, ULONG ulId, LPVOID pInstanceData, ULONG ulInstanceLength, LPVOID pPropertyData, ULONG ulDataLength, PULONG pulBytesReturned);

		virtual STDMETHODIMP Set(REFGUID rguidPropSet, ULONG ulId, LPVOID pInstanceData, ULONG ulInstanceLength, LPVOID pPropertyData, ULONG ulDataLength);

		virtual HRESULT FillBuffer(IMediaSample *pSamp);

		virtual HRESULT DecideBufferSize(IMemAllocator * pAlloc, ALLOCATOR_PROPERTIES * pRequest);

		virtual STDMETHODIMP Notify(IBaseFilter *pSelf, Quality q);

		virtual STDMETHODIMP SetSink(IQualityControl *piqc);

        virtual STDMETHODIMP SetFormat(AM_MEDIA_TYPE *pmt);
        
        virtual STDMETHODIMP GetFormat(AM_MEDIA_TYPE **ppmt);
        
        virtual STDMETHODIMP GetNumberOfCapabilities(int *piCount, int *piSize);
        
        virtual STDMETHODIMP GetStreamCaps(int iIndex, AM_MEDIA_TYPE **ppmt, BYTE *pSCC);

	protected:

	    virtual HRESULT GetMediaType(/*int iPos,*/CMediaType *pMediaType);

		virtual HRESULT OnThreadStartPlay(void);

	private:
		long long oldStartTime_;

		std::chrono::system_clock::time_point lastTime;
		std::function<bool()> fpsChecker;
		HRESULT GetScaling(int width, int height, double* pScaling, double* pOffsetX, double* pOffsetY);

		LONGLONG m_llTime;

		LONGLONG m_llFrame;

		REFERENCE_TIME m_avgTimePerFrame;

		int m_iPrefWidth;

		int m_iPrefHeight;
		CMediaType m_mt;

		LONGLONG m_llLastSampleTime;

		CaptureSharedMemory* m_pReceiver;

	};

};
