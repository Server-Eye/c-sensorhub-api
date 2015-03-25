#pragma once

#include "SEAPIIncludes.h"
#include "AgentSetting.h"

namespace ServerEye
{
	class Agent : public SEObject
	{
	public:

		typedef std::map<wstring, AgentSettingPtr> SettingMap;

		const static wstring TYPE_EXTERNAL_HARDWARE;	// "EXTERNAL_HARDWARE"
		const static wstring TYPE_EXTERNAL_SOFTWARE;	// "EXTERNAL_SOFTWARE"
		const static wstring TYPE_EXTERNAL_NETWORK;		// "EXTERNAL_NETWORK"

		static AgentPtr createAgent(const wstring& parentId, const wstring& name, 
			const wstring& type);

		virtual const wstring& getParentId() const = 0;
		virtual const wstring& getId() const = 0;
		virtual const wstring& getName() const = 0;
		virtual const wstring& getType() const = 0;

		virtual void setName(const wstring& name) = 0;

		virtual const SettingMap& getAllSettings() const = 0;
		virtual bool hasSetting(const wstring& key) const = 0;
		virtual AgentSettingPtr getSetting(const wstring& key) const = 0;
		virtual void setSetting(const AgentSettingPtr& setting) = 0;
		virtual bool removeSetting(const wstring& key) = 0;
	};
}
