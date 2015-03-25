#include "defaults.h"
#include "ContainerData.h"

using namespace ServerEye;

///////////////////////////////////////////////////////////
///					SEOBJECT INTERFACE
///////////////////////////////////////////////////////////
wstring ContainerData::getAsJSON() const
{
	// CONTAINER
	json_spirit::wObject containerObj;
	containerObj.push_back( json_spirit::wPair( L"id", m_Id ) );
	containerObj.push_back( json_spirit::wPair( L"name", m_Name ) );
	containerObj.push_back( json_spirit::wPair( L"type", m_ContainerType ) );
	containerObj.push_back( json_spirit::wPair( L"customerId", m_CustomerId ) );
	containerObj.push_back( json_spirit::wPair( L"realName", m_RealName ) );
	containerObj.push_back( json_spirit::wPair( L"maxHeartbeatTimeout", m_maxHeartbeatTimeout ) );
	containerObj.push_back( json_spirit::wPair( L"parentId", m_ParentId ) );

	// SYSTEM
	json_spirit::wObject systemObj;
	systemObj.push_back( json_spirit::wPair( L"ip", m_SystemData.getIp() ) );
	systemObj.push_back( json_spirit::wPair( L"isServer", m_SystemData.isServer() ) );
	systemObj.push_back( json_spirit::wPair( L"isVm", m_SystemData.isVM() ) );
	systemObj.push_back( json_spirit::wPair( L"numberOfProcessors", m_SystemData.getNumberOfProcessors() ) );
	#ifdef _WIN32
	systemObj.push_back(json_spirit::wPair(L"totalRam", m_SystemData.getTotalRam()));	
	#else
	systemObj.push_back(json_spirit::wPair(L"totalRam", (int64_t)m_SystemData.getTotalRam()));
	#endif
	systemObj.push_back(json_spirit::wPair(L"architecture", m_SystemData.getArchitecture()));
	containerObj.push_back( json_spirit::wPair( L"system", systemObj) );

	// ADDITIONAL
	json_spirit::wObject additionalObj;
	additionalObj.push_back( json_spirit::wPair( L"name", m_AdditionalData.getName() ) );
	additionalObj.push_back( json_spirit::wPair( L"version", m_AdditionalData.getVersion() ) );
	additionalObj.push_back( json_spirit::wPair( L"servicepack", m_AdditionalData.getServicePack() ) );
	#ifdef _WIN32
	additionalObj.push_back( json_spirit::wPair( L"lastStart", m_AdditionalData.getLastStart() ) );
	#else
	additionalObj.push_back( json_spirit::wPair( L"lastStart", (int64_t)m_AdditionalData.getLastStart() ) );
	#endif
	containerObj.push_back( json_spirit::wPair( L"additional", additionalObj) );

	std::wstringstream os;
	json_spirit::write( containerObj, os, json_spirit::none );

	return os.str();
}

///////////////////////////////////////////////////////////
///						CONTAINER
///////////////////////////////////////////////////////////

//STATIC
ContainerPtr Container::createContainer(const wstring& customerId, const wstring& name, const wstring& realName, const ContainerType type)
{
	return ContainerPtr(new ContainerData(customerId, name, realName, type));
}

// CTOR & DTOR
ContainerData::ContainerData(wstring customerId, wstring name, wstring realName, ContainerType type) 
	:m_ParentId(L"")
	,m_maxHeartbeatTimeout(0)
	,m_Id(L"")
{
	m_CustomerId = customerId;
	m_Name = name;
	m_RealName = realName;
	m_ContainerType = type;
}

ContainerData::ContainerData() 
	:m_CustomerId(L"")
	,m_Name(L"")
	,m_RealName(L"")
	,m_ContainerType(EXTERNAL)
	,m_ParentId(L"")
	,m_maxHeartbeatTimeout(0)
	,m_Id(L"")
{
}

ContainerData::~ContainerData()
{
}

// GETTER
const wstring& ContainerData::getId() const
{
	return m_Id;
}

const wstring& ContainerData::getRealName() const
{
	return m_RealName;
}		

const wstring& ContainerData::getCustomerId() const
{
	return m_CustomerId;
}	

const wstring& ContainerData::getParentId() const
{
	return m_ParentId;
}

const wstring& ContainerData::getName() const
{
	return m_Name;
}

Container::ContainerType ContainerData::getType() const
{
	return m_ContainerType;
}

int ContainerData::getMaxHeartbeatTimeout() const
{
	return m_maxHeartbeatTimeout;
}

// SETTER
void ContainerData::setId(const wstring& id)
{
	m_Id = id;
}

void ContainerData::setName(const wstring& name)
{
	m_Name = name;
}

void ContainerData::setRealName(const wstring& realName)
{
	m_RealName = realName;
}

void ContainerData::setContainerType(const ContainerType& type)
{
	m_ContainerType = type;
}

void ContainerData::setMaxHeartbeatTimeout(const int maxHeartbeatTimeout) 
{
	m_maxHeartbeatTimeout = maxHeartbeatTimeout;
}

///////////////////////////////////////////////////////////
///					CONTAINER::SYSTEM
///////////////////////////////////////////////////////////
ContainerData::SystemData::SystemData()
	:m_Ip()
	,m_IsServer(0)
	,m_IsVM(0)
	,m_NumberOfProcessors(0)
	,m_TotalRam(0)
	,m_Architecture(0)
{
}

ContainerData::SystemData::~SystemData()
{
}

// GETTER
bool ContainerData::SystemData::isServer() const
{
	return m_IsServer;
}

bool ContainerData::SystemData::isVM() const
{
	return m_IsVM;
}

const wstring& ContainerData::SystemData::getIp() const 
{
	return m_Ip;
}

int ContainerData::SystemData::getArchitecture() const
{
	return m_Architecture;
}

int ContainerData::SystemData::getNumberOfProcessors() const
{
	return m_NumberOfProcessors;
}

long long ContainerData::SystemData::getTotalRam() const
{
	return m_TotalRam;
}

// SETTER
void  ContainerData::setParentId(const wstring& parentId)
{
	m_ParentId = parentId;
}

void ContainerData::SystemData::setVM(const bool value) 
{
	m_IsVM = value;
}

void ContainerData::SystemData::setIp(const wstring& ip) 
{
	m_Ip = ip;
}

void ContainerData::SystemData::setServer(const bool value) 
{
	m_IsServer = value;
}

void ContainerData::SystemData::setNumberOfProcessors(const int processors) 
{
	m_NumberOfProcessors = processors;
}

void ContainerData::SystemData::setTotalRam(const long long totalRam) 
{
	m_TotalRam = totalRam;
}

void ContainerData::SystemData::setArchitecture(const int architecture) 
{
	m_Architecture = architecture;
}

void ContainerData::setCustomerId(const wstring& customerId)
{
	m_CustomerId = customerId;
}


///////////////////////////////////////////////////////////
///					CONTAINER::ADDITIONAL
///////////////////////////////////////////////////////////
ContainerData::AdditionalData::AdditionalData()
	:m_Name(L"")
	,m_Version(L"")
	,m_ServicePack(L"")
	,m_lastStart(0)
{
}

ContainerData::AdditionalData::~AdditionalData()
{
}

// SETTERS
void ContainerData::AdditionalData::setName(const wstring& name) 
{
	m_Name = name;
}

void ContainerData::AdditionalData::setVersion(const wstring& version)  
{
	m_Version = version;
}

void ContainerData::AdditionalData::setLastStart(const long long lastStart)
{
	m_lastStart = lastStart;
}

void ContainerData::AdditionalData::setServicePack(const wstring& servicePack) 
{
	m_ServicePack = servicePack;
}

// GETTERS
const wstring& ContainerData::AdditionalData::getVersion() const 
{
	return m_Version;
}

const wstring& ContainerData::AdditionalData::getName() const
{
	return m_Name;
}

const wstring& ContainerData::AdditionalData::getServicePack() const
{
	return m_ServicePack;
}

long long ContainerData::AdditionalData::getLastStart() const 
{
	return m_lastStart;
}
