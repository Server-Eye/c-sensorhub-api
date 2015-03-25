#pragma once

#include "Results.h"
#include "Customer.h"

namespace ServerEye
{

	class CreateCustomerResultData : public CreateCustomerResult
	{
	public:
		CreateCustomerResultData();
		CreateCustomerResultData(bool isCreated, const wstring& message, CustomerPtr& customer);
		~CreateCustomerResultData();

		bool isCreated() override;
		wstring getMessage() override;
		CustomerPtr getCustomer() override;

		void setIsCreated(bool value);
		void setMessage(const wstring& message);
		void setCustomer(CustomerPtr& customer);

	private:
		CustomerPtr m_Customer;
		wstring m_Message;
		bool m_IsCreated;
	};

	typedef shared_ptr<CreateCustomerResultData> CreateCustomerResultDataPtr;

}
