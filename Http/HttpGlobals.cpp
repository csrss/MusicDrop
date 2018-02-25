#include "pch.h"
#include "HttpGlobals.h"

namespace Http
{
	CHttpGlobals::CHttpGlobals(void)
		:	_pSrvRoot(Platform::String::Concat(Windows::Storage::ApplicationData::Current->LocalFolder->Path, L"\\html"))
		,	_pSrvTemp(Platform::String::Concat(Windows::Storage::ApplicationData::Current->LocalFolder->Path, L"\\temp"))
	{
	}
	CHttpGlobals::~CHttpGlobals(void)
	{
	}

	CHttpGlobals^ CHttpGlobals::Instance::get(void)
	{
		if( nullptr == _pInstance )
		{
			_pInstance = ref new CHttpGlobals();
		}
		return _pInstance;
	}

	Platform::String^ CHttpGlobals::SrvRoot::get(void)
	{
		return _pSrvRoot;
	}

	Platform::String^ CHttpGlobals::SrvTempUploadsPath::get(void)
	{
		return _pSrvTemp;
	}

	Platform::String^ CHttpGlobals::GetRelativeUploadedFilePath(Platform::String^ pFileName)
	{
		return Platform::String::Concat(L"temp\\", pFileName);
	}

	CHttpGlobals^ CHttpGlobals::_pInstance = nullptr;
};
