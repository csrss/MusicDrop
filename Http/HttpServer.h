#pragma once
#include "IHttpFileDropEvent.h"

namespace Http
{
	/*
		Wrapper class for http connection.
	*/
	public ref class CHttpServerConnection sealed
	{
	public:
		CHttpServerConnection(void)
		{
		}
		virtual ~CHttpServerConnection(void)
		{
		}

	internal:
		CHttpServerConnection(XLib::XObjectPtr<XHttpConnection> pConnection)
			:	_pConnection(pConnection)
		{
		}

	internal:
		property XLib::XObjectPtr<XHttpConnection> Connection
		{
			XLib::XObjectPtr<XHttpConnection> get(void)
			{
				return _pConnection;
			}
		}

	private:
		XLib::XObjectPtr<XHttpConnection> _pConnection;
	};

	/*
		Server is exposed to C#. By that means it has to be WinRT.
	*/
	public ref class CHttpServer sealed
	{
	public:
		CHttpServer(Platform::String^ addr, int port/*, IHttpFileDropEvent^ pHttpFileDropEvent, IHttpSongsExposeEvent^ pHttpSongsExposeEvent*/);
		virtual ~CHttpServer(void);

		// Accepts new http connection.
		CHttpServerConnection^ Accept(void);

		// Processes current http connection
		void Process(CHttpServerConnection^ pConnection);

		// Stops current http server.
		void Stop(void);

		// Event, that occures when file gets uploaded.
		property IHttpFileDropEvent^ HttpFileDropEvent
		{
			void set(IHttpFileDropEvent^ value);
		}

		// Occurs when client browser requests audio tracks list
		property IHttpSongsExposeEvent^ HttpSongsExposeEvent
		{
			void set(IHttpSongsExposeEvent^ value);
		}

	private:
		XLib::XObjectPtr<XHttpAcceptor>		_pHttpAcceptor;
		XLib::XObjectPtr<XHttpDispatchers>	_pHttpDispatchers;
		XLib::XObjectPtr<XHttpHeaderFields>	_pHttpHeaderFields;
	};
};
