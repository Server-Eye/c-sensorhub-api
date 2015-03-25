#pragma once

#include "APIConnector.h"
#include "ConnectionManager.h"

namespace ServerEye
{
	typedef shared_ptr<boost::thread> ThreadPtr;

	class APIConnectorImpl : public APIConnector
	{
	public:
		APIConnectorImpl();
		~APIConnectorImpl();

		// API KEY
		CreateKeyResultPtr createKey(const wstring& user, const wstring& password, const wstring& name, const wstring& customerId) const override;

		// INIT API 
		void init(const wstring& apiKey) override;

		// PUSH
		virtual void pushStart(PushCallBackPtr& pushCallBack, const ContainerPtr& container) override;
		virtual void pushStop(const ContainerPtr& container) override;								
		virtual void pushAck(const wstring& containerId, const wstring& messageId) override;
		virtual ResultPtr pushResult(const wstring& containerId, const wstring& messageId, bool success) const override;
		virtual ResultPtr pushResult(const wstring& containerId, const wstring& messageId, bool success, const wstring& message) const override;
		virtual ResultPtr pushResult(const wstring& containerId, const wstring& messageId, bool success, const wstring& message, const wstring& data) const override;

		// CONTAINER
		virtual ContainerResultPtr postContainer(const ContainerPtr& container) const override;
		virtual ContainerResultPtr putContainer(const ContainerPtr& container) const override;
		virtual ContainerResultPtr getContainer(const wstring& containerId) const override;

		// AGENT
		virtual AgentResultPtr postAgent(const AgentPtr& agent) const override;
		virtual AgentResultPtr putAgent(const AgentPtr& agent) const override;
		virtual AgentResultPtr getAgent(const wstring& agentId) const override;
		virtual AllAgentsResultPtr getAllAgents(const wstring& containerId) const override;
		virtual NotifyResultPtr notifyAgent(const NotifyMessagePtr& notifyMessage) const override;

		class PushWorker
		{
			PushWorker(const PushWorker& other);
		public:
			PushWorker(PushCallBackPtr& pushCallBack, const wstring& containerId, const wstring& apiKey);
			~PushWorker();

			void execute();
			void StopReceive();
			void sendAck(const wstring& containerId, const wstring& messageId);
			const wstring& getContainerId();

		private:
			ConnectionManager::CurlHandle m_CurlHandle;
			bool m_ShouldStop;
			PushCallBackPtr& m_Callback;
			wstring m_containerId;
			wstring m_ApiKey;
			ThreadPtr m_Thread;
			int m_ConnectedState; // -1 not yet set
			unsigned long long m_LastConnect;
			unsigned long long m_PushTimeOut;
		};

		typedef shared_ptr<PushWorker> PushWorkerPtr;

		typedef std::map<std::wstring, PushWorkerPtr> PushWorkerMap;

	private:
		PushWorkerMap m_PushWorkerMap;
		wstring m_ApiKey;
	};
}
