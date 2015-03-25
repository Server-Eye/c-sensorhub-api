#pragma once

#include "Results.h"

namespace ServerEye
{
	class CreateKeyResultData : public CreateKeyResult
	{
	public:
		CreateKeyResultData();
		~CreateKeyResultData();

		bool getSuccess() override;
		wstring getMessage() override;

		const wstring& getAPIKey() const override;
		const wstring& getCustomerId() const override;
		const wstring& getUserId() const override;

		void setSuccess(bool value);
		void setMessage(const wstring& message);
		void setAPIKey(const wstring& apiKey); 
		void setCustomerId(const wstring& customerId); 
		void setUserId(const wstring& userId); 

	private:
		bool m_Success;
		wstring m_Message;
		wstring m_ApiKey;
		wstring m_CustomerId;
		wstring m_UserId;
	};

	typedef shared_ptr<CreateKeyResultData> CreateKeyResultDataPtr;

}
