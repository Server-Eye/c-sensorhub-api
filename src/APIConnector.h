#pragma once

#include "SEAPIIncludes.h"
#include "Results.h"
#include "Agent.h"
#include "Container.h"
#include "PushCallBack.h"

namespace ServerEye
{
	class APIConnector
	{
	public:
		virtual ~APIConnector() {}

		static void setProxy(const wstring& ip, const wstring& port, const wstring& user, const wstring& password); // set a http proxy

		static APIConnectorPtr create();

		// API KEY
		virtual CreateKeyResultPtr createKey(const wstring& user, const wstring& password, const wstring& name, const wstring& customerId) const = 0;	// Creates a new API key by user and password

		// INIT API 
		virtual void init(const wstring& apiKey) = 0;													// Configures your API key 

		// CONTAINER
		virtual ContainerResultPtr getContainer(const wstring& containerId) const = 0;					// Get an existing container item by id
		virtual ContainerResultPtr postContainer(const ContainerPtr& container) const= 0;				// Creates a new container
		virtual ContainerResultPtr putContainer(const ContainerPtr& container) const = 0;				// Updates an existing container

		// AGENTS
		virtual AgentResultPtr getAgent(const wstring& agentId) const = 0;								// Get an existing agent by id
		virtual AllAgentsResultPtr getAllAgents(const wstring& containerId) const = 0;					// Get all agents by container id
		virtual AgentResultPtr postAgent(const AgentPtr& agent) const = 0;								// Creates new agent
		virtual AgentResultPtr putAgent(const AgentPtr& agent) const = 0;								// Updates an existing agent
		virtual NotifyResultPtr notifyAgent(const NotifyMessagePtr& notifyMessage) const = 0;			// Sends a notification message for an agent

		// PUSH
		virtual void pushStart(PushCallBackPtr& pushCallBack, const ContainerPtr& container) = 0;			// Starts the push service
		virtual void pushStop(const ContainerPtr& container) = 0;										// Stopps the push service
		virtual void pushAck(const wstring& containerId, const wstring& messageId) = 0;					// Sends an ack for an inbound push event
		virtual ResultPtr pushResult(const wstring& containerId, const wstring& messageId, bool success) const = 0;
		virtual ResultPtr pushResult(const wstring& containerId, const wstring& messageId, bool success, const wstring& message) const = 0;
		virtual ResultPtr pushResult(const wstring& containerId, const wstring& messageId, bool success, const wstring& message, const wstring& data) const = 0;

	};
}
