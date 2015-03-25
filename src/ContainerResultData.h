#pragma once

#include "Results.h"

namespace ServerEye
{
	class ContainerResultData : public ContainerResult
	{
	public:
		ContainerResultData();
		~ContainerResultData();

		bool getSuccess() override;
		wstring getMessage() override;
		const ContainerPtr& getData() const override;

		void setSuccess(bool value);
		void setMessage(const wstring& message);
		void setData(const ContainerPtr& ptr);

	private:
		bool m_Success;
		wstring m_Message;
		ContainerPtr m_containerPtr;
	};

	typedef shared_ptr<ContainerResultData> ContainerResultDataPtr;

}
