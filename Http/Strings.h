#pragma once
#include "pch.h"

//namespace Http
//{
//	class CStrings
//	{
//	public:
	//	static CHAR * AllocString(const UINT& nSize)
	//	{
	//		if( CHAR * p = new (std::nothrow) CHAR[nSize] )
	//		{
	//			return reinterpret_cast<CHAR *>( ::memset(p, 0x00, nSize) );
	//		}
	//		return nullptr;
	//	}

	//	static void FreeString(CHAR *& p)
	//	{
	//		if( p )
	//		{
	//			delete [] p, p = nullptr;
	//		}
	//	}

	//	static CHAR * StringFormat(const CHAR * fmt_str, ...)
	//	{
	//		CHAR * result = nullptr;
	//		if( nullptr != fmt_str )
	//		{
	//			va_list ap;
	//			va_start(ap, fmt_str);
	//			int len = ::_vscprintf(fmt_str, ap);
	//			if( len > 0 )
	//			{
	//				result = AllocString(len + 1);
	//					//.resize(len + 1);
	//				len = ::_vsnprintf_s(result, len + 1, _TRUNCATE, fmt_str, ap);
	//			}
	//			va_end(ap);
	//		}
	//		return result;
	//	}

		//static std::wstring StringFormatW(const WCHAR * fmt_str, ...)
		//{
		//	std::wstring result;
		//	if( nullptr != fmt_str )
		//	{
		//		va_list ap;
		//		va_start(ap, fmt_str);
		//		int len = ::_vscwprintf(fmt_str, ap);
		//		if( len > 0 )
		//		{
		//			result.resize(len + 1);
		//			len = ::_vsnwprintf_s(const_cast<WCHAR*>( result.c_str() ), len + 1, _TRUNCATE, fmt_str, ap);
		//		}
		//		va_end(ap);
		//	}
		//	return result;
		//}

		//static void Tokenize(const CHAR * string, const CHAR * delimiter, std::vector<std::string>& tokens)
		//{
		//	UINT size = static_cast<UINT>( ::strlen(string) + 1 );
		//	if( CHAR * pTemp = new(std::nothrow) CHAR[size] )
		//	{
		//		::memset(pTemp, 0x00, size);
		//		::memcpy(pTemp, string, size - 1);

		//		CHAR * lpszPosition = NULL;
		//		CHAR * lpszToken = ::strtok_s(pTemp, delimiter, &lpszPosition);
		//		while( lpszToken )
		//		{
		//			tokens.push_back(lpszToken);
		//			lpszToken = ::strtok_s(nullptr, delimiter, &lpszPosition);
		//		}

		//		delete [] pTemp, pTemp = nullptr;
		//	}
		//}

		//static void Tokenize(const WCHAR * string, const WCHAR * delimiter, std::vector<std::wstring>& tokens)
		//{
		//	UINT size = static_cast<UINT>( ::wcslen(string) + 1 );
		//	if( WCHAR * pTemp = new(std::nothrow) WCHAR[size] )
		//	{
		//		::wmemset(pTemp, 0x00, size);
		//		::wmemcpy(pTemp, string, size - 1);

		//		WCHAR * lpszPosition = NULL;
		//		WCHAR * lpszToken = ::wcstok_s(pTemp, delimiter, &lpszPosition);
		//		while( lpszToken )
		//		{
		//			tokens.push_back(lpszToken);
		//			lpszToken = ::wcstok_s(nullptr, delimiter, &lpszPosition);
		//		}

		//		delete [] pTemp, pTemp = nullptr;
		//	}
		//}
//	};
//};
