#pragma once

#include "SEAPIIncludes.h"

namespace ServerEye
{

	class Customer : public SEObject
	{
	public:
		virtual const wstring& getId() const = 0;
		virtual int getCustomerNr() const = 0;
		virtual const wstring& getCompany() const = 0;
		virtual const wstring& getEmail() const = 0;
		virtual const wstring& getName() const = 0;
		virtual const wstring& getSurname() const = 0;
		virtual const wstring& getStreet() const = 0;
		virtual const wstring& getPhone() const = 0;
		virtual const wstring& getCellphone() const = 0;
		virtual const wstring& getSetupSource() const = 0;
		virtual const wstring& getZipCode() const = 0;
		virtual const wstring& getCity() const = 0;
		virtual const wstring& getCountry() const = 0;
		virtual int getOceNumber() const = 0;
		virtual const bool getDeactivated() const = 0;
		virtual const wstring& getStreetNumber() const = 0;
	};

	typedef shared_ptr<Customer> CustomerPtr;

	class CreateCustomerResult
	{
	public:
		virtual ~CreateCustomerResult() {}
		virtual bool isCreated() = 0;
		virtual wstring getMessage() = 0;
		virtual CustomerPtr getCustomer() = 0;
	};

	typedef shared_ptr<CreateCustomerResult> CreateCustomerResultPtr;

}