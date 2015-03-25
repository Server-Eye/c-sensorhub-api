#include "defaults.h"
#include "APIConnectorImpl.h"
#include "Log.h"
#include "AgentData.h"
#include "ContainerData.h"
#include "NotifyMessageData.h"
#include "ResultData.h"
#include "ObjectConverter.h"
#include "ThreadCaller.h"
#include "StringConverter.h"
#include "TimeHelper.h"

#ifndef _WIN32
	#include <unistd.h>
	#define Sleep(x) usleep((x)*1000)
#endif


using namespace ServerEye;

void APIConnector::setProxy(const wstring& ip, const wstring& port, const wstring& user, const wstring& password)
{
	ConnectionManager::setProxy(ip, port, user, password);
}

APIConnectorPtr APIConnector::create()
{
	return APIConnectorPtr(new APIConnectorImpl());
}

APIConnectorImpl::APIConnectorImpl()
{
}

APIConnectorImpl::~APIConnectorImpl()
{
}

// API KEY
CreateKeyResultPtr APIConnectorImpl::createKey(const wstring& user, const wstring& password, const wstring& name, const wstring& customerId) const
{
	json_spirit::wObject credentials;

	credentials.push_back(json_spirit::wPair(L"email", user));
	credentials.push_back(json_spirit::wPair(L"password", password));
	credentials.push_back(json_spirit::wPair(L"type", 1));
	credentials.push_back(json_spirit::wPair(L"name", name));
	if (customerId.size())
		credentials.push_back(json_spirit::wPair(L"targetId", customerId));

	std::wstringstream os;
	json_spirit::write(credentials, os, json_spirit::pretty_print);

	wstring credentialsJSON = os.str();

	wstring result = ConnectionManager::httpPostStr(URL_SERVER_EYE + wstring(SUBJECT_AUTH_KEY), credentialsJSON);

	return ObjectConverter::GetCreateKeyResult(result);
}

// INIT
void APIConnectorImpl::init(const wstring& apiKey)
{
	m_ApiKey = apiKey;
}


// CONTAINER
ContainerResultPtr APIConnectorImpl::postContainer(const ContainerPtr& container) const
{
	return ObjectConverter::GetContainerResult(ConnectionManager::httpPostStr(URL_SERVER_EYE + wstring(SUBJECT_CONTAINER) + APIKEY_HEADER + m_ApiKey, container->getAsJSON()));
}

ContainerResultPtr APIConnectorImpl::putContainer(const ContainerPtr& container) const
{
	return ObjectConverter::GetContainerResult(ConnectionManager::httpPutStr(URL_SERVER_EYE + wstring(SUBJECT_CONTAINER) + APIKEY_HEADER + m_ApiKey, container->getAsJSON()));
}

ContainerResultPtr APIConnectorImpl::getContainer(const wstring& containerId) const
{
	return ObjectConverter::GetContainerResult(ConnectionManager::httpGetStr(URL_SERVER_EYE + wstring(SUBJECT_CONTAINER) + L"?id=" + containerId + L"&apiKey=" + m_ApiKey));
}


// AGENT
AgentResultPtr APIConnectorImpl::postAgent(const AgentPtr& agent) const
{
	return ObjectConverter::GetAgentResult(ConnectionManager::httpPostStr(URL_SERVER_EYE + wstring(SUBJECT_AGENT) + APIKEY_HEADER + m_ApiKey, agent->getAsJSON()));
}

AgentResultPtr APIConnectorImpl::putAgent(const AgentPtr& agent) const
{
	return ObjectConverter::GetAgentResult(ConnectionManager::httpPutStr(URL_SERVER_EYE + wstring(SUBJECT_AGENT) + L"?id=" + agent->getId() + L"&apiKey=" + m_ApiKey, agent->getAsJSON()));
}

AgentResultPtr APIConnectorImpl::getAgent(const wstring& agentId) const
{
	return ObjectConverter::GetAgentResult(ConnectionManager::httpGetStr(URL_SERVER_EYE + wstring(SUBJECT_AGENT) + L"?id=" + agentId + L"&apiKey=" + m_ApiKey));
}

AllAgentsResultPtr APIConnectorImpl::getAllAgents(const wstring& containerId) const
{
	return ObjectConverter::GetAllAgentsResult(ConnectionManager::httpGetStr(URL_SERVER_EYE + wstring(SUBJECT_AGENT) + L"/all/" + containerId + L"?apiKey=" + m_ApiKey));
}

NotifyResultPtr APIConnectorImpl::notifyAgent(const NotifyMessagePtr& notifyMessage) const
{
	return ObjectConverter::GetNotifyAgentResult(ConnectionManager::httpPostStr(URL_SERVER_EYE + wstring(SUBJECT_AGENT) + L"/" + notifyMessage->getAgentId() + L"/notify?apiKey=" + m_ApiKey, notifyMessage->getAsJSON()));
}


// PUSH
void APIConnectorImpl::pushStart(PushCallBackPtr& pushCallBack, const ContainerPtr& container)
{
	// Check if push is already running for this container
	if (m_PushWorkerMap.find(container->getId()) == m_PushWorkerMap.end())
	{
		// Get a pointer to the worker 
		PushWorkerPtr newPushWorker = PushWorkerPtr(new PushWorker(pushCallBack, container->getId(), m_ApiKey));

		// Create a new thread with a caller object
		boost::thread(ThreadCaller<PushWorkerPtr>(newPushWorker));

		m_PushWorkerMap.insert(PushWorkerMap::value_type(container->getId(), newPushWorker));
		Log::Debug(L"Registered push " + container->getId());
	}
	else
	{
		// Push already initiated -> we do nothing
		Log::Warn(L"Already registered push for container " + container->getId());
	}
}

void APIConnectorImpl::pushStop(const ContainerPtr& container)
{
	auto iter = m_PushWorkerMap.find(container->getId());

	if (iter != m_PushWorkerMap.end())
	{
		// Send a signoff to occ
		ResultPtr signOffRes = ObjectConverter::GetResult(ConnectionManager::httpGetStr(URL_SERVER_EYE + wstring(PUSH) + container->getId() + L"/signoff?apiKey=" + m_ApiKey));

		if (signOffRes->getSuccess())
		{
			Log::Debug(L"Push signoff accepted");

			// Stop the thread
			iter->second->StopReceive();

			// Remove from worker map
			if (m_PushWorkerMap.erase(container->getId()) != 0)
				Log::Debug(L"Unregistered push " + container->getId());
		}
		else
		{
			Log::Error(L"Server refused signoff for container " + container->getId());
		}
	}
	else
	{
		// Container ID not found -> do nothing
		Log::Warn(L"Container " + container->getId() + L" not found");
	}
}

void APIConnectorImpl::pushAck(const wstring& containerId, const wstring& messageId)
{
	json_spirit::wObject ackObj;

	ackObj.push_back(json_spirit::wPair(L"containerId", containerId));
	ackObj.push_back(json_spirit::wPair(L"messageId", messageId));

	std::wstringstream os;
	json_spirit::write(ackObj, os, json_spirit::pretty_print);

	wstring response = ConnectionManager::httpPostStr(URL_SERVER_EYE + wstring(PUSH) + containerId + L"/ack/" + messageId + L"?apikey=" + m_ApiKey, os.str());

	ResultPtr res = ObjectConverter::GetResult(response);
	if (res != nullptr && res->getSuccess())
		Log::Debug(L"MANUAL_ACK was successfull");
	else
		Log::Debug(L"MANUAL_ACK has failed: " + res->getMessage());
}

ResultPtr APIConnectorImpl::pushResult(const wstring& containerId, const wstring& messageId, bool success) const
{
	return pushResult(containerId, messageId, success, L"", L"");
}

ResultPtr APIConnectorImpl::pushResult(const wstring& containerId, const wstring& messageId, bool success, const wstring& message) const
{
	return pushResult(containerId, messageId, success, message, L"");
}

ResultPtr APIConnectorImpl::pushResult(const wstring& containerId, const wstring& messageId, bool success, const wstring& message, const wstring& data) const
{
	json_spirit::wObject resultObj;

	resultObj.push_back(json_spirit::wPair(L"containerId", containerId));
	resultObj.push_back(json_spirit::wPair(L"messageId", messageId));
	resultObj.push_back(json_spirit::wPair(L"success", success));
	resultObj.push_back(json_spirit::wPair(L"message", message));

	if (data != L"")
	{
		// try to parse json
		json_spirit::wValue value;
		if (json_spirit::read(data, value))
		{
			resultObj.push_back(json_spirit::wPair(L"data", value));
		}
		else
		{
			ResultDataPtr result(new ResultData());
			result->setSuccess(false);
			result->setMessage(L"invalid json format");
			return result;
		}
	}

	std::wstringstream os;
	json_spirit::write(resultObj, os, json_spirit::pretty_print);

	return ObjectConverter::GetResult(ConnectionManager::httpPostStr(URL_SERVER_EYE + wstring(PUSH) + containerId + L"/result/" + messageId + L"?apikey=" + m_ApiKey, os.str()));
}

#define MAX_PUSH_TIMEOUT 90000

///////////////////////////////////////////////////////////
///					APIConnectorImpl::PushWorker
///////////////////////////////////////////////////////////
APIConnectorImpl::PushWorker::PushWorker(PushCallBackPtr& pushCallBack, const wstring& containerId, const wstring& apiKey)
	: m_ApiKey(apiKey)
	, m_Callback(pushCallBack)
	, m_containerId(containerId)
	, m_ShouldStop(false)
	, m_ConnectedState(-1)
	, m_PushTimeOut(MAX_PUSH_TIMEOUT)
{
}

APIConnectorImpl::PushWorker::~PushWorker()
{
}

const wstring& APIConnectorImpl::PushWorker::getContainerId()
{
	return m_containerId;
}

void APIConnectorImpl::PushWorker::StopReceive()
{
	m_ShouldStop = true;
	m_CurlHandle.Close();
}

void APIConnectorImpl::PushWorker::execute()
{
	bool doWait = false;
	int connectionResult = -1;
	wstring response;
	m_LastConnect = TimeHelper::TickCount64();

	for (;;)
	{
		if (doWait) // delay next check
		{
			for (int i = 0; i < 10 * 10; ++i)
			{
				Sleep(100);
				if (m_ShouldStop) return;
			}
			doWait = false;
		}

		// Loop and reconnect for the long polling service
		connectionResult = ConnectionManager::httpGet(m_CurlHandle, URL_SERVER_EYE + wstring(PUSH) + m_containerId + L"?apikey=" + m_ApiKey, response);

		if (connectionResult == CURLE_OK && response.size())
		{
			m_LastConnect = TimeHelper::TickCount64();

			// Convert from json string to object
			PushResultPtr res = ObjectConverter::GetPushResult(response);

			if (res == nullptr)
			{
				m_Callback->OnError();

				if (m_ConnectedState != 0)
				{
					m_ConnectedState = 0;
					m_Callback->OnDisconnect();
				}
				doWait = true;
				continue;
			}

			// Call the callback for OnConnected()
			if (m_ConnectedState != 1)
			{
				m_ConnectedState = 1;
				m_Callback->OnConnect();
			}

			// getSuccess() == true -> there is a command inside
			if (res->getSuccess())
			{
				if (res->getControl() == L"")
				{
					// DO nothing
				}
				else if (res->getControl() == L"REMOTE_ACTION") // Install
				{
					Log::Info(L"Push REMOTE_ACTION received");

					// Installation required information
					vector<wstring> downloadUrls;
					bool checkConnector;
					wstring installUserName;
					wstring installUserPw;
					wstring domain;
					wstring innerAction;
					wstring args;

					// parse the json object in message field
					wstring pipedMessage = res->getMessage();
					vector<wstring> messageResult = StringConverter::split(pipedMessage, '|');

					json_spirit::wValue value;
					if (json_spirit::read(messageResult[2], value))
					{
						json_spirit::wObject obj = value.get_obj();
						for (size_t i = 0; i < obj.size(); i++)
						{
							if (obj[i].name_ == L"name")
							{
								innerAction = obj[i].value_.get_str();
							}
							if (obj[i].name_ == L"user")
							{
								installUserName = obj[i].value_.get_str();
							}
							if (obj[i].name_ == L"password")
							{
								installUserPw = obj[i].value_.get_str();
							}
							if (obj[i].name_ == L"domain")
							{
								domain = obj[i].value_.get_str();
							}
							if (obj[i].name_ == L"args")
							{
								args = obj[i].value_.get_str();

								// parse the next inner json 
								json_spirit::wValue argsValue;
								if (json_spirit::read(args, argsValue))
								{
									json_spirit::wObject argsObj = argsValue.get_obj();
									for (size_t k = 0; k < argsObj.size(); k++)
									{
										if (argsObj[k].name_ == L"urls")
										{
											json_spirit::wArray jsonUrlArray = argsObj[k].value_.get_array();
											for (size_t l = 0; l < jsonUrlArray.size(); l++)
											{
												downloadUrls.push_back(jsonUrlArray[l].get_str());
											}
										}
										if (argsObj[k].name_ == L"no_occ_connector_check")
										{
											checkConnector = !argsObj[k].value_.get_bool();
										}
									}
								}
								else
								{
									Log::Error(L"Failed to read inner inner json value of push");
									continue;
								}
							}
						}
					}
					else
					{
						Log::Error(L"Failed to read inner json value of push");
						continue;
					}

					sendAck(
						res->getContainerId(),
						res->getMessageId());

					m_Callback->OnRemoteInstall(
						installUserName,
						installUserPw,
						domain,
						res->getContainerId(),
						res->getControl(),
						res->getMessageId(),
						checkConnector,
						downloadUrls);

					Log::Debug(L"Message: " + res->getMessage());
				}
				else if (res->getControl() == L"ADD_OR_UPDATE_AGENT")
				{
					Log::Info(L"Push ADD_OR_UPDATE_AGENT received");

					sendAck(
						res->getContainerId(),
						res->getMessageId());

					m_Callback->OnAddOrUpdateAgent(
						res->getContainerId(),
						res->getSenderId(),
						res->getControl(),
						res->getMessageId());
				}
				else if (res->getControl() == L"REFRESH_AGENT")
				{
					Log::Info(L"Push REFRESH_AGENT received");

					sendAck(
						res->getContainerId(),
						res->getMessageId());

					m_Callback->OnRefreshAgent(
						res->getContainerId(),
						res->getSenderId(),
						res->getControl(),
						res->getMessageId());
				}
				else if (res->getControl() == L"DELETE_AGENT")
				{
					Log::Info(L"Push DELETE_AGENT received");

					sendAck(
						res->getContainerId(),
						res->getMessageId());

					m_Callback->OnDeleteAgent(
						res->getContainerId(),
						res->getSenderId(),
						res->getControl(),
						res->getMessageId());
				}
				else
				{
					Log::Error(L"Unknown command: " + res->getControl());
				}
			}
			else
			{
				Log::Debug(L"Received push - no commands");
				Sleep(20);
			}
		}
		else if (m_LastConnect + m_PushTimeOut > TimeHelper::TickCount64())
		{
			Log::Warn(L"Connection broken - waiting for timeout or reconnect...");
			doWait = true;
			continue;
		}
		else
		{
			Log::Error(L"Connection lost");
			m_Callback->OnError();

			// Disconnect case
			if (m_ConnectedState != 0)
			{
				m_ConnectedState = 0;
				m_Callback->OnDisconnect();
			}
			doWait = true;
		}

		// Leave the loop
		if (m_ShouldStop)
		{
			if (m_ConnectedState != 0)
			{
				m_ConnectedState = 0;
				m_Callback->OnDisconnect();
			}
			return;
		}
	}
}

void APIConnectorImpl::PushWorker::sendAck(const wstring& containerId, const wstring& messageId)
{
	json_spirit::wObject ackObj;

	ackObj.push_back(json_spirit::wPair(L"containerId", containerId));
	ackObj.push_back(json_spirit::wPair(L"messageId", messageId));

	std::wstringstream os;
	json_spirit::write(ackObj, os, json_spirit::pretty_print);

	wstring response = ConnectionManager::httpPostStr(URL_SERVER_EYE + wstring(PUSH) + containerId + L"/ack/" + messageId + L"?apikey=" + m_ApiKey, os.str());

	ResultPtr res = ObjectConverter::GetResult(response);
	if (res != nullptr && res->getSuccess())
		Log::Debug(L"AUTO_ACK was successfull");
}
