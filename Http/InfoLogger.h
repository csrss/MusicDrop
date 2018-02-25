#pragma once
#include "CriticalSection.h"
//
//namespace Http
//{
//	class CInfoLogger
//	{
//	protected:
//		CInfoLogger(void);
//		~CInfoLogger(void);
//
//	public:
//		static CInfoLogger * Instance(void);
//
//		void Log(const std::wstring& strMessage);
//		void LogFormat(WCHAR * lpwszFormat, ...);
//
//	private:
//		CInfoLogger(CInfoLogger const&);
//		CInfoLogger& operator = (CInfoLogger const&);
//		static CInfoLogger* _instance;
//
//	protected:
//		CCriticalSection _cs;
//		std::wstring	 _fl;
//		
//		HANDLE			_hFile;
//	};
//};
