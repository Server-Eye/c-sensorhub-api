#pragma once

#include "SEAPIIncludes.h"

namespace ServerEye
{
	class Container : public SEObject
	{
	public:

		enum ContainerType
		{
			OCC_CONNECTOR = 0,
			SENSORHUB = 2,
			EXTERNAL = 4
		};

		static ContainerPtr createContainer(const wstring& customerId, const wstring& name, const wstring& realName, const ContainerType type);

		virtual const wstring& getId() const = 0;					// returns the UUID of this container
		virtual const wstring& getRealName() const = 0;				// returns the write-only name of this container
		virtual const wstring& getCustomerId() const = 0;			// returns the customer id 
		virtual const wstring& getParentId() const = 0;				// returns the parent id of this container (nessesary for type SENSORHUB)
		virtual const wstring& getName() const = 0;					// returns the custom name of this container
		virtual int getMaxHeartbeatTimeout() const = 0;				// returns the maxheartbeat timeout 
		virtual ContainerType getType() const = 0;					// returns OCC_CONNECTOR, SENSORHUB or EXTERNAL

		virtual void setName(const wstring& name) = 0;				// sets a custom name for this container
		virtual void setMaxHeartbeatTimeout(const int maxHeartbeatTimeout) = 0;		// default is 20

		class System 
		{
		public:
			virtual const wstring& getIp() const = 0;
			virtual void setIp(const wstring& ip) = 0;

			virtual bool isServer() const = 0;
			virtual void setServer(const bool value) = 0;

			virtual bool isVM() const = 0;
			virtual void setVM(const bool value) = 0;

			virtual int getNumberOfProcessors() const = 0;
			virtual void setNumberOfProcessors(const int processors) = 0;

			virtual long long getTotalRam() const = 0;
			virtual void setTotalRam(const long long totalRam) = 0;

			virtual int getArchitecture() const = 0;
			virtual void setArchitecture(const int architecture) = 0;
		};

		virtual System& getSystem() = 0;
		virtual const System& getSystem() const = 0;

		class Additional 
		{
		public:
			virtual const wstring& getName() const = 0;
			virtual void setName(const wstring& name) = 0;

			virtual const wstring& getVersion() const = 0;
			virtual void setVersion(const wstring& version) = 0;

			virtual const wstring& getServicePack() const = 0;
			virtual void setServicePack(const wstring& servicePack) = 0;

			virtual long long getLastStart() const = 0;
			virtual void setLastStart(const long long lastStart) = 0;
		};

		virtual Additional& getAdditional() = 0;
		virtual const Additional& getAdditional() const = 0;
	};
}
