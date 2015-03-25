#include "defaults.h"
#include "PushResultData.h"

using namespace ServerEye;

// CTOR & DTOR
PushResultData::PushResultData()
	: m_Success(false)
{
}

PushResultData::~PushResultData()
{
}

// GETTER
const wstring& PushResultData::getRootContainerId() const 
{
	return m_RootContainerId;
}

const wstring& PushResultData::getContainerId() const 
{
	return m_ContainerId;
}

const wstring& PushResultData::getSenderId() const 
{
	return m_SenderId;
}

const wstring& PushResultData::getMessageId() const 
{
	return m_MessageId;
}

const wstring& PushResultData::getControl() const 
{
	return m_Control;
}

const wstring& PushResultData::getData() const 
{
	return m_Data;
}

bool PushResultData::getSuccess() 
{
	return m_Success;	
}

wstring PushResultData::getMessage() 
{
	return m_Message;
}

// SETTER
void PushResultData::setRootContainerId(const wstring& rootContainerId)
{
	m_RootContainerId = rootContainerId;
}

void PushResultData::setContainerId(const wstring& containerId)
{
	m_ContainerId = containerId;
}

void PushResultData::setSenderId(const wstring& senderId)
{
	m_SenderId = senderId;
}

void PushResultData::setMessageId(const wstring& messageId)
{
	m_MessageId = messageId;
}

void PushResultData::setControl(const wstring& control)
{
	m_Control = control;
}

void PushResultData::setData(const wstring& data)
{
	m_Data = data;
}

void PushResultData::setMessage(const wstring& message)
{
	m_Message = message;
} 

void PushResultData::setSuccess(bool value)
{
	m_Success = value;
}

