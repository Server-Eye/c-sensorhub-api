#include "defaults.h"
#include "NotifyMessageData.h"
#include "Log.h"
#include "RSMData.h"

using namespace ServerEye;


// STATIC 
NotifyMessagePtr NotifyMessage::createNotifyMessage(const wstring& agentId, const wstring& parentId, bool error, const wstring& message, const RSMPtr rsm, const long long utcMs)
{
	return NotifyMessagePtr(new NotifyMessageData(agentId, parentId, error, message, rsm, utcMs));
}

NotifyMessagePtr NotifyMessage::createNotifyMessage(const wstring& agentId, const wstring& parentId, bool error, const wstring& message, const long long utcMs)
{
	return NotifyMessagePtr(new NotifyMessageData(agentId, parentId, error, message, utcMs));
}

NotifyMessagePtr NotifyMessage::createNotifyMessage(const wstring& json)
{
	json_spirit::wValue notify;
	if (!json_spirit::read(json, notify))
	{
		return NotifyMessagePtr();
	}

	shared_ptr<NotifyMessageData> notifyData(new NotifyMessageData());
	json_spirit::wObject notifyObj = notify.get_obj();

	for (size_t i = 0; i < notifyObj.size(); i++)
	{
		if (notifyObj[i].name_ == L"containerId")
		{
			notifyData->setParentId(notifyObj[i].value_.get_str());
		}
		else if (notifyObj[i].name_ == L"msg")
		{
			if (notifyObj[i].value_.type() == json_spirit::obj_type)
			{
				json_spirit::wObject msgObj = notifyObj[i].value_.get_obj();
				for (size_t i = 0; i < msgObj.size(); i++)
				{
					if (msgObj[i].name_ == L"guid")
					{
						notifyData->setAgentId(msgObj[i].value_.get_str());
					}
					else if (msgObj[i].name_ == L"error")
					{
						notifyData->setError(msgObj[i].value_.get_bool());
					}
					else if (msgObj[i].name_ == L"message")
					{
						notifyData->setMessage(msgObj[i].value_.get_str());
					}
					else if (msgObj[i].name_ == L"utcMs")
					{
						notifyData->setUtcMs(msgObj[i].value_.get_int64());
					}
					else if (msgObj[i].name_ == L"rsm")
					{
						if (msgObj[i].value_.type() == json_spirit::obj_type)
						{
							json_spirit::wObject rsmObj = msgObj[i].value_.get_obj();
							shared_ptr<RSMData> rsmData(new RSMData);
							for (size_t i = 0; i < rsmObj.size(); i++)
							{
								if (rsmObj[i].name_ == L"key")
								{
									rsmData->setKey(rsmObj[i].value_.get_str());
								}
								else if (rsmObj[i].name_ == L"version")
								{
									rsmData->setVersion(rsmObj[i].value_.get_int());
								}
								else if (rsmObj[i].name_ == L"state")
								{
									rsmData->setState((RSM::State)rsmObj[i].value_.get_int());
								}
								else if (rsmObj[i].name_ == L"data")
								{
									json_spirit::wObject rsmDataObj = rsmObj[i].value_.get_obj();
									std::wstringstream data;
									json_spirit::write(rsmDataObj, data, json_spirit::none);
									rsmData->setData(data.str());
								}
							}
							notifyData->setRSM(rsmData);
						}
					}
				}
			}
		}
	}
	return NotifyMessagePtr(notifyData);
}

// CTOR & DTOR
NotifyMessageData::NotifyMessageData(const wstring& agentId, const wstring& parentId, bool error, const wstring& message, const RSMPtr rsm, const long long utcMs)
	: m_AgentId(agentId)
	, m_ParentId(parentId)
	, m_Error(error)
	, m_Message(message)
	, m_Rsm(rsm)
	, m_UtcMs(utcMs)	
{
}

NotifyMessageData::NotifyMessageData(const wstring& agentId, const wstring& parentId, bool error, const wstring& message, const long long utcMs)
	: m_Rsm(nullptr)
	, m_AgentId(agentId)
	, m_ParentId(parentId)
	, m_Error(error)
	, m_Message(message)
	, m_UtcMs(utcMs)
{
}

NotifyMessageData::NotifyMessageData()
{
}

NotifyMessageData::~NotifyMessageData()
{
}

// GETTER
const wstring& NotifyMessageData::getParentId() const 
{
	return m_ParentId;
}

const wstring& NotifyMessageData::getAgentId() const 
{
	return m_AgentId;
}

const bool NotifyMessageData::getError() const 
{
	return m_Error;
}

const wstring& NotifyMessageData::getMessage() const 
{
	return m_Message;
}

const RSMPtr NotifyMessageData::getRSM() const 
{
	return m_Rsm;
}

const long long NotifyMessageData::getUtcMs() const 
{
	return m_UtcMs;
}

// SETTER
void NotifyMessageData::setParentId(const wstring& parentId)
{
	m_ParentId = parentId;
}

void NotifyMessageData::setAgentId(const wstring& agentId)
{
	m_AgentId = agentId;
}

void NotifyMessageData::setError(bool value)
{
	m_Error = value;
}

void NotifyMessageData::setMessage(const wstring& message)
{
	m_Message = message;
}

void NotifyMessageData::setRSM(const RSMPtr& rsm)
{
	m_Rsm = rsm;
}

void NotifyMessageData::setUtcMs(const long long utcMs)
{
	m_UtcMs = utcMs;
}

// SE Object
wstring NotifyMessageData::getAsJSON() const 
{
	json_spirit::wObject notifyMsgObj;

	notifyMsgObj.push_back( json_spirit::wPair( L"containerId", m_ParentId ) );

	json_spirit::wObject msgObj;
	msgObj.push_back( json_spirit::wPair( L"guid",		m_AgentId ) );

	// Write this only if a RSM message is attached
	if (m_Rsm != nullptr) msgObj.push_back( json_spirit::wPair( L"rsm", true));

	msgObj.push_back( json_spirit::wPair( L"error",		m_Error ) );
	msgObj.push_back( json_spirit::wPair( L"message",	m_Message ) );
	#ifdef _WIN32
	msgObj.push_back( json_spirit::wPair( L"utcMs",		m_UtcMs) );
	#else
	msgObj.push_back( json_spirit::wPair( L"utcMs",		(int64_t)m_UtcMs) );
	#endif

	// RSM 
	if(m_Rsm != nullptr)
	{
		json_spirit::wObject rsmObj;

		rsmObj.push_back( json_spirit::wPair( L"key",		m_Rsm->getKey()) );
		rsmObj.push_back( json_spirit::wPair( L"version",	m_Rsm->getVersion()) );
		rsmObj.push_back( json_spirit::wPair( L"state",		m_Rsm->getState()) );

		// Parse RSM data field to json
		if(m_Rsm->getData().size() > 0)
		{
			// Try to parse json
			json_spirit::wValue value;
			if(json_spirit::read(m_Rsm->getData(), value))
			{
				json_spirit::wObject parsedObj = value.get_obj();
				for(size_t i=0; i < parsedObj.size(); i++)
				{
					if(parsedObj[i].name_ == L"data")
					{
						rsmObj.push_back( json_spirit::wPair(L"data", parsedObj[i].value_.get_obj()) );
					}
				}
			}
			else
			{
				Log::Error(L"Cannot convert NotifyMessage - invalid json format in RSM->getData()");
				return L"";
			}
		}

		msgObj.push_back( json_spirit::wPair(L"rsm", rsmObj) );
	}

	notifyMsgObj.push_back( json_spirit::wPair( L"msg", msgObj) );

	std::wstringstream os;
	json_spirit::write(notifyMsgObj, os, json_spirit::none);

	return os.str();
}
