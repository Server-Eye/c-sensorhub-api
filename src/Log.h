#pragma once

#include "SEAPIIncludes.h"

namespace ServerEye
{
	namespace Log
	{
		void Debug(const wstring& message);
		void Info(const wstring& message);
		void Warn(const wstring& message);
		void Error(const wstring& message);
		void Fatal(const wstring& message);
		
		void enableFileLogging(const wstring& logPath, const wstring& logNamePrefix, const wstring& rotatePath);
	}
}
