#include "defaults.h"
#include "RSMData.h"

using namespace ServerEye;

// STATIC
const RSMPtr RSM::createRSM(const wstring& key, const int version, const State state, const wstring& data)
{
	return RSMPtr( new RSMData(key, version, state, data) );
}

// CTOR & DTOR
RSMData::RSMData(const wstring& key, const int version, const State state, const wstring& data)
	: m_Key(key)
	, m_Version(version)
	, m_State(state)
	, m_Data(data)
{
}

RSMData::RSMData()
{
}

RSMData::~RSMData()
{
}

// GETTER
const wstring& RSMData::getKey() const 
{
	return m_Key;
}

const int RSMData::getVersion() const 
{
	return m_Version;
}

const wstring& RSMData::getMessage() const 
{
	return m_Message;
}

const RSMData::State& RSMData::getState() const 
{
	return m_State;
}

const wstring& RSMData::getData() const 
{
	return m_Data;
}

// SETTER
void RSMData::setMessage(const wstring& message) 
{
	m_Message = message;
}

void RSMData::setKey(const wstring& key)
{
	m_Key = key;
}

void RSMData::setVersion(const int version)
{
	m_Version = version;
}

void RSMData::setState(const State& state)
{
	m_State = state;
}

void RSMData::setData(const wstring& data)
{
	m_Data = data;
}
