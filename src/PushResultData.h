#pragma once

#include "Results.h"

namespace ServerEye
{
	class PushResultData : public PushResult
	{
	public:
		PushResultData();
		~PushResultData();

		bool getSuccess() override;
		wstring getMessage() override;
		const wstring& getRootContainerId() const override;
		const wstring& getContainerId() const override;
		const wstring& getSenderId() const override;
		const wstring& getMessageId() const override;
		const wstring& getControl() const override;
		const wstring& getData() const override;

		void setSuccess(bool value);
		void setMessage(const wstring& message); 
		void setRootContainerId(const wstring& rootContainerId);
		void setContainerId(const wstring& containerId);
		void setSenderId(const wstring& senderId);
		void setMessageId(const wstring& messageId);
		void setControl(const wstring& control);
		void setData(const wstring& data);

	private:
		bool m_Success;
		wstring m_Message;
		wstring m_RootContainerId;
		wstring m_ContainerId;
		wstring m_SenderId;
		wstring m_MessageId;
		wstring m_Control;
		wstring m_Data;
	};

	typedef shared_ptr<PushResultData> PushResultDataPtr;

}
