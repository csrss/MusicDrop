#pragma once

namespace Http
{
	public ref class CSongTransferResult sealed
	{
	public:
		CSongTransferResult(Platform::String^ name, Platform::String^ artist, Platform::String^ album);
		virtual ~CSongTransferResult(void);

		property Platform::String^ Name
		{
			Platform::String^ get(void);
		}

		property Platform::String^ Artist
		{
			Platform::String^ get(void);
		}

		property Platform::String^ Album
		{
			Platform::String^ get(void);
		}

	private:
		Platform::String^ _name;
		Platform::String^ _artist;
		Platform::String^ _album;
	};
};
