#include "pch.h"
#include "HttpGetRequestDispatcher.h"
#include "HttpGlobals.h"

namespace Http
{
	CHttpGetRequestDispatcher::CHttpGetRequestDispatcher(IHttpSongsExposeEvent^ pHttpSongsExposeEvent) 
		:	_pHttpSongsExposeEvent(pHttpSongsExposeEvent)
	{
	}
	CHttpGetRequestDispatcher::~CHttpGetRequestDispatcher(void)
	{
	}

	HRESULT CHttpGetRequestDispatcher::Dispatch(XLib::XObjectPtr<XHttpRequest> pRequest, XLib::XObjectPtr<XHttpSocket> pSocket)
	{
		XHttpHeaderFieldRequestPtr pField = pRequest->GetHttpHeader()->GetHeaderFields()->FindField<XHttpHeaderFieldRequest>();
		if( !pField )
		{
			return HRESULT_FROM_WIN32(ERROR_INVALID_DATA);
		}

		if( pField->Type == XHttpRequestType::XHttpRequestTypePost )
		{
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		}

		HRESULT hr = S_OK;
		if( pField->Query.ContentType & XHttpContentType::IsQuery )
		{
			// query
			if( pField->Query.File == "/" )
			{
				// root server directory request
				CHAR header[] = "HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Type: text/html\r\n\r\n";
				hr = pSocket->Send(header, sizeof(header));

				if( FAILED(hr) ) return hr;

				// Construct html file path
				std::wstring strPath = XLib::XStr::Format(L"%s\\index.html", CHttpGlobals::Instance->SrvRoot->Data());

				// Create html main page
				XLib::XObjectPtr<XHttpHtmlPage> pHttpHtmlPage;
				hr = XHttpHtmlPage::CreateObject(&pHttpHtmlPage, strPath.c_str());
				if( FAILED(hr) )
				{
					return hr;
				}

				// Html page data utf8 encoded
				std::string str_utf8 = pHttpHtmlPage->Buffer;

				// And at this point, we have to expose our current collection to the user.
				if( _pHttpSongsExposeEvent )
				{
					if( Windows::Foundation::Collections::IVector<CAudioItem^>^ pAudioItems = _pHttpSongsExposeEvent->GetAudioCollection() )
					{
						std::string strAudioItems = GetStringFromItemsCollection(pAudioItems);
						if( !strAudioItems.empty() )
						{
							std::string str_what = "[audio_collection]";
							size_t nPos = str_utf8.find(str_what);
							if( nPos != std::string::npos )
							{
								str_utf8.replace(nPos, str_what.length(), strAudioItems);
							}
						}
					}
				}

				return pSocket->Send(str_utf8.c_str(), ::strlen(str_utf8.c_str()));
			}
			else
			{
				// Currently we do not process any other queries except main page request
			}
		}
		else if( pField->Query.ContentType & XHttpContentType::IsFile )
		{
			// file
			// Construct file path
			std::wstring strPath = XLib::XStr::Format(L"%s\\%s", CHttpGlobals::Instance->SrvRoot->Data(), XLib::XStr::A2W(pField->Query.File).c_str());

			// Now grab this file size and check if it is valid
			UINT fileSize = _getFileSize(strPath.c_str());
			if( INVALID_FILE_SIZE == fileSize )
			{
				return HRESULT_FROM_WIN32(::GetLastError());
			}

			// Open this damn file.
			XLib::XHandle hFile = ::CreateFile2(strPath.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr);
			if( !hFile.Valid )
			{
				return HRESULT_FROM_WIN32(::GetLastError());
			}

			// Now we have to construct a valid http header to be sent to a client browser. 
			std::string header;
			switch( pField->Query.FileType )
			{
			case XHttpFileType::Css:
				header = XLib::XStr::Format("HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: %I64u\r\nContent-Type: text/css\r\n\r\n", fileSize);
				break;

			case XHttpFileType::Html:
				header = XLib::XStr::Format("HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: %I64u\r\nContent-Type: text/html\r\n\r\n", fileSize);
				break;

			case XHttpFileType::JavaScript:
				header = XLib::XStr::Format("HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: %I64u\r\nContent-Type: text/javascript\r\n\r\n", fileSize);
				break;

			case XHttpFileType::Jpg:
				header = XLib::XStr::Format("HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: %I64u\r\nContent-Type: image/jpg\r\n\r\n", fileSize);
				break;

			case XHttpFileType::Png:
				header = XLib::XStr::Format("HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: %I64u\r\nContent-Type: image/png\r\n\r\n", fileSize);
				break;

			case XHttpFileType::Txt:
				header = XLib::XStr::Format("HTTP/1.0 200 OK\r\nConnection: close\r\nContent-Length: %I64u\r\nContent-Type: text/plain\r\n\r\n", fileSize);
				break;
			}

			if( pField->Query.ContentType & XHttpContentType::IsDocument )
			{
				// Create html main page
				XLib::XObjectPtr<XHttpHtmlPage> pHttpHtmlPage;
				hr = XHttpHtmlPage::CreateObject(&pHttpHtmlPage, strPath.c_str());
				if( FAILED(hr) )
				{
					return hr;
				}

				// Html page data utf8 encoded
				std::string str_utf8 = pHttpHtmlPage->Buffer;

				// Sending page data to a client browser
				return pSocket->Send(str_utf8.c_str(), ::strlen(str_utf8.c_str()));
			}
			else if( pField->Query.ContentType & XHttpContentType::IsBinary )
			{
				DWORD dwBytesRead = 0, dwTotalRead = 0;
				BYTE buffer[4096] = { 0x00 };
				while( dwTotalRead != fileSize && ::ReadFile(hFile, buffer, sizeof(buffer), &dwBytesRead, NULL) && dwBytesRead > 0 )
				{
					hr = pSocket->Send((CHAR *)buffer, dwBytesRead);
					if( FAILED(hr) ) return hr;

					dwTotalRead += dwBytesRead;
				}
				return hr;
			}
		}
		return E_FAIL;
	}

	UINT CHttpGetRequestDispatcher::_getFileSize(const WCHAR * pwszPath)
	{
		WIN32_FILE_ATTRIBUTE_DATA fileInformation = { 0x00 };
		if( TRUE == ::GetFileAttributesExW(pwszPath, GetFileExInfoStandard, &fileInformation) )
		{
			__int64 size = ((__int64)fileInformation.nFileSizeHigh << 32) | fileInformation.nFileSizeLow;
			if( size > UINT_MAX )
			{
				::SetLastError(ERROR_FILE_TOO_LARGE);
				return INVALID_FILE_SIZE;
			}
			return static_cast<UINT>( size );
		}
		return INVALID_FILE_SIZE;
	}

	std::string CHttpGetRequestDispatcher::GetStringFromItemsCollection(Windows::Foundation::Collections::IVector<CAudioItem^>^ pAudioItems)
	{
		std::string strTemp;
		for( CAudioItem^ pItem : pAudioItems )
		{
			strTemp += "<li><table><tr>";
			strTemp += "<td width=\"10%\"><button id=\"btn\" class=\"sort btn\" style=\"width:48px;height:48px;background-image:url('appbar.transport.play.rest.png')\" onClick=\"new XMLHttpPlaySongRequest('";
			strTemp += XLib::XStr::Escape(pItem->GetName()) + "', '" + XLib::XStr::Escape(pItem->GetArtist()) + "', '" + XLib::XStr::Escape(pItem->GetAlbum());
			strTemp += "');\"></button></td><td>";
            strTemp += "<span class=\"feature\">";
			strTemp += pItem->GetName();
            strTemp += "</span><br>";
            strTemp += "<small class=\"description\">";
			strTemp += pItem->GetArtist();
            strTemp += "</small><br>";
            strTemp += "<small class=\"information\">";
			strTemp += pItem->GetAlbum();
            strTemp += "</small></td></tr>";
            strTemp += "</table></li>";
		}
		return strTemp;
	}
};

