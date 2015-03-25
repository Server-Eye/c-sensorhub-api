#include "defaults.h"
#include "ContainerResultData.h"

using namespace ServerEye;

ContainerResultData::ContainerResultData()
	: m_Success(false)
{
}

ContainerResultData::~ContainerResultData()
{
}

bool ContainerResultData::getSuccess() 
{
	return m_Success;
}

wstring ContainerResultData::getMessage() 
{
	return m_Message;
}

const ContainerPtr& ContainerResultData::getData() const
{
	return m_containerPtr;
}

void ContainerResultData::setSuccess(bool value)
{
	m_Success = value;
}

void ContainerResultData::setMessage(const wstring& message)
{
	m_Message = message;
}

void ContainerResultData::setData(const ContainerPtr& ptr)
{
	m_containerPtr = ptr;
}