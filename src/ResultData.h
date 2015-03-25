#pragma once

#include "Results.h"

namespace ServerEye
{
	class ResultData : public Result
	{
	public:
		ResultData();
		~ResultData();

		virtual bool getSuccess() override;
		virtual wstring getMessage() override;

		void setSuccess(const bool value);
		void setMessage(const wstring& message);

	private:
		wstring m_Message;
		bool m_Success;
	};

	typedef shared_ptr<ResultData> ResultDataPtr;

}
