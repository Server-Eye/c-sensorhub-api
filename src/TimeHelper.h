#pragma once

namespace ServerEye
{
	class TimeHelper
	{
	public:
		static wstring getTimeStamp();
		static long long getTime();
		static unsigned long long TickCount64();
		static wstring posix_time_to_wmi_time_string(const boost::posix_time::ptime& time);
		static wstring current_time_as_wmi_time_string();

	private:
		TimeHelper();
		~TimeHelper();
	};
}
