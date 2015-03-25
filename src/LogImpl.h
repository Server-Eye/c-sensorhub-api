#pragma once

#include "SEAPIIncludes.h"

#include <boost/log/sources/logger.hpp>
#include <boost/log/common.hpp>
#include <boost/log/trivial.hpp>

namespace ServerEye
{
	class LogImpl
	{
	public:
		typedef boost::log::trivial::severity_level severity_level;
		typedef boost::log::sources::severity_logger<severity_level> Logger;

		static LogImpl* GetInstance();
		static void CreateInstance(const wstring& logPath, const wstring& logNamePrefix, const wstring& rotatePath);

		LogImpl();
		~LogImpl();

		void init(const wstring& logPath, const wstring& logNamePrefix, const wstring& rotatePath);

		void Debug(const wstring& message);
		void Info(const wstring& message);
		void Warn(const wstring& message);
		void Error(const wstring& message);
		void Fatal(const wstring& message);

		static LogImpl* s_instance;
		Logger m_lg;
	};
}
