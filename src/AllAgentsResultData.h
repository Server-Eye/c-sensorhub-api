#pragma once

#include "Results.h"

namespace ServerEye
{
	class AllAgentsResultData : public AllAgentsResult
	{
	public:
		AllAgentsResultData();
		~AllAgentsResultData();

		bool getSuccess() override;
		wstring getMessage() override;
		const AgentList& getData() const override;

		void setSuccess(bool value);
		void setMessage(const wstring& message);
		void setData(const AgentList& list);

	private:
		bool m_Success;
		wstring m_Message;
		AgentList m_AgentList;
	};

	typedef shared_ptr<AllAgentsResultData> AllAgentsResultDataPtr;

}
