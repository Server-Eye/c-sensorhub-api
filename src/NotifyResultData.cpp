#include "defaults.h"
#include "NotifyResultData.h"

using namespace ServerEye;

// CTOR & DTOR
NotifyResultData::NotifyResultData()
	: m_Success(false)
{
}

NotifyResultData::~NotifyResultData()
{
}

// GETTER
const wstring& NotifyResultData::getData() const 
{
	return m_Data;
}

bool NotifyResultData::getSuccess() 
{
	return m_Success;
}

wstring NotifyResultData::getMessage() 
{
	return m_Message;
}

// SETTER
void NotifyResultData::setData(const wstring& data)
{
	m_Data = data;
}

void NotifyResultData::setSuccess(const bool value)
{
	m_Success = value;
}

void NotifyResultData::setMessage(const wstring& message)
{
	m_Message = message;
}
