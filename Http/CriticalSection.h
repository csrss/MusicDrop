#pragma once
#include "pch.h"

namespace Http
{
	class CCriticalSection
	{
	public:
		CCriticalSection(void)
		{
			::InitializeCriticalSectionEx(&_criticalSection, _spinCount, CRITICAL_SECTION_NO_DEBUG_INFO);
		}
		virtual ~CCriticalSection(void)
		{
			::DeleteCriticalSection(&_criticalSection);
		}

		void AcquireLock(void)
		{
			::EnterCriticalSection(&_criticalSection);
		}
		void ReleaseLock(void)
		{
			::LeaveCriticalSection(&_criticalSection);
		}

		operator CRITICAL_SECTION(void) const
		{
			return _criticalSection;
		}

	protected:
		CRITICAL_SECTION	_criticalSection;
		DWORD				_spinCount;
	}; 

	class CCriticalSectionLock
	{
	public:
		CCriticalSectionLock(CCriticalSection * pCriticalSection)
		{
			if( nullptr != _pCriticalSection )
			{
				_pCriticalSection->AcquireLock();
			}
		}
		virtual ~CCriticalSectionLock(void)
		{
			if( nullptr != _pCriticalSection )
		{
			_pCriticalSection->ReleaseLock();
		}
		}

	protected:
		CCriticalSection* _pCriticalSection;
	};
};
