#pragma once

#include "Agent.h"

namespace ServerEye
{
	class AgentData : public Agent
	{
	public:
		AgentData(const wstring& parentId, const wstring& name, const wstring& type);
		AgentData();
		~AgentData();

		void setId(const wstring& id);
		void setParentId(const wstring& parentId);
		void setType(const wstring& type);

		const wstring& getParentId() const override;
		const wstring& getName() const override;
		const wstring& getType() const override;
		const wstring& getId() const override;

		void setName(const wstring& name) override;

		// Settings
		const SettingMap& getAllSettings() const override;
		bool hasSetting(const wstring& key) const override;
		bool removeSetting(const wstring& key) override;

		// SE OBJECT
		wstring getAsJSON() const override;

		class AgentSettingData : public AgentSetting
		{
		public:
			AgentSettingData(const wstring& key);
			AgentSettingData();
			~AgentSettingData();

			void setKey(const wstring& key);
			const wstring& getKey() const override;

			void setValue(const wstring& value) override;
			const wstring& getValue() const override;

			void setOrderId(const int orderId) override;
			int getOrderInt() const override;

			void setEditable(const bool value) override;
			bool getEditable() const override;

			void setDefinition(const wstring& definition) override; 
			const wstring& getDefinition() const override;

		private:
			wstring m_Key;
			wstring m_Value;
			int m_OrderId;
			bool m_IsEditable;
			wstring m_Definition;
		};

		AgentSettingPtr getSetting(const wstring& key) const override;
		void setSetting(const AgentSettingPtr& setting) override;

	private:
		wstring m_Id;
		wstring m_ParentId;
		wstring m_Name;	
		wstring m_AgentType;
		SettingMap m_settings;
	};

	typedef shared_ptr<AgentData> AgentDataPtr;

}
