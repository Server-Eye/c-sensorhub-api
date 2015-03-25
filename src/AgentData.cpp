#include "defaults.h"
#include "AgentData.h"

using namespace ServerEye;

const wstring Agent::TYPE_EXTERNAL_HARDWARE(L"EXTERNAL_HARDWARE");
const wstring Agent::TYPE_EXTERNAL_SOFTWARE(L"EXTERNAL_SOFTWARE");
const wstring Agent::TYPE_EXTERNAL_NETWORK(L"EXTERNAL_NETWORK");

///////////////////////////////////////////////////////////
///					SEOBJECT INTERFACE
///////////////////////////////////////////////////////////
wstring AgentData::getAsJSON() const
{
	// AGENT INFORMATION
	json_spirit::wObject agentObj;
	agentObj.push_back( json_spirit::wPair( L"parentId", m_ParentId ) );
	agentObj.push_back( json_spirit::wPair( L"name", m_Name ) );
	agentObj.push_back( json_spirit::wPair( L"type", m_AgentType ) );

	// SETTINGS
	json_spirit::wArray settings;
	for (auto iter = m_settings.begin(); iter != m_settings.end(); iter++) 
	{
		json_spirit::wObject curSetting;
		curSetting.push_back( json_spirit::wPair( L"key", iter->second->getKey()) );
		curSetting.push_back( json_spirit::wPair( L"value", iter->second->getValue()) );
		curSetting.push_back( json_spirit::wPair( L"definition", iter->second->getDefinition()) );
		curSetting.push_back( json_spirit::wPair( L"orderInt", iter->second->getOrderInt()) );

		settings.push_back( curSetting );
	}

	agentObj.push_back( json_spirit::wPair( L"settings", settings) );

	std::wstringstream os;
	json_spirit::write( agentObj, os, json_spirit::none );

	return os.str();
}

///////////////////////////////////////////////////////////
///						AGENTDATA
///////////////////////////////////////////////////////////

// STATIC
AgentPtr Agent::createAgent(const wstring& parentId, const wstring& name, const wstring& type)
{
	return AgentPtr(new AgentData(parentId, name, type));
}

// CTOR & DTOR
AgentData::AgentData(const wstring& parentId, const wstring& name, const wstring& type)
{
	m_ParentId = parentId;
	m_Name = name;
	m_AgentType = type;
}

AgentData::AgentData()
{
}

AgentData::~AgentData()
{
}

// GETTERS
const wstring& AgentData::getParentId() const 
{
	return m_ParentId;
}

const wstring& AgentData::getName() const 
{
	return m_Name;
}

const wstring& AgentData::getId() const
{
	return m_Id;
}

const wstring& AgentData::getType() const 
{
	return m_AgentType;
}

// SETTERS
void AgentData::setName(const wstring& name) 
{
	m_Name = name;
}

void AgentData::setId(const wstring& id)
{
	m_Id = id;
}

void AgentData::setParentId(const wstring& parentId)
{
	m_ParentId = parentId;
}

void AgentData::setType(const wstring& type)
{
	m_AgentType = type;
}

// SETTING 
bool AgentData::hasSetting(const wstring& key) const 
{
	return m_settings.find(key) != m_settings.end();
}

bool AgentData::removeSetting(const wstring& key) 
{
	return m_settings.erase(key) != 0;
}

AgentSettingPtr AgentData::getSetting(const wstring& key) const 
{
	auto iter = m_settings.find(key);
	if (iter != m_settings.end())
		return iter->second;
	return nullptr;
}

const Agent::SettingMap& AgentData::getAllSettings() const
{
	return m_settings;
}

void AgentData::setSetting(const AgentSettingPtr& setting) 
{
	auto iter=m_settings.insert(SettingMap::value_type(setting->getKey(),setting));
	if (!iter.second) // bool which tell it is inserted or not
		(iter.first)->second=setting; // first is the iterator to the element and its second is the value
}

///////////////////////////////////////////////////////////
///						AGENTDATA::AGENTSETTING
///////////////////////////////////////////////////////////

// STATIC
AgentSettingPtr AgentSetting::createSetting(const wstring& key)	
{
	return AgentSettingPtr(new AgentData::AgentSettingData(key));
}

// CTOR & DTOR
AgentData::AgentSettingData::AgentSettingData(const wstring& key)
{
	m_Key = key;
}

AgentData::AgentSettingData::AgentSettingData()
{
}

AgentData::AgentSettingData::~AgentSettingData()
{
}

// SETTERS
void AgentData::AgentSettingData::setKey(const wstring& key) 
{
	m_Key = key;
}				

void AgentData::AgentSettingData::setValue(const wstring& value) 
{
	m_Value = value;
}

void AgentData::AgentSettingData::setOrderId(const int orderId) 
{
	m_OrderId = orderId;
}

void AgentData::AgentSettingData::setEditable(const bool value) 
{
	m_IsEditable = value;
}			

void AgentData::AgentSettingData::setDefinition(const wstring& definition) 
{
	m_Definition = definition;
}

// GETTERS
const wstring& AgentData::AgentSettingData::getValue() const 
{
	return m_Value;
}

const wstring& AgentData::AgentSettingData::getKey() const 
{
	return m_Key;
}

bool AgentData::AgentSettingData::getEditable() const 
{
	return m_IsEditable;
}

int AgentData::AgentSettingData::getOrderInt() const 
{
	return m_OrderId;
}

const wstring& AgentData::AgentSettingData::getDefinition() const 
{
	return m_Definition;
}
