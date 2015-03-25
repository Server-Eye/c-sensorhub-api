#pragma once

#include "SEAPIIncludes.h"
#include "Customer.h" // private interface

namespace ServerEye
{

	class ObjectConverter
	{
	public:
		ObjectConverter();
		~ObjectConverter();

		static bool IsValidJSON(const wstring& json);

		static CreateCustomerResultPtr GetCreateCustomerResult(const wstring& json);
		static std::vector<wstring> GetCustomerList(const wstring& json);

		static ContainerResultPtr GetContainerResult(const wstring& json); 
		static AllContainersResultPtr GetAllContainersResult(const wstring& json);

		static AgentResultPtr GetAgentResult(const wstring& json);
		static AllAgentsResultPtr GetAllAgentsResult(const wstring& json);
		static NotifyResultPtr GetNotifyAgentResult(const wstring& json);

		static CreateKeyResultPtr GetCreateKeyResult(const wstring& json);

		static PushResultPtr GetPushResult(const wstring& json);
		static CustomerPtr getCustomerByEmail(const wstring& json);
		static ResultPtr GetResult(const wstring& json);
	};
}
