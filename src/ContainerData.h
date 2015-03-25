#pragma once

#include "SEAPIIncludes.h"
#include "Container.h"

namespace ServerEye
{
	class ContainerData : public Container
	{
	public:
		ContainerData(wstring customerId, wstring name, wstring realName, ContainerType type);
		ContainerData();
		~ContainerData();

		void setId(const wstring& id);
		void setName(const wstring& name);
		void setRealName(const wstring& realName);
		void setContainerType(const ContainerType& type);
		void setCustomerId(const wstring& customerId);
		void setParentId(const wstring& parentId);

		const wstring& getId() const override;			
		const wstring& getRealName() const override;		
		const wstring& getCustomerId() const override;	
		const wstring& getParentId() const override;
		const wstring& getName() const override;
		ContainerType getType() const override;
		int getMaxHeartbeatTimeout() const override;

		void setMaxHeartbeatTimeout(const int maxHeartbeatTimeout) override;

		System& getSystem() override { return m_SystemData; }
		const System& getSystem() const override { return m_SystemData; }

		Additional& getAdditional() override { return m_AdditionalData; }
		const Additional& getAdditional() const override { return m_AdditionalData; }

		// SE OBJECT
		wstring getAsJSON() const override;

		// SYSTEM
		class SystemData : public System
		{
		public:
			SystemData();
			~SystemData();

			virtual const wstring& getIp() const override;
			virtual void setIp(const wstring& ip) override;

			virtual bool isServer() const override;
			virtual void setServer(const bool value) override;

			virtual bool isVM() const override;
			virtual void setVM(const bool value) override;

			virtual int getNumberOfProcessors() const override;
			virtual void setNumberOfProcessors(const int processors) override;

			virtual long long getTotalRam() const override;
			virtual void setTotalRam(const long long totalRam) override;

			virtual int getArchitecture() const override;
			virtual void setArchitecture(const int architecture) override;

		private:
			wstring m_Ip;
			bool m_IsServer;
			bool m_IsVM;
			int m_NumberOfProcessors;
			long long m_TotalRam;
			int m_Architecture;
		};

		class AdditionalData : public Additional
		{
		public:
			AdditionalData();
			~AdditionalData();

			const wstring& getName() const override;
			void setName(const wstring& name) override;

			const wstring& getVersion() const override;
			void setVersion(const wstring& version) override;

			const wstring& getServicePack() const override;
			void setServicePack(const wstring& servicePack) override;

			long long getLastStart() const override;
			void setLastStart(const long long lastStart);

		private:
			wstring m_Name;
			wstring m_Version;
			wstring m_ServicePack;
			long long m_lastStart;
		};

	private:
		wstring m_Id;
		wstring m_RealName;
		wstring m_CustomerId;
		wstring m_ParentId;
		wstring m_Name;
		ContainerType m_ContainerType;
		int m_maxHeartbeatTimeout;

		SystemData m_SystemData;
		AdditionalData m_AdditionalData;
	};

	typedef shared_ptr<ContainerData> ContainerDataPtr;

}
