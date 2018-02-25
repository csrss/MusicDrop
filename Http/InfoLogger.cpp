#include "pch.h"
#include "InfoLogger.h"
//
//namespace Http
//{
//	CInfoLogger::CInfoLogger(void)
//	{
//		_fl = Windows::Storage::ApplicationData::Current->LocalFolder->Path->Data();
//		_fl += L"\\Log.txt";
//
//		_hFile = ::CreateFile2(_fl.c_str(), GENERIC_WRITE, 0, CREATE_ALWAYS, nullptr);
//	}
//	CInfoLogger::~CInfoLogger(void)
//	{
//		if( INVALID_HANDLE_VALUE != _hFile )
//		{
//			::CloseHandle(_hFile);
//		}
//	}
//
//	CInfoLogger * CInfoLogger::Instance(void) 
//	{
//		if( nullptr == _instance )
//		{
//			_instance = new CInfoLogger();
//		}
//		return _instance;
//	}
//
//	void CInfoLogger::LogFormat(WCHAR * lpwszFormat, ...)
//	{
//		CCriticalSectionLock lock(&_cs);
//
//		WCHAR * p = nullptr;
//
//		va_list ap;
//		va_start(ap, lpwszFormat);
//		int len = ::_vscwprintf(lpwszFormat, ap);
//		if( len > 0 )
//		{
//			if( p = new (std::nothrow) WCHAR[len + 1] )
//			{
//				::_vsnwprintf_s(p, len + 1, _TRUNCATE, lpwszFormat, ap);
//			}
//		}
//		va_end(ap);
//
//		return Log(p);
//	}
//
//	void CInfoLogger::Log(const std::wstring& strMessage)
//	{
//		CCriticalSectionLock lock(&_cs);
//		if( INVALID_HANDLE_VALUE != _hFile )
//		{
//			std::string strMessageA(strMessage.begin(), strMessage.end());
//
//			// Then go with an actual data write
//			DWORD dwNumberOfBytesToWrite = strMessageA.length();
//			do
//			{
//				DWORD dwNumberOfBytesWritten = 0;
//				if( TRUE == ::WriteFile(
//					_hFile, strMessageA.c_str() + ( strMessageA.length() - dwNumberOfBytesToWrite ), dwNumberOfBytesToWrite, &dwNumberOfBytesWritten, nullptr
//					) )
//				{
//					dwNumberOfBytesToWrite -= dwNumberOfBytesWritten;
//				}
//			}
//			while( dwNumberOfBytesToWrite > 0 && ERROR_SUCCESS == ::GetLastError() );
//		}
//	}
//
//	CInfoLogger* CInfoLogger::_instance = nullptr;
//};
