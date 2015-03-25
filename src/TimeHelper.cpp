#include "defaults.h"
#include "TimeHelper.h"
#include <time.h> 
#include <sstream>

#ifndef _WIN32
	#include "sys/times.h"
#endif

using namespace ServerEye;

TimeHelper::TimeHelper()
{
}

TimeHelper::~TimeHelper()
{
}

wstring TimeHelper::getTimeStamp()
{
	std::wstringstream os;
	os << getTime();
	return wstring(os.str());
}

wstring TimeHelper::posix_time_to_wmi_time_string(const boost::posix_time::ptime& time)
{
	std::locale loc(std::wcout.getloc(), new boost::posix_time::wtime_facet(L"%Y%m%d%H%M%S.%f"));
	std::wstringstream wss;
	wss.imbue(loc);
	wss << time;
	wss.flush();
	//Append timezone offset (0 as we use UTC)
	return wss.str() + L"000";
}

wstring TimeHelper::current_time_as_wmi_time_string()
{
	return posix_time_to_wmi_time_string(boost::posix_time::second_clock::universal_time());
}

long long TimeHelper::getTime()
{
	time_t timer;
	time(&timer);
	return timer*1000;
}

#ifdef _WIN32
typedef DWORD TICKTYPE;
#define TickCount (TICKTYPE)GetTickCount
#else
typedef clock_t TICKTYPE;
TICKTYPE TickCount()
{
	//	calculate the millisecond factor
	static long div = 1000 / sysconf(_SC_CLK_TCK);
	return (TICKTYPE)(times(NULL)*div);
}

#endif


struct Ticker
{
	unsigned long long lTicks;		// last updated tick count without overflow;
	unsigned int lTick; // last updated tick count;
	// constructor 
	Ticker();
	// initialize the lTicks. Called by constructor
	unsigned long long start();
	// returns tickcount without the 32 bit overflow. This must not be the system start
	unsigned long long ticks();
};

// implement 64 with 32 bit function because the function 50% faster and
// under Windows the 64 bit function is only available for Vista and Server 2008 
static Ticker _g_ticker;

unsigned long long TimeHelper::TickCount64()
{
	static boost::mutex s_lock;
	boost::interprocess::scoped_lock<boost::mutex> lock(s_lock);
	auto _cTick = _g_ticker.ticks();
	return _cTick;
}


Ticker::Ticker()
:lTicks(0)
,lTick(0)
{
	start();
}

unsigned long long Ticker::start()
{
	lTick = (unsigned int)(lTicks = TimeHelper::TickCount64());
	return lTicks;
}

unsigned long long Ticker::ticks()
{
	unsigned int _cTick = TickCount();
	unsigned int diff = (unsigned int)(_cTick - lTick);
	if (diff<0xFFFF0000) // avoid back adjustments, allow only forwards. 
	{	// This will fail if the function is only called after 49,7095111.. days
		// or a back adjustment has more than 65535 milliseconds, but back adjustments were only seen with 1 millisecond
		// because the adjustment is done with microseconds
		lTicks += diff;
		// remember last tick
		lTick = _cTick;
	}
	return lTicks;
}


