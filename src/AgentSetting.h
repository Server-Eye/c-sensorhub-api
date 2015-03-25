#pragma once

#include "SEAPIIncludes.h"

namespace ServerEye
{
	class AgentSetting 
	{
	public:
		virtual ~AgentSetting() {}

		typedef shared_ptr<AgentSetting> AgentSettingPtr;

		static AgentSettingPtr createSetting(const wstring& key);	// Returns a new settings object

		virtual const wstring& getKey() const = 0;

		virtual void setValue(const wstring& value) = 0;			// Value of this setting
		virtual const wstring& getValue() const = 0;

		virtual void setOrderId(const int orderId) = 0;				// Settings will be displayed using this orderInt.
		virtual int getOrderInt() const = 0;

		virtual void setEditable(const bool value) = 0;				// Are users allowed to change this value
		virtual bool getEditable() const = 0;

		virtual void setDefinition(const wstring& definition) = 0; // The value editor definition. Example: { 'type' : 'string', 'allowBlank' : false }
		virtual const wstring& getDefinition() const = 0;
	};
}
