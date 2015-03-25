#pragma once

#include "SEAPIIncludes.h"
#include "RSM.h"

namespace ServerEye
{
	class NotifyMessage : public SEObject
	{
	public:

		static NotifyMessagePtr createNotifyMessage(const wstring& agentId, const wstring& parentId, bool error, const wstring& message,
			const RSMPtr rsm, const long long utcMs);

		static NotifyMessagePtr createNotifyMessage(const wstring& agentId, const wstring& parentId, bool error, const wstring& message,
			const long long utcMs);

		static NotifyMessagePtr createNotifyMessage(const wstring& json);

		virtual const wstring& getParentId() const = 0;
		virtual const wstring& getAgentId() const = 0;
		virtual const bool getError() const = 0;
		virtual const wstring& getMessage() const = 0;
		virtual const RSMPtr getRSM() const = 0;
		virtual const long long getUtcMs() const = 0;
	};
}
