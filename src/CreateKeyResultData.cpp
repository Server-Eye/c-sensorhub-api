#include "defaults.h"
#include "CreateKeyResultData.h"

using namespace ServerEye;

// CTOR & DTOR
CreateKeyResultData::CreateKeyResultData()
	: m_Success(false)
{
}

CreateKeyResultData::~CreateKeyResultData()
{
}

// GETTER
bool CreateKeyResultData::getSuccess() 
{
	return m_Success;
}

wstring CreateKeyResultData::getMessage() 
{
	return m_Message;
}

const wstring& CreateKeyResultData::getAPIKey() const 
{
	return m_ApiKey;
}

const wstring& CreateKeyResultData::getCustomerId() const 
{
	return m_CustomerId;
}

const wstring& CreateKeyResultData::getUserId() const 
{
	return m_UserId;
}

// SETTER
void CreateKeyResultData::setSuccess(bool value)
{
	m_Success = value;
}

void CreateKeyResultData::setMessage(const wstring& message)
{
	m_Message = message;
}

void CreateKeyResultData::setAPIKey(const wstring& apiKey)
{
	m_ApiKey = apiKey;
}

void CreateKeyResultData::setCustomerId(const wstring& customerId)
{
	m_CustomerId = customerId;
}

void CreateKeyResultData::setUserId(const wstring& userId)
{
	m_UserId = userId;
}
