#include "pch.h"
#include "SongTransferResult.h"

namespace Http
{
	CSongTransferResult::CSongTransferResult(Platform::String^ name, Platform::String^ artist, Platform::String^ album)
		:	_name(name), _artist(artist), _album(album)
	{
	}
	CSongTransferResult::~CSongTransferResult(void)
	{
	}

	Platform::String^ CSongTransferResult::Name::get(void)
	{
		return _name;
	}
	Platform::String^ CSongTransferResult::Artist::get(void)
	{
		return _artist;
	}
	Platform::String^ CSongTransferResult::Album::get(void)
	{
		return _album;
	}
};
