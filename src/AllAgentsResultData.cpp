#include "defaults.h"
#include "AllAgentsResultData.h"

using namespace ServerEye;

// CTOR & DTOR
AllAgentsResultData::AllAgentsResultData()
	: m_Success(false)
{
}


AllAgentsResultData::~AllAgentsResultData()
{
}

// GETTER
bool AllAgentsResultData::getSuccess() 
{
	return m_Success;
}

wstring AllAgentsResultData::getMessage() 
{
	return m_Message;
}

const AgentList& AllAgentsResultData::getData() const
{
	return m_AgentList;
}

// SETTER
void AllAgentsResultData::setSuccess(bool value)
{
	m_Success = value;
}

void AllAgentsResultData::setMessage(const wstring& message)
{
	m_Message = message;
}

void AllAgentsResultData::setData(const AgentList& list)
{
	m_AgentList = list;
}
