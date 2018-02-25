#pragma once

namespace Http
{
	public ref class CHttpGlobals sealed
	{
	private:
		CHttpGlobals(void);
		~CHttpGlobals(void);

	public:
		static property CHttpGlobals^ Instance
		{
			CHttpGlobals^ get(void);
		}

		// Returns server root, where html files are stored.
		property Platform::String^ SrvRoot
		{
			Platform::String^ get(void);
		}

		// Returns absolute path for temporary folder, which stores uploaded files.
		property Platform::String^ SrvTempUploadsPath
		{
			Platform::String^ get(void);
		}

		// Returns relative file path, like "temp\\file_name"
		Platform::String^ GetRelativeUploadedFilePath(Platform::String^ pFileName);

	private:
		static CHttpGlobals^	_pInstance;
		Platform::String^		_pSrvRoot;
		Platform::String^		_pSrvTemp;
	};
};
