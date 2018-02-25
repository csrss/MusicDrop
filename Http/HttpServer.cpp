#include "pch.h"
#include "HttpServer.h"

#include "HttpGetRequestDispatcher.h"
#include "HttpPostRequestDispatcher.h"

#include "HttpHeaderFields.h"

namespace Http
{
	CHttpServer::CHttpServer(Platform::String^ addr, int port/*, IHttpFileDropEvent^ pHttpFileDropEvent, IHttpSongsExposeEvent^ pHttpSongsExposeEvent*/) 
		:	_pHttpDispatchers(new (std::nothrow) XHttpDispatchers())
		,	_pHttpHeaderFields(new (std::nothrow) XHttpHeaderFields())
	{
		// Setup specific header fields, that we are going to parse later.
		_pHttpHeaderFields->CreateField<Header::CHttpHeaderFieldFileName>();
		_pHttpHeaderFields->CreateField<Header::CHttpHeaderFieldFileSize>();
		_pHttpHeaderFields->CreateField<Header::CHttpHeaderFieldFileType>();
		_pHttpHeaderFields->CreateField<Header::CHttpHeaderFieldFileChunkSize>();
		_pHttpHeaderFields->CreateField<Header::CHttpHeaderFieldFilePosition>();
		_pHttpHeaderFields->CreateField<Header::CHttpHeaderFieldFileArtist>();
		_pHttpHeaderFields->CreateField<Header::CHttpHeaderFieldFileAlbum>();

		// Create http acceptor.
		HRESULT hr = XHttpAcceptor::CreateObject(&_pHttpAcceptor, XLib::XStr::W2A(addr->Data()), port);
		if( FAILED(hr) )
		{
			throw ref new Platform::Exception(hr);
		}

//		_vHttpDispatchers.push_back(new (std::nothrow) CHttpGetRequestDispatcher(pHttpSongsExposeEvent));
	//	_vHttpDispatchers.push_back(new (std::nothrow) CHttpPostRequestDispatcher(pHttpFileDropEvent));
	}
	CHttpServer::~CHttpServer(void)
	{
	}

	CHttpServerConnection^ CHttpServer::Accept(void)
	{
		XLib::XObjectPtr<XHttpConnection> pConnection;
		HRESULT hr = _pHttpAcceptor->Accept(&pConnection);

		if( FAILED(hr) )
		{
			throw ref new Platform::Exception(hr);
		}

		//pConnection->AddDispatcher(new (std::nothrow) CHttpGetRequestDispatcher(pHttpSongsExposeEvent));
	//	pConnection->AddDispatcher(new (std::nothrow) CHttpPostRequestDispatcher(pHttpFileDropEvent));

		return ref new CHttpServerConnection(pConnection);
	}

	void CHttpServer::Process(CHttpServerConnection^ pConnection)
	{
		HRESULT hr = pConnection->Connection->Process(_pHttpHeaderFields, _pHttpDispatchers);
//		HRESULT hr = XHttpAcceptor::Process(pConnection->Socket, _vHttpDispatchers);
		if( FAILED(hr) )
		{
			throw ref new Platform::Exception(hr);
		}
	}

	void CHttpServer::Stop(void)
	{
		HRESULT hr = _pHttpAcceptor->Close();
		if( FAILED(hr) )
		{
			throw ref new Platform::Exception(hr);
		}
	}

	void CHttpServer::HttpFileDropEvent::set(IHttpFileDropEvent^ value)
	{
		_pHttpDispatchers->Add(new (std::nothrow) CHttpPostRequestDispatcher(value));
	}

	void CHttpServer::HttpSongsExposeEvent::set(IHttpSongsExposeEvent^ value)
	{
		_pHttpDispatchers->Add(new (std::nothrow) CHttpGetRequestDispatcher(value));
	}

	//int CHttpServer::IsValidIp4(const wchar_t *str)
	//{
	//	int segs = 0;   /* Segment count. */
	//	int chcnt = 0;  /* Character count within segment. */
	//	int accum = 0;  /* Accumulator for segment. */

	//	/* Catch NULL pointer. */

	//	if (str == NULL)
	//	{
	//		return 0;
	//	}

	//	/* Process every character in string. */

	//	while (*str != L'\0') 
	//	{
	//		/* Segment changeover. */

	//		if (*str == L'.') 
	//		{
	//			/* Must have some digits in segment. */

	//			if (chcnt == 0)
	//			{
	//				return 0;
	//			}

	//			/* Limit number of segments. */

	//			if (++segs == 4)
	//			{
	//				return 0;
	//			}

	//			/* Reset segment values and restart loop. */

	//			chcnt = accum = 0;
	//			str++;
	//			continue;
	//		}

	//		/* Check numeric. */

	//		if ((*str < L'0') || (*str > L'9'))
	//		{
	//			return 0;
	//		}

	//		/* Accumulate and check segment. */

	//		if ((accum = accum * 10 + *str - L'0') > 255)
	//		{
	//			return 0;
	//		}

	//		/* Advance other segment specific stuff and continue loop. */

	//		chcnt++;
	//		str++;
	//	}

	//	/* Check enough segments and enough characters in last segment. */

	//	if (segs != 3)
	//	{
	//		return 0;
	//	}

	//	if (chcnt == 0)
	//	{
	//		return 0;
	//	}

	//	/* Address okay. */

	//	return 1;
	//}
};
