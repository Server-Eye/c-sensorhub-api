#include "defaults.h"
#include "LogImpl.h"

#include <boost/log/sources/logger.hpp>
#include <boost/log/common.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/core/null_deleter.hpp>

#include <stdio.h>
#include <time.h>
#include "Log.h"
#ifdef _WIN32
#include <windows.h>
#endif


namespace Logging = boost::log;
namespace sinks = boost::log::sinks;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

// Complete file sink type
typedef sinks::synchronous_sink< sinks::text_ostream_backend > text_sink;
typedef sinks::synchronous_sink< sinks::text_file_backend > file_sink;
#ifdef _WIN32
	typedef sinks::synchronous_sink< sinks::debug_output_backend > sink_t;
#endif

using namespace ServerEye;

//static
LogImpl* LogImpl::s_instance=NULL;

// STATIC
void Log::Debug(const wstring& message)
{
	LogImpl::GetInstance()->Debug(message);
}

void Log::Info(const wstring& message)
{
	LogImpl::GetInstance()->Info(message);
}

void Log::Warn(const wstring& message)
{
	LogImpl::GetInstance()->Warn(message);
}

void Log::Error(const wstring& message)
{
	LogImpl::GetInstance()->Error(message);
}

void Log::Fatal(const wstring& message)
{
	LogImpl::GetInstance()->Fatal(message);
}

void Log::enableFileLogging(const wstring& logPath, const wstring& logNamePrefix, const wstring& rotatePath)
{
	LogImpl::CreateInstance(logPath, logNamePrefix, rotatePath);
}

LogImpl* LogImpl::GetInstance()
{
	if(s_instance == 0)
	{
		s_instance = new LogImpl();
		s_instance->init(wstring(), wstring(), wstring()); // no file logging
	}

	return s_instance;
}

void LogImpl::CreateInstance(const wstring& logPath, const wstring& logNamePrefix, const wstring& rotatePath)
{
	if(s_instance == 0)
	{
		s_instance = new LogImpl();
		s_instance->init(logPath, logNamePrefix, rotatePath);
	}
}

// DTOR & CTOR
LogImpl::LogImpl()
{
}

LogImpl::~LogImpl()
{
}

void LogImpl::Debug(const wstring& message)
{
	BOOST_LOG_SEV(m_lg, severity_level::debug) << StringConverter::ws2s_UTF8(message);
}

void LogImpl::Info(const wstring& message)
{
	BOOST_LOG_SEV(m_lg, severity_level::info) << StringConverter::ws2s_UTF8(message);
}

void LogImpl::Warn(const wstring& message)
{
	BOOST_LOG_SEV(m_lg, severity_level::warning) << StringConverter::ws2s_UTF8(message);
}

void LogImpl::Error(const wstring& message)
{
	BOOST_LOG_SEV(m_lg, severity_level::error) << StringConverter::ws2s_UTF8(message);
}

void LogImpl::Fatal(const wstring& message)
{
	BOOST_LOG_SEV(m_lg, severity_level::fatal) << StringConverter::ws2s_UTF8(message);
}

int t_daylight;
long t_dstbias;
long t_timeZone;

int getUtcOffset()
{
#ifdef _WIN32
	_tzset();

	_get_daylight(&t_daylight);
	_get_dstbias(&t_dstbias);
	_get_timezone(&t_timeZone);

	return -(((int)t_timeZone)+(t_daylight * t_dstbias));
#else
	tzset();
	return -(((int)timezone)-(daylight*3600));
#endif

}

void LogImpl::init(const wstring& logPath, const wstring& logNamePrefix, const wstring& rotatePath)
{
	boost::shared_ptr< Logging::core > core = Logging::core::get();

	if (!logNamePrefix.empty())
	{	// Create a text file sink
		boost::shared_ptr< file_sink > filesink(new file_sink(
			keywords::auto_flush = true,
			keywords::open_mode = std::ios_base::app | std::ios_base::out,
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
			keywords::rotation_size = 1 * 1024 * 1024, // limit to 1 MB 
			keywords::file_name = (logPath.empty()?L"":(logPath + L'/')) + logNamePrefix + L"_%Y%m%d.%N.log"
			));

		filesink->set_formatter
			(
			expr::stream << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y/%m/%d %H:%M:%S.%f")
			<< ": <" << Logging::trivial::severity
			<< ">\t" << expr::smessage
			);

		if (!rotatePath.empty())
		{
			// Set up where the rotated files will be stored
			filesink->locked_backend()->set_file_collector(sinks::file::make_collector(
				keywords::target = rotatePath,
				keywords::max_size = 100 * 1024 * 1024,
				keywords::min_free_space = 150 * 1024 * 1024
				));
			filesink->locked_backend()->scan_for_files();
		}
		// Upon restart, scan the directory for files matching the file_name pattern
		core->add_sink(filesink);
	}

	// Console output sink
	boost::shared_ptr<text_sink> consoleSink = boost::make_shared<text_sink>();
	consoleSink->locked_backend()->add_stream(boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
	consoleSink->set_formatter
		(
		expr::stream << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y/%m/%d %H:%M:%S.%f")
		<< ": <" << Logging::trivial::severity
		<< ">\t" << expr::smessage
		);
	core->add_sink(consoleSink);

	// Windows debugger output backend
	#ifdef _WIN32
	boost::shared_ptr<sink_t> debuggerSink(new sink_t());
	debuggerSink->set_filter(expr::is_debugger_present());
	debuggerSink->set_formatter
		(
		expr::stream << expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y/%m/%d %H:%M:%S.%f")
		<< ": <" << Logging::trivial::severity
		<< ">\t" << expr::smessage << "\n"
		);
	core->add_sink(debuggerSink);
	#endif

	core->set_filter(boost::log::trivial::severity <= boost::log::trivial::fatal);
	core->add_global_attribute("TimeStamp", attrs::utc_clock());

	Info(L"Offset to GMT is " + StringConverter::decimal2String(getUtcOffset() / 60) + L" min");
}
