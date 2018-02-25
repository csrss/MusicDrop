#include "pch.h"
#include "HttpFileTransfer.h"
#include "HttpHeaderFields.h"

namespace Http
{
	CHttpFileTransfer::CHttpFileTransfer(void)
		:	_fileName(L""), _fileSize(0), _chunkSize(0), _filePos(0)
	{
	}
	CHttpFileTransfer::~CHttpFileTransfer(void)
	{
	}

	bool CHttpFileTransfer::Parse(const XHttpHeader& header)
	{
		if( Header::CHttpHeaderFieldFileName * pFieldName = header.GetHeaderFields()->FindField<Header::CHttpHeaderFieldFileName>() )
		{
			// W tym miejscu robimy konwersje z utf8, bo w takim kodowaniu wysylamy wszystko do jasnej dupy.
			_fileName = XLib::XStr::A2W_UTF8(pFieldName->Value);

			// If a filename is fucking empty, we cannot proceed.
			if( _fileName.empty() )
			{
				return false;
			}

			// So far we got name at least. 
			if( Header::CHttpHeaderFieldFileSize * pFieldSize = header.GetHeaderFields()->FindField<Header::CHttpHeaderFieldFileSize>() )
			{
				// We got a file size, meaning this is an upload.
				_fileSize = pFieldSize->Value;

				// Kewl, got size! Now check if this is chunked file transfer..
				if( Header::CHttpHeaderFieldFileChunkSize * pFieldChunkSize = header.GetHeaderFields()->FindField<Header::CHttpHeaderFieldFileChunkSize>() )
				{
					_chunkSize = pFieldChunkSize->Value;
				}

				// We need file position also
				if( Header::CHttpHeaderFieldFilePosition * pFilePos = header.GetHeaderFields()->FindField<Header::CHttpHeaderFieldFilePosition>() )
				{
					_filePos = pFilePos->Value;
				}

				// return success at this point.
				return true;
			}
			else if( Header::CHttpHeaderFieldFileArtist * pFieldArtist = header.GetHeaderFields()->FindField<Header::CHttpHeaderFieldFileArtist>() )
			{
				// We have an artist field, means this file is going to be either played or deleted.
				_fileArtist = XLib::XStr::A2W_UTF8(pFieldArtist->Value);

				// We can have the below value or not - it is not critical to have it.
				if( Header::CHttpHeaderFieldFileAlbum * pFieldAlbum = header.GetHeaderFields()->FindField<Header::CHttpHeaderFieldFileAlbum>() )
				{
					_fileAlbum = XLib::XStr::A2W_UTF8(pFieldAlbum->Value);
				}

				// return success at this point.
				return true;
			}
		}
		return false;
	}

	const std::wstring& CHttpFileTransfer::GetName(void) const
	{
		return _fileName;
	}

	const std::wstring& CHttpFileTransfer::GetArtist(void) const
	{
		return _fileArtist;
	}

	const std::wstring& CHttpFileTransfer::GetAlbum(void) const
	{
		return _fileAlbum;
	}

	const UINT& CHttpFileTransfer::GetSize(void) const
	{
		return _fileSize;
	}

	const UINT& CHttpFileTransfer::GetChunkSize(void) const
	{
		return _chunkSize;
	}

	const UINT& CHttpFileTransfer::GetFilePosition(void) const
	{
		return _filePos;
	}

	bool CHttpFileTransfer::GetChunked(void) const
	{
		return ( _chunkSize != 0 );
	}

	const bool CHttpFileTransfer::IsQueuedForPlay(void) const
	{
		return ( _fileSize == 1 && _chunkSize == 1 );
	}
};
