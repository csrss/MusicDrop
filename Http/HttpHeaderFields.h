#pragma once

namespace Http
{
	namespace Header
	{
		class CHttpHeaderFieldFileAlbum : public IHttpHeaderField
		{
		public:
			CHttpHeaderFieldFileAlbum(void)
				:	IHttpHeaderField("X-FILEALBUM:\\s+(.*)\\s*")
			{
			}
			virtual ~CHttpHeaderFieldFileAlbum(void)
			{
			}

			virtual void Parse(const std::vector<std::string>& matches) override
			{
				Value = matches.at(0);
			}

		public:
			std::string Value;
		};

		//-----------------------------------------------------------------------------1

		class CHttpHeaderFieldFileArtist : public IHttpHeaderField
		{
		public:
			CHttpHeaderFieldFileArtist(void)
				:	IHttpHeaderField("X-FILEARTIST:\\s+(.*)\\s*")
			{
			}
			virtual ~CHttpHeaderFieldFileArtist(void)
			{
			}

			virtual void Parse(const std::vector<std::string>& matches) override
			{
				Value = matches.at(0);
			}

		public:
			std::string Value;
		};

		//-----------------------------------------------------------------------------2

		class CHttpHeaderFieldFileChunkSize : public IHttpHeaderField
		{
		public:
			CHttpHeaderFieldFileChunkSize(void)
				:	IHttpHeaderField("X-FILECHUNKSIZE:\\s+(.*)\\s*"), Value(0)
			{
			}
			virtual ~CHttpHeaderFieldFileChunkSize(void)
			{
			}

			virtual void Parse(const std::vector<std::string>& matches) override
			{
				Value = ::atoi(matches.at(0).c_str());
			}

		public:
			UINT Value;
		};

		//-----------------------------------------------------------------------------3

		class CHttpHeaderFieldFileName : public IHttpHeaderField
		{
		public:
			CHttpHeaderFieldFileName(void)
				:	IHttpHeaderField("X-FILENAME:\\s+(.*)\\s*")
			{
			}
			virtual ~CHttpHeaderFieldFileName(void)
			{
			}

			virtual void Parse(const std::vector<std::string>& matches) override
			{
				Value = matches.at(0);
			}

		public:
			std::string Value;
		};

		//-----------------------------------------------------------------------------4

		class CHttpHeaderFieldFilePosition : public IHttpHeaderField
		{
		public:
			CHttpHeaderFieldFilePosition(void)
				:	IHttpHeaderField("X-FILEPOSITION:\\s+(.*)\\s*"), Value(0)
			{
			}
			virtual ~CHttpHeaderFieldFilePosition(void)
			{
			}

			virtual void Parse(const std::vector<std::string>& matches) override
			{
				Value = ::atoi(matches.at(0).c_str());
			}

		public:
			UINT Value;
		};

		//-----------------------------------------------------------------------------5

		class CHttpHeaderFieldFileSize : public IHttpHeaderField
		{
		public:
			CHttpHeaderFieldFileSize(void)
				:	IHttpHeaderField("X-FILESIZE:\\s+(.*)\\s*"), Value(0)
			{
			}
			virtual ~CHttpHeaderFieldFileSize(void)
			{
			}

			virtual void Parse(const std::vector<std::string>& matches) override
			{
				Value = ::atoi(matches.at(0).c_str());
			}

		public:
			size_t Value;
		};

		//-----------------------------------------------------------------------------6

		class CHttpHeaderFieldFileType : public IHttpHeaderField
		{
		public:
			CHttpHeaderFieldFileType(void)
				:	IHttpHeaderField("X-FILETYPE:\\s+(.*)\\s*")
			{
			}
			virtual ~CHttpHeaderFieldFileType(void)
			{
			}

			virtual void Parse(const std::vector<std::string>& matches) override
			{
				Value = matches.at(0);
			}

		public:
			std::string Value;
		};

		//-----------------------------------------------------------------------------7


	};
};
