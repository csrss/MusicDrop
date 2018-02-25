#pragma once
#include "pch.h"

namespace Http
{
	/*
        Class to be created in C# component. Only c-tor is needed to be visible.
    */
    public ref class CAudioItem sealed
    {
    public:
        CAudioItem(Platform::String^ name, Platform::String^ artist, Platform::String^ album)
            :    _name(XLib::XStr::W2A_UTF8(name->Data())), _artist(XLib::XStr::W2A_UTF8(artist->Data())), _album(XLib::XStr::W2A_UTF8(album->Data()))
        {
        }
        virtual ~CAudioItem(void)
        {
        }

	internal:
		const std::string& GetName(void) const
		{
			return _name;
		}

		const std::string& GetArtist(void) const
		{
			return _artist;
		}

		const std::string& GetAlbum(void) const
		{
			return _album;
		}

    private:
		std::string _name;
        std::string _artist;
        std::string _album;
    };
};
