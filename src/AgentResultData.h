#pragma once

#include "Results.h"

namespace ServerEye
{
	class AgentResultData : public AgentResult
	{
	public:
		AgentResultData();
		~AgentResultData();

		bool getSuccess() override;
		wstring getMessage() override;
		const AgentPtr& getData() const override;

		void setSuccess(bool value);
		void setMessage(const wstring& message);
		void setData(const AgentPtr& ptr);

	private:
		bool m_Success;
		wstring m_Message;
		AgentPtr m_AgentPtr;
	};

	typedef shared_ptr<AgentResultData> AgentResultDataPtr;

}
