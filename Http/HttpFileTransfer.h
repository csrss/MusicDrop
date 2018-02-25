#pragma once

namespace Http
{
	class CHttpFileTransfer
	{
	public:
		CHttpFileTransfer(void);
		virtual ~CHttpFileTransfer(void);

		bool Parse(const XHttpHeader& header);

		__declspec( property(get = GetChunked) ) bool Chunked;
		bool GetChunked(void) const;

		__declspec( property(get = GetName) ) std::wstring& Name;
		const std::wstring& GetName(void) const;

		__declspec( property(get = GetArtist) ) std::wstring& Artist;
		const std::wstring& GetArtist(void) const;

		__declspec( property(get = GetAlbum) ) std::wstring& Album;
		const std::wstring& GetAlbum(void) const;

		__declspec( property(get = GetSize) ) UINT& Size;
		const UINT& GetSize(void) const;

		__declspec( property(get = GetChunkSize) ) UINT& ChunkSize;
		const UINT& GetChunkSize(void) const;

		__declspec( property(get = GetFilePosition) ) UINT& FilePosition;
		const UINT& GetFilePosition(void) const;

		__declspec( property(get = IsQueuedForPlay) ) bool QueuedForPlay;
		const bool IsQueuedForPlay(void) const;

	protected:
		std::wstring	_fileName;
		UINT			_fileSize;
		UINT			_chunkSize;
		UINT			_filePos;

		std::wstring	_fileArtist;
		std::wstring	_fileAlbum;
	};
};