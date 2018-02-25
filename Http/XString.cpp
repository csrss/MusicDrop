#include "pch.h"
#include "XString.h"

namespace Http
{
	XString::XString(void) : _pW(nullptr), _pA(nullptr)
	{
	}
	XString::XString(const WCHAR * fmt_str, ...) : _pW(nullptr), _pA(nullptr)
	{
		if( nullptr != fmt_str )
		{
			va_list ap;
			va_start(ap, fmt_str);
			int len = ::_vscwprintf(fmt_str, ap);
			if( len > 0 )
			{
				if( _pW = _xallocT<WCHAR>(len + 1) )
				{
					len = ::_vsnwprintf_s(_pW, len + 1, _TRUNCATE, fmt_str, ap);
				}
			}
			va_end(ap);
		}
	}
	XString::XString(const CHAR * fmt_str, ...) : _pW(nullptr), _pA(nullptr)
	{
		if( nullptr != fmt_str )
		{
			va_list ap;
			va_start(ap, fmt_str);
			int len = ::_vscprintf(fmt_str, ap);
			if( len > 0 )
			{
				if( _pA = _xallocT<CHAR>(len + 1) )
				{
					len = ::_vsnprintf_s(_pA, len + 1, _TRUNCATE, fmt_str, ap);
				}
			}
			va_end(ap);
		}
	}
	XString::XString(const XString& str)
	{
		*this = str;
	}
	XString::~XString(void)
	{
		_xfreeT(_pW);
		_xfreeT(_pA);
	}

	XString::operator WCHAR * (void) const
	{
		if( _pW )
		{
			return _pW;
		}
		else if( _pA )
		{
			XString::_toW();
			return _pW;
		}
		return nullptr;
	}

	XString::operator CHAR * (void) const
	{
		if( _pA )
		{
			return _pA;
		}
		else if( _pW )
		{
			XString::_toA();
			return _pA;
		}
		return nullptr;
	}

	XString::operator BYTE * (void) const
	{
		if( _pA )
		{
			return reinterpret_cast<BYTE*>( _pA );
		}
		else if( _pW )
		{
			XString::_toA();
			return reinterpret_cast<BYTE*>( _pA );
		}
		return nullptr;
	}

	XString& XString::operator = (const XString& value)
	{
		if( value._pA )
		{
			return operator = (value._pA);
		}
		else if( value._pW )
		{
			return operator = (value._pW);
		}
		return * this;
	}

	XString& XString::operator = (const WCHAR * value)
	{
		if( value )
		{
			_xfreeT(_pW);
			_xfreeT(_pA);

			if( this->_pW = this->_xallocT<WCHAR>(::wcslen(value) + 1) )
			{
				::wmemcpy(this->_pW, value, ::wcslen(value));
			}
		}
		return * this;
	}

	XString& XString::operator = (const CHAR * value)
	{
		if( value )
		{
			_xfreeT(_pW);
			_xfreeT(_pA);

			if( this->_pA = this->_xallocT<CHAR>(::strlen(value) + 1) )
			{
				::memcpy(this->_pA, value, ::strlen(value));
			}
		}
		return * this;
	}

	XString& XString::operator += (const XString& value)
	{
		if( value._pA )
		{
			return operator += (value._pA);
		}
		else if( value._pW )
		{
			return operator += (value._pW);
		}
		return * this;
	}
	XString& XString::operator += (const WCHAR * value)
	{
		if( value )
		{
			// We are appending wchar value, so if we have our inner string as multibyte - we have to convert it to wide char.
			XString::_toW();

			// At this moment we have our inner string converted.
			UINT nLength = XString::Length + ::wcslen(value) + 1;
			if( WCHAR * pTemp = new (std::nothrow) WCHAR[nLength] )
			{
				if( pTemp == static_cast<WCHAR *>( ::wmemset(pTemp, 0x00, nLength) ) )
				{
					::wmemcpy(pTemp, _pW, XString::Length);
					::wmemcpy(pTemp + XString::Length, value, ::wcslen(value));

					_xfreeT(_pW), _pW = pTemp;
				}
			}
		}
		return * this;
	}
	XString& XString::operator += (const CHAR * value)
	{
		if( value )
		{
			// We are appending char value, so if we have our inner string as wide char - we have to convert it to ansi.
			XString::_toA();

			// At this moment we have our inner string converted.
			UINT nLength = XString::Length + ::strlen(value) + 1;
			if( CHAR * pTemp = new (std::nothrow) CHAR[nLength] )
			{
				if( pTemp == static_cast<CHAR *>( ::memset(pTemp, 0x00, nLength) ) )
				{
					::memcpy(pTemp, _pA, XString::Length);
					::memcpy(pTemp + XString::Length, value, ::strlen(value));

					_xfreeT(_pA), _pA = pTemp;
				}
			}
		}
		return * this;
	}

	bool XString::operator == (const XString& value)
	{
		if( value._pA )
		{
			return operator == (value._pA);
		}
		else if( value._pW )
		{
			return operator == (value._pW);
		}
		return false;
	}
	bool XString::operator == (const WCHAR * value)
	{
		return ( 0 == ::_wcsicmp(*this, value) );
	}
	bool XString::operator == (const CHAR * value)
	{
		return ( 0 == ::_stricmp(*this, value) );
	}

	WCHAR * XString::GetW(void) const
	{
		return static_cast<WCHAR *>( *this );
	}
	CHAR * XString::GetA(void) const
	{
		return static_cast<CHAR *>( *this );
	}

	const UINT XString::GetLength(void) const
	{
		if( _pW )
		{
			return ( ::wcslen(_pW) );
		}
		else if( _pA )
		{
			return ( ::strlen(_pA) );
		}
		return 0;
	}

	const UINT XString::GetSize(void) const
	{
		if( _pW )
		{
			return ( ::wcslen(_pW) * sizeof(WCHAR) );
		}
		else if( _pA )
		{
			return ( ::strlen(_pA) * sizeof(CHAR) );
		}
		return 0;
	}

	void XString::Format(const WCHAR * fmt_str, ...)
	{
		if( nullptr != fmt_str )
		{
			va_list ap;
			va_start(ap, fmt_str);
			int len = ::_vscwprintf(fmt_str, ap);
			if( len > 0 )
			{
				if( _pW )
				{
					_xfreeT(_pW);
				}
				if( _pW = _xallocT<WCHAR>(len + 1) )
				{
					len = ::_vsnwprintf_s(_pW, len + 1, _TRUNCATE, fmt_str, ap);
				}
			}
			va_end(ap);
		}
	}
	void XString::Format(const CHAR * fmt_str, ...)
	{
		if( nullptr != fmt_str )
		{
			va_list ap;
			va_start(ap, fmt_str);
			int len = ::_vscprintf(fmt_str, ap);
			if( len > 0 )
			{
				if( _pA )
				{
					_xfreeT(_pA);
				}
				if( _pA = _xallocT<CHAR>(len + 1) )
				{
					len = ::_vsnprintf_s(_pA, len + 1, _TRUNCATE, fmt_str, ap);
				}
			}
			va_end(ap);
		}
	}

	bool XString::IsEmpty(void) const
	{
		if( _pA )
		{
			return false;
		}
		else if( _pW )
		{
			return false;
		}
		return true;
	}

	void XString::_toW(void) const
	{
		if( _pA )
		{
			// Currently our string is ANSI, we do a quick switch to unicode
			int nLengthNeeded = ::MultiByteToWideChar(CP_ACP, 0, _pA, -1, nullptr, 0);
			if( nLengthNeeded > 0 )
			{
				if( _pW = _xallocT<WCHAR>(nLengthNeeded) )
				{
					if( ::MultiByteToWideChar(CP_ACP, 0, _pA, -1, _pW, nLengthNeeded) )
					{
						_xfreeT(_pA);
					}
				}
			}
		}
	}
	void XString::_toA(void) const
	{
		if( _pW )
		{
			int nLengthNeeded = ::WideCharToMultiByte(CP_ACP, 0, _pW, -1, nullptr, 0, nullptr, nullptr);
			if( nLengthNeeded > 0 )
			{
				if( _pA = _xallocT<CHAR>(nLengthNeeded) )
				{
					if( ::WideCharToMultiByte(CP_ACP, 0, _pW, -1, _pA, nLengthNeeded, nullptr, nullptr) )
					{
						_xfreeT(_pW);
					}
				}
			}
		}
	}
};

