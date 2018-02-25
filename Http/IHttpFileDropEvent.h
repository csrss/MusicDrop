#pragma once
#include "AudioItem.h"
#include "SongTransferResult.h"

namespace Http
{
	public interface class IHttpFileDropEvent
	{
	public:
		CSongTransferResult^ OnFileDrop(Platform::String^ strFilePathName);
	//	void OnFileDelete(Platform::String^ name, Platform::String^ artist, Platform::String^ album);
		void OnFilePlay(Platform::String^ name, Platform::String^ artist, Platform::String^ album);
	};

	public interface class IHttpSongsExposeEvent
	{
	public:
		Windows::Foundation::Collections::IVector<CAudioItem^>^ GetAudioCollection(void);
	};
};
