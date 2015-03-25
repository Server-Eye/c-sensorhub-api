#pragma once

#include <stdio.h>
#include "StringConverter.h"
#include <curl/curl.h>


namespace ServerEye
{
	class ConnectionManager
	{
	public:
		ConnectionManager();
		~ConnectionManager();

		class CurlHandle
		{
		public:
			CurlHandle();
			~CurlHandle();

			static curl_socket_t opensocketCallBack(void *clientp, curlsocktype purpose, struct curl_sockaddr *address);
			static int closesocketCallBack(void *clientp, curl_socket_t sock);

			void Init();
			void Close();

			CURL* m_CurlObj;
			curl_socket_t m_Socket;
		};

		// Formatting URL 
		static wstring urlParamEncode(const wstring& param);

		// Proxy stuff
		static void setProxy(const wstring& ip, const wstring& port, const wstring& user, const wstring& password);
		static void configureProxy(CurlHandle& curlHandle);
		static void InitHandle(CurlHandle& curlHandle);

		// HTTP Posts, Requests
		static int httpPost(CurlHandle& curl, const wstring& url, const wstring& body, wstring& OutMsg);
		static int httpPost(const wstring& url, const wstring& body, wstring& OutMsg)
		{
			CurlHandle curl;
			return httpPost(curl, url, body, OutMsg);
		}
		static wstring httpPostStr(CurlHandle& curl, const wstring& url, const wstring& body){
			wstring OutMsg;
			httpPost(curl, url, body, OutMsg);
			return OutMsg;
		}
		static wstring httpPostStr(const wstring& url, const wstring& body){
			wstring OutMsg;
			httpPost(url, body, OutMsg);
			return OutMsg;
		}
		static int httpPut(CurlHandle& curl, const wstring& url, const wstring& body, wstring& OutMsg);
		static int httpPut(const wstring& url, const wstring& body, wstring& OutMsg)
		{
			CurlHandle curl;
			return httpPut(curl, url, body, OutMsg);
		}
		static wstring httpPutStr(CurlHandle& curl, const wstring& url, const wstring& body){
			wstring OutMsg;
			httpPut(curl, url, body, OutMsg);
			return OutMsg;
		}
		static wstring httpPutStr(const wstring& url, const wstring& body){
			wstring OutMsg;
			httpPut(url, body, OutMsg);
			return OutMsg;
		}
		static int httpGetFile(CurlHandle& curl, const wstring& url, const wstring& fileName);
		static int httpGetFile(const wstring& url, const wstring& fileName)
		{
			CurlHandle curl;
			return httpGetFile(curl, url, fileName);
		}
		static int httpGet(CurlHandle& curl, const wstring& url, wstring& OutMsg);
		static int httpGet(const wstring& url, wstring& OutMsg)
		{
			CurlHandle curl;
			return httpGet(curl, url, OutMsg);
		}
		static wstring httpGetStr(CurlHandle& curl, const wstring& url){
			wstring OutMsg;
			httpGet(curl, url, OutMsg);
			return OutMsg;
		}
		static wstring httpGetStr(const wstring& url){
			wstring OutMsg;
			httpGet(url, OutMsg);
			return OutMsg;
		}

	private:
		static wstring proxyIp;
		static wstring proxyPort;
		static wstring proxyUser;
		static wstring proxyPW;
	};
}
