#include "defaults.h"
#include "AgentResultData.h"

using namespace ServerEye;

// CTOR & DTRO
AgentResultData::AgentResultData()
	: m_Success(false)
{
}

AgentResultData::~AgentResultData()
{
}

// GETTER
bool AgentResultData::getSuccess()
{
	return m_Success;
}

wstring AgentResultData::getMessage() 
{
	return m_Message;
}

const AgentPtr& AgentResultData::getData() const
{
	return m_AgentPtr;
}

// SETTER
void AgentResultData::setSuccess(bool value)
{
	m_Success = value;
}

void AgentResultData::setMessage(const wstring& message)
{
	m_Message = message;
}

void AgentResultData::setData(const AgentPtr& ptr)
{
	m_AgentPtr = ptr;
}