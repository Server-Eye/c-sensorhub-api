#pragma once

#include "Customer.h"

namespace ServerEye
{
	class CustomerData : public Customer
	{
	public:
		CustomerData(const wstring& company, const wstring& zipcode, const wstring& city, const wstring& country);
		CustomerData();
		~CustomerData();

		wstring getAsJSON() const override;

		const wstring& getId() const override;
		int getCustomerNr() const override;
		const wstring& getCompany() const override;
		const wstring& getEmail() const override;
		const wstring& getName() const override;
		const wstring& getSurname() const override;
		const wstring& getStreet() const override;
		const wstring& getPhone() const override;
		const wstring& getCellphone() const override;
		const wstring& getSetupSource() const override;
		const wstring& getZipCode() const override;
		const wstring& getCity() const override;
		const wstring& getCountry() const override;
		int getOceNumber() const override;
		const bool getDeactivated() const override;
		const wstring& getStreetNumber() const override;

		void setId(const wstring& id);
		void setCustomerNr(int customerNumber);
		void setCompany(const wstring& company);
		void setEmail(const wstring& email);
		void setName(const wstring&  name);
		void setSurname(const wstring& surname);
		void setStreet(const wstring& street);
		void setPhone(const wstring& phone);
		void setCellphone(const wstring& cellphone);
		void setSetupSource(const wstring& setupSource);
		void setZipCode(const wstring& zipCode);
		void setCity(const wstring& city);
		void setCountry(const wstring& country);
		void setOceNumber(int oceNumber);
		void setDeactivated(bool value);
		void setStreetNumber(const wstring& streetNumber);

	private:
		wstring m_Id;
		int m_CustomerNumber;
		wstring m_Company;
		wstring m_Email;
		wstring m_Name;
		wstring m_Surname;
		wstring m_Street;
		wstring m_Phone;
		wstring m_Cellphone;
		wstring m_setupSource;
		wstring m_ZipCode;
		wstring m_City;
		wstring m_Country;
		int m_OceNumber;
		bool m_Deactivated;
		wstring m_StreetNumber;
	};

	typedef shared_ptr<CustomerData> CustomerDataPtr;

}
