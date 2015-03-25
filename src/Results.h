#pragma once

#include "SEAPIIncludes.h"

namespace ServerEye
{
	class Result
	{
	public:
		virtual ~Result() {}
		virtual bool getSuccess() = 0;
		virtual wstring getMessage() = 0;
	};


	class ContainerResult : public Result
	{
	public:
		virtual const ContainerPtr& getData() const = 0;
	};

	class AgentResult : public Result
	{
	public:
		virtual const AgentPtr& getData() const = 0;
	};

	class AllAgentsResult : public Result
	{
	public:
		virtual const AgentList& getData() const = 0;
	};

	class CreateKeyResult : public Result
	{
	public:
		virtual const wstring& getAPIKey() const = 0;
		virtual const wstring& getCustomerId() const = 0;
		virtual const wstring& getUserId() const = 0;
	};

	class PushResult : public Result
	{
	public:
		virtual const wstring& getRootContainerId() const = 0;
		virtual const wstring& getContainerId() const = 0;
		virtual const wstring& getSenderId() const = 0;
		virtual const wstring& getMessageId() const = 0;
		virtual const wstring& getControl() const = 0;
		virtual const wstring& getData() const = 0;
	};

	class NotifyResult : public Result
	{
	public :
		virtual const wstring& getData() const = 0;
	};
}
