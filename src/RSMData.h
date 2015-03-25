#pragma once

#include "RSM.h"

namespace ServerEye
{
	class RSMData : public RSM
	{
	public:
		RSMData(const wstring& key, const int version, const State state, const wstring& data);
		RSMData();
		~RSMData();

		const wstring& getKey() const override;
		const int getVersion() const override;
		const wstring& getMessage() const override;
		const State& getState() const override;
		const wstring& getData() const override;

		void setMessage(const wstring& message) override;

		void setKey(const wstring& key);
		void setVersion(const int version);
		void setState(const State& state);
		void setData(const wstring& data);

	private:
		wstring m_Key;
		int m_Version;
		wstring m_Message;
		State m_State;
		wstring m_Data;
	};
}
