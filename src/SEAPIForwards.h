#pragma once

namespace ServerEye
{
	class APIConnector;
	typedef shared_ptr<APIConnector> APIConnectorPtr;

	class Container;
	typedef shared_ptr<Container> ContainerPtr;

	class Agent;
	typedef shared_ptr<Agent> AgentPtr;

	class AgentSetting;
	typedef shared_ptr<AgentSetting> AgentSettingPtr;

	class NotifyMessage;
	typedef shared_ptr<NotifyMessage> NotifyMessagePtr;

	class RSM;
	typedef shared_ptr<RSM> RSMPtr;

	class Result;
	typedef shared_ptr<Result> ResultPtr;

	class ContainerResult;
	typedef shared_ptr<ContainerResult> ContainerResultPtr;

	class AgentResult;
	typedef shared_ptr<AgentResult> AgentResultPtr;

	class AllAgentsResult;
	typedef shared_ptr<AllAgentsResult> AllAgentsResultPtr;
	typedef std::vector<AgentPtr> AgentList;

	class AllContainersResult;
	typedef shared_ptr<AllContainersResult> AllContainersResultPtr;
	typedef std::vector<ContainerPtr> ContainerList;

	class CreateKeyResult;
	typedef shared_ptr<CreateKeyResult> CreateKeyResultPtr;

	class PushResult;
	typedef shared_ptr<PushResult> PushResultPtr;

	class PushCallBack;
	typedef shared_ptr<PushCallBack> PushCallBackPtr;

	class NotifyResult;
	typedef shared_ptr<NotifyResult> NotifyResultPtr;

}
