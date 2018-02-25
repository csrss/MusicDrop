#pragma once

namespace Http
{
	/*
		Switchable string, that can be both: wide char and ansi.
	*/
	class XString
	{
	public:
		XString(void);
		XString(const WCHAR * fmt_str, ...);
		XString(const CHAR * fmt_str, ...);
		XString(const XString& str);
		virtual ~XString(void);

		operator WCHAR * (void) const;
		operator CHAR * (void) const;
		operator BYTE * (void) const;

		XString& operator = (const XString& value);
		XString& operator = (const WCHAR * value);
		XString& operator = (const CHAR * value);

		XString& operator += (const XString& value);
		XString& operator += (const WCHAR * value);
		XString& operator += (const CHAR * value);


		bool operator == (const XString& value);
		bool operator == (const WCHAR * value);
		bool operator == (const CHAR * value);

		__declspec( property(get = GetW) ) WCHAR* W;
		WCHAR * GetW(void) const;

		__declspec( property(get = GetA) ) CHAR* A;
		CHAR * GetA(void) const;

		/*
			Returns string length. In both cases it will be the same.
		*/
		__declspec( property(get = GetLength) ) UINT Length;
		const UINT GetLength(void) const;

		/*
			Returns string size. It will be different depending on current string type.
		*/
		__declspec( property(get = GetSize) ) UINT Size;
		const UINT GetSize(void) const;

		void Format(const WCHAR * fmt_str, ...);
		void Format(const CHAR * fmt_str, ...);

		__declspec( property(get = IsEmpty) ) bool Empty;
		bool IsEmpty(void) const;

	protected:
		template <class T> T * _xallocT(const UINT& nLen) const
		{
			if( T * p = new (std::nothrow) T[nLen] )
			{
				return reinterpret_cast<T *>( ::memset(p, 0x00, nLen * sizeof(T)) );
			}
			return nullptr;
		}

		template <class T> void _xfreeT(T *& p) const
		{
			if( p )
			{
				delete [] p, p = nullptr;
			}
		}

		void _toW(void) const;
		void _toA(void) const;

	protected:
		mutable WCHAR * _pW;
		mutable CHAR  *	_pA;
	};
};
