#pragma once
#include "HttpRequestDispatcher.h"
#include "IHttpFileDropEvent.h"

namespace Http
{
	/*
		This guy need temporary folder and no one else.
	*/
	class CHttpPostRequestDispatcher : public IHttpDispatcher
	{
	public:
		CHttpPostRequestDispatcher(IHttpFileDropEvent^ pHttpFileDropEvent);
		virtual ~CHttpPostRequestDispatcher(void);

		virtual HRESULT Dispatch(XLib::XObjectPtr<XHttpRequest> pRequest, XLib::XObjectPtr<XHttpSocket> pSocket) override;

	protected:
		CSongTransferResult^ CopyFileToAudioLibrary(const WCHAR *pwszPathName);
		void PlaySongFromAudioLibrary(const WCHAR *pwszName, const WCHAR *pwszArtist, const WCHAR *pwszAlbum);

		std::string GetMetaInfoFromSongTransferResult(CSongTransferResult^ pSongTransferResult);

	protected:
		IHttpFileDropEvent^				_pHttpFileDropEvent;
		XLib::XObjectPtr<XLib::XFolder>	_pTempFolder;
	};
};
