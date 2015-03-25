#pragma once

#include "SEAPIIncludes.h"

namespace ServerEye
{
	class PushCallBack
	{
	public:
		virtual void OnConnect() {};	// Called if the connection is established
		virtual void OnDisconnect() {};	// Called after disconnect or connection was lost
		virtual void OnError() {};		// Called if the connection failed or wrong data was received

		virtual void OnAddOrUpdateAgent(const wstring& containerId, const wstring& senderId, const wstring& control, const wstring& messageId) {};
		virtual void OnDeleteAgent(const wstring& containerId, const wstring& senderId, const wstring& control, const wstring& messageId) {};
		virtual void OnRefreshAgent(const wstring& containerId, const wstring& senderId, const wstring& control, const wstring& messageId) {};
		virtual void OnRemoteInstall(const wstring& user, const wstring& password, const wstring& domain, const wstring& containerId, const wstring& control, const wstring& messageId, bool occConnectorCheck, const vector<wstring>& urls) {};
	};
}
