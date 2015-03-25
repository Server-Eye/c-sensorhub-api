#pragma once

#include "SEAPIIncludes.h"

namespace ServerEye
{
	class RSM 
	{
	public:

		enum State 
		{
			Information,
			OK,
			Warning,
			Error,
			Fatal,
			ForcedError,
			ParamMissing,
			Hidden
		};

		static const RSMPtr createRSM(const wstring& key, const int version, const State state, const wstring& data);

		virtual const wstring& getKey() const = 0;
		virtual const int getVersion() const = 0;
		virtual const wstring& getMessage() const = 0;
		virtual const State& getState() const = 0;
		virtual const wstring& getData() const = 0;

		virtual void setMessage(const wstring& message) = 0;
	};
}
