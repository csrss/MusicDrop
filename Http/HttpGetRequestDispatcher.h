#pragma once
#include "HttpRequestDispatcher.h"
#include "IHttpFileDropEvent.h"

namespace Http
{
	class CHttpGetRequestDispatcher : public IHttpDispatcher
	{
	public:
		CHttpGetRequestDispatcher(IHttpSongsExposeEvent^ pHttpSongsExposeEvent);
		virtual ~CHttpGetRequestDispatcher(void);

		/*
			Returns data as response, or null in there is no response. Throws exception on error.
		*/
		virtual HRESULT Dispatch(XLib::XObjectPtr<XHttpRequest> pRequest, XLib::XObjectPtr<XHttpSocket> pSocket) override;

	protected:
		UINT _getFileSize(const WCHAR * pwszPath);

		std::string GetStringFromItemsCollection(Windows::Foundation::Collections::IVector<CAudioItem^>^ pAudioItems);

	protected:
		IHttpSongsExposeEvent^	_pHttpSongsExposeEvent;
	};
};
