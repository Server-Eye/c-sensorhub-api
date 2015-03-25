#include "defaults.h"
#include "CustomerData.h"

using namespace ServerEye;

// CTOR & DTOR
CustomerData::CustomerData(const wstring& company, const wstring& zipcode, const wstring& city, const wstring& country)
						   : m_Company(company)
						   , m_ZipCode(zipcode)
						   , m_City(city)
						   , m_Country(country)
{
}

CustomerData::CustomerData()
{
}

CustomerData::~CustomerData()
{
}

// SE OBJECT
wstring CustomerData::getAsJSON() const
{
	return L"";
}

// GETTER
const wstring& CustomerData::getId() const 
{
	return m_Id;
}

int CustomerData::getCustomerNr() const
{
	return m_CustomerNumber;
}

const wstring& CustomerData::getCompany() const 
{
	return m_Company;
}

const wstring& CustomerData::getEmail() const 
{
	return m_Email;
}

const wstring& CustomerData::getName() const 
{
	return m_Name;
}

const wstring& CustomerData::getSurname() const 
{
	return m_Surname;
}

const wstring& CustomerData::getStreet() const 
{
	return m_Street;
}

const wstring& CustomerData::getPhone() const 
{
	return m_Phone;
}

const wstring& CustomerData::getCellphone() const 
{
	return m_Cellphone;
}

const wstring& CustomerData::getSetupSource() const 
{
	return m_setupSource;
}

const wstring& CustomerData::getZipCode() const 
{
	return m_ZipCode;
}

const wstring& CustomerData::getCity() const 
{
	return m_City;
}

const wstring& CustomerData::getCountry() const 
{
	return m_Country;
}

int CustomerData::getOceNumber() const 
{
	return m_OceNumber;
}

const bool CustomerData::getDeactivated() const 
{
	return m_Deactivated;
}

const wstring& CustomerData::getStreetNumber() const 
{
	return m_StreetNumber;
}


// SETTER
void CustomerData::setId(const wstring& id)
{
	m_Id = id;
}
void CustomerData::setCustomerNr(int customerNumber)
{
	m_CustomerNumber = customerNumber;
}

void CustomerData::setCompany(const wstring& company)
{
	m_Company = company;
}

void CustomerData::setEmail(const wstring& email)
{
	m_Email = email;
}

void CustomerData::setName(const wstring&  name)
{
	m_Name = name;
}

void CustomerData::setSurname(const wstring& surname)
{
	m_Surname = surname;
}

void CustomerData::setStreet(const wstring& street)
{
	m_Street = street;
}

void CustomerData::setPhone(const wstring& phone)
{
	m_Phone = phone;
}

void CustomerData::setCellphone(const wstring& cellphone)
{
	m_Cellphone = cellphone;
}

void CustomerData::setSetupSource(const wstring& setupSource)
{
	m_setupSource = setupSource;
}

void CustomerData::setZipCode(const wstring& zipCode)
{
	m_ZipCode = zipCode;
}

void CustomerData::setCity(const wstring& city)
{
	m_City = city;
}

void CustomerData::setCountry(const wstring& country)
{
	m_Country = country;
}

void CustomerData::setOceNumber(int oceNumber)
{
	m_OceNumber = oceNumber;
}

void CustomerData::setDeactivated(bool value)
{
	m_Deactivated = value;
}
void CustomerData::setStreetNumber(const wstring& streetNumber)
{
	m_StreetNumber = streetNumber;
}
