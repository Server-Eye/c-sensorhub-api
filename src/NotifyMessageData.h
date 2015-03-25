#pragma once

#include "NotifyMessage.h"

namespace ServerEye
{
	class NotifyMessageData : public NotifyMessage
	{
	public:

		NotifyMessageData(const wstring& agentId, const wstring& parentId, bool error, const wstring& message, const RSMPtr rsm, const long long utcMs);
		NotifyMessageData(const wstring& agentId, const wstring& parentId, bool error, const wstring& message, const long long utcMs);

		NotifyMessageData();
		~NotifyMessageData();

		const wstring& getParentId() const override;
		const wstring& getAgentId() const override;
		const bool getError() const override;
		const wstring& getMessage() const override;
		const RSMPtr getRSM() const override;
		const long long getUtcMs() const override;

		void setParentId(const wstring& parentId);
		void setAgentId(const wstring& agentId);
		void setError(bool value);
		void setMessage(const wstring& message);
		void setRSM(const RSMPtr& rsm);
		void setUtcMs(const long long utcMs);

		// SE Object
		wstring getAsJSON() const override;

	private:
		wstring m_ParentId;
		wstring m_AgentId;
		bool m_Error;
		wstring m_Message;
		RSMPtr m_Rsm;
		long long m_UtcMs;
	};
}
