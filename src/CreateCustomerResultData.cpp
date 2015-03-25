#include "defaults.h"
#include "CreateCustomerResultData.h"

using namespace ServerEye;

// CTOR & DTOR
CreateCustomerResultData::CreateCustomerResultData(bool isCreated, const wstring& message, CustomerPtr& customer)
	: m_IsCreated(isCreated)
	, m_Message(message)
	, m_Customer(customer)
{
}

CreateCustomerResultData::CreateCustomerResultData()
	: m_IsCreated(false)
	, m_Message(wstring())
	, m_Customer(nullptr)
{
}

CreateCustomerResultData::~CreateCustomerResultData()
{
}

// GETTER
bool CreateCustomerResultData::isCreated() 
{
	return m_IsCreated;
}

wstring CreateCustomerResultData::getMessage() 
{
	return m_Message;
}

CustomerPtr CreateCustomerResultData::getCustomer() 
{
	return m_Customer;
}

// SETTTER
void CreateCustomerResultData::setIsCreated(bool value)
{
	m_IsCreated = value;
}

void CreateCustomerResultData::setMessage(const wstring& message)
{
	m_Message = message;
}

void CreateCustomerResultData::setCustomer(CustomerPtr& customer)
{
	m_Customer = customer;
}
