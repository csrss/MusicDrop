#include "pch.h"
#include "HttpPostRequestDispatcher.h"
#include "SongTransferResult.h"
#include "HttpGlobals.h"
#include "HttpFileTransfer.h"

namespace Http
{
	CHttpPostRequestDispatcher::CHttpPostRequestDispatcher(IHttpFileDropEvent^ pHttpFileDropEvent)
		:	_pHttpFileDropEvent(pHttpFileDropEvent)
	{
		HRESULT hr = XLib::XFolder::CreateObject(&_pTempFolder, CHttpGlobals::Instance->SrvTempUploadsPath->Data());
		if( FAILED(hr) )
		{
			throw ref new Platform::Exception(hr);
		}

		hr = _pTempFolder->Clear();
		if( FAILED(hr) )
		{
			throw ref new Platform::Exception(hr);
		}
	}
	CHttpPostRequestDispatcher::~CHttpPostRequestDispatcher(void)
	{
		if( _pTempFolder )
		{
			_pTempFolder->Clear();
		}
	}

	HRESULT CHttpPostRequestDispatcher::Dispatch(XLib::XObjectPtr<XHttpRequest> pRequest, XLib::XObjectPtr<XHttpSocket> pSocket)
	{
		// Mandatory field, - must be present.
		XHttpHeaderFieldRequestPtr pField = pRequest->GetHttpHeader()->GetHeaderFields()->FindField<XHttpHeaderFieldRequest>();
		if( !pField )
		{
			return HRESULT_FROM_WIN32(ERROR_INVALID_FIELD);
		}

		// This is not a get processor.
		if( pField->Type == XHttpRequestType::XHttpRequestTypeGet )
		{
			return HRESULT_FROM_WIN32(ERROR_NOT_SUPPORTED);
		}

		// Field must be valid, so we can determine the action we have to take.
		if( !pField->Query.IsValid )
		{
			return HRESULT_FROM_WIN32(ERROR_INVALID_FIELD);
		}

		// Response data;
		std::string response;

		// Result code
		HRESULT hr = S_OK;

		// Post request is always about a file. We are actually doing something with incoming file: delete, play, save.
		CHttpFileTransfer fileTransfer;
		if( fileTransfer.Parse(*pRequest->HttpHeader) )
		{
			// At this moment, we know that we have a valid file here. Lets see what the hell we have to do with it.
			if( pField->Query.File.compare("play") == 0 )
			{
				// We have to play this particular file.
				PlaySongFromAudioLibrary(fileTransfer.Name.c_str(), fileTransfer.Artist.c_str(), fileTransfer.Album.c_str());
			}
			else
			{
				// Now, we have to determine, if we have a chunked file transfer or we have a full file at one shot.
				if( fileTransfer.Chunked )
				{
					// If we have chunked file transfer, we have to validate if we have received correct data first.
					if( fileTransfer.ChunkSize != pRequest->HttpBuffer->Size )
					{
						if( fileTransfer.ChunkSize > pRequest->HttpBuffer->Size )
						{
							// We do not finalize here because we still reading from this socket.
							return E_PENDING;
						}
						else if( fileTransfer.ChunkSize < pRequest->HttpBuffer->Size )
						{
							// This means incoming data has been corrupted. I do not really know for sure what to do in such case now.
							return E_FAIL;
						}
					}

					// Try to find this file in our temporary folder.
					XLib::XObjectPtr<XLib::XFile> pFile = nullptr;
					if( FAILED(hr = _pTempFolder->FindFileByName(fileTransfer.Name, &pFile)) )
					{
						// Could not load temporary folder content.
						return hr;
					}

					// Assuming we have found a file, we need to verify, that this file is not a corrupted one.
					if( !pFile )
					{
						if( FAILED(hr = _pTempFolder->CreateFileByName(fileTransfer.Name, &pFile)) )
						{
							return hr;
						}
					}
					else if( pFile )
					{
						// Physical file is present.
						if( 0 == fileTransfer.FilePosition )
						{
							// This is beginning of file transfer but this file is already present. Just open it for overwriting.
							if( FAILED(hr = pFile->Create(XLib::XFile::Disposition::OpenFlag | XLib::XFile::Disposition::TruncateFlag)) )
							{
								return hr;
							}
						}
					}

					if( FAILED(hr = pFile->Write(reinterpret_cast<const BYTE *>( pRequest->HttpBuffer->Data ), pRequest->HttpBuffer->Size)) )
					{
						return hr;
					}

					// Now we can check if we are done or not.
					if( pFile->Size == fileTransfer.Size )
					{
						// We got whole file already. Copy file to audio library. 
						CSongTransferResult^ pResult = CopyFileToAudioLibrary(&pFile->Name[0]);
						if( nullptr != pResult )
						{
							response = GetMetaInfoFromSongTransferResult(pResult);
						}
					}
				}
			}

			// Notify browser that we have received everything and we are closing connection.
			CHAR header[] = "HTTP/1.0 200 OK\r\nConnection: close\r\n\r\n";
			if( FAILED(hr = pSocket->Send(header, sizeof(header))) )			
			{
				return hr;
			}

			// If we do have some response data, we just send it before socket closure.
			if( !response.empty() )
			{
				hr = pSocket->Send(response.c_str(), response.length());
			}

			return hr;
		}
		return S_OK;
	}

	CSongTransferResult^ CHttpPostRequestDispatcher::CopyFileToAudioLibrary(const WCHAR *pwszPathName)
	{
		if( _pHttpFileDropEvent )
		{
		//	std::wstring filePath = tempFolder.GetFilePathByName(pwszName, true);
		//	if( !filePath.empty() )
		//	{
			return _pHttpFileDropEvent->OnFileDrop(CHttpGlobals::Instance->GetRelativeUploadedFilePath(ref new Platform::String(pwszPathName)));
		//	}
		}
		return nullptr;
	}

	//void CHttpPostRequestDispatcher::DeleteFileFromAudioLibrary(
	//	const WCHAR *pwszName, const WCHAR *pwszArtist, const WCHAR *pwszAlbum
	//	)
	//{
	//	if( _pHttpFileDropEvent )
	//	{
	//		_pHttpFileDropEvent->OnFileDelete(
	//			ref new Platform::String(pwszName), ref new Platform::String(pwszArtist), ref new Platform::String(pwszAlbum)
	//			);
	//	}
	//}

	void CHttpPostRequestDispatcher::PlaySongFromAudioLibrary(
		const WCHAR *pwszName, const WCHAR *pwszArtist, const WCHAR *pwszAlbum
		)
	{
		if( _pHttpFileDropEvent )
		{
			_pHttpFileDropEvent->OnFilePlay(
				ref new Platform::String(pwszName), ref new Platform::String(pwszArtist), ref new Platform::String(pwszAlbum)
				);
		}
	}

	std::string CHttpPostRequestDispatcher::GetMetaInfoFromSongTransferResult(CSongTransferResult^ pSongTransferResult)
	{
		std::string name = XLib::XStr::W2A_UTF8(pSongTransferResult->Name->Data());
		std::string artist = XLib::XStr::W2A_UTF8(pSongTransferResult->Artist->Data());
		std::string album = XLib::XStr::W2A_UTF8(pSongTransferResult->Album->Data());

		return XLib::XStr::Format("%s:%s:%s", name.c_str(), artist.c_str(), album.c_str());
	}
};
