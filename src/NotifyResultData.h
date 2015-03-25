#pragma once

#include "Results.h"

namespace ServerEye
{

	class NotifyResultData : public NotifyResult
	{
	public:
		NotifyResultData();
		~NotifyResultData();

		const wstring& getData() const override;
		bool getSuccess() override;
		wstring getMessage() override;

		void setData(const wstring& data);
		void setSuccess(const bool value);
		void setMessage(const wstring& message);

	private:
		wstring m_Data;
		bool m_Success;
		wstring m_Message;
	};

	typedef shared_ptr<NotifyResultData> NotifyResultDataPtr;

}
