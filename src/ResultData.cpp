#include "defaults.h"
#include "ResultData.h"

using namespace ServerEye;

ResultData::ResultData()
	: m_Success(false)
{
}

ResultData::~ResultData()
{
}

bool ResultData::getSuccess() 
{
	return m_Success;
}

wstring ResultData::getMessage() 
{
	return m_Message;
}

void ResultData::setSuccess(const bool value)
{
	m_Success = value;
}

void ResultData::setMessage(const wstring& message)
{
	m_Message = message;
}
