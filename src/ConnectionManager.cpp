#include "defaults.h"
#include "ConnectionManager.h"
#include "Log.h"

#ifdef _WIN32
	#define sclose(x)  closesocket((x))
#else
	#define sclose(x)  close((x))
#endif

using std::string;
using namespace ServerEye;

bool printCommDebug = true;

#ifdef _WIN32
class WSAInit
{
public:
	WSAInit()
	{
		// INIT WINSOCK
		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
	}
	~WSAInit()
	{
		WSACleanup();
	}
} _WSAInit;
#endif

size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) 
{
	((string*)stream)->append((char*)ptr, 0, size*count);
	return size*count;
}

///////////////////////////////////////////////////////////////////////////////////////
///									ConnectionManager
///////////////////////////////////////////////////////////////////////////////////////

#define CONNECTION_MANAGER_TIMEOUT 60L

// CTOR & DTOR
ConnectionManager::ConnectionManager()
{
}

ConnectionManager::~ConnectionManager()
{
}

// STATIC
wstring ConnectionManager::proxyIp = L"";
wstring ConnectionManager::proxyPort = L"";
wstring ConnectionManager::proxyUser = L"";
wstring ConnectionManager::proxyPW = L"";

///////////////////////////////////////////////////////////////////////////////////////
///								ConnectionManager HELPERS 
///////////////////////////////////////////////////////////////////////////////////////
wstring ConnectionManager::urlParamEncode(const wstring& param)
{
	string sparam=StringConverter::ws2s_UTF8(param);
	string nparam;
	for (size_t i=0;i<sparam.size(); ++i)
	{
		char c=sparam[i];
		if (isalnum(c) || c=='-' || c=='_' || c=='.' || c=='~')
			nparam+=c;
		else
		{
			nparam+='%';
			int n=(c>>4)&0xf;
			if (n>9)
				nparam+='a'+n-16;
			else
				nparam+='0'+n;
			n=c&0xf;
			if (n>9)
				nparam+='a'+n-16;
			else
				nparam+='0'+n;
		}
	}
	return StringConverter::s2ws_UTF8(nparam);
}

void ConnectionManager::InitHandle(CurlHandle& curlHandle)
{
	curlHandle.Init();
	configureProxy(curlHandle);
}

void ConnectionManager::setProxy(const wstring& ip, const wstring& port, const wstring& user, const wstring& password)
{
	proxyIp = ip;
	proxyPort = port;
	proxyUser = user;
	proxyPW = password;

	if (!proxyIp.size() && !proxyPort.size() && !proxyUser.size() && !proxyPW.size())
		Log::Info(L"Proxy configuration is cleared");
	else if (proxyIp.size() && proxyPort.size() && proxyUser.size() && proxyPW.size())
		Log::Info(L"Proxy configuration set to " + ip + L":" + proxyPort + L" with authentification");
	else
		Log::Info(L"Proxy configuration set to " + ip + L":" + proxyPort);
}

void ConnectionManager::configureProxy(CurlHandle& curlHandle)
{
	// Standard curl init
	curlHandle.Init();

	// convert to string weepy curl
	if(proxyIp.size() && proxyPort.size())
	{
		// Convert to normal string for whiny curl
		string pIp = StringConverter::ws2s_UTF8(proxyIp);
		int pPort = StringConverter::string2Int(proxyPort);

		// Configrue proxy ip an port
		curl_easy_setopt(curlHandle.m_CurlObj, CURLOPT_PROXYAUTH, CURLAUTH_NONE);
		curl_easy_setopt(curlHandle.m_CurlObj, CURLOPT_PROXY, pIp.c_str());
		curl_easy_setopt(curlHandle.m_CurlObj, CURLOPT_PROXYPORT, pPort);
	}

	// Configure proxy authentification
	if(proxyUser.size() && proxyPW.size())
	{
		string login = StringConverter::ws2s_UTF8(proxyUser) + ":" + StringConverter::ws2s_UTF8(proxyPW);
		curl_easy_setopt(curlHandle.m_CurlObj, CURLOPT_PROXYAUTH, CURLAUTH_BASIC);
		curl_easy_setopt(curlHandle.m_CurlObj, CURLOPT_PROXYUSERPWD, login.c_str());  	
	}
}

///////////////////////////////////////////////////////////////////////////////////////
///							ConnectionManager HTTP REQUESTS
///////////////////////////////////////////////////////////////////////////////////////
int ConnectionManager::httpPost(CurlHandle& curlHandle, const wstring& url, const wstring& body, wstring& OutMsg)
{
	if (printCommDebug)
	{
		if (body.size())
			Log::Debug(L"ConnectionManager (httpPost) " + url + L" BODY: " + body);
		else
			Log::Debug(L"ConnectionManager (httpPost) " + url);
	}

	InitHandle(curlHandle);
	string response;
	auto curl = curlHandle.m_CurlObj;
	CURLcode res = CURLcode::CURLE_FAILED_INIT;

	if(curl)
	{
		string sUrl = StringConverter::ws2s_UTF8(url);
		string sBody = StringConverter::ws2s_UTF8(body);
		size_t contentLength = sBody.size();

		// CREATE HEADER OPTIONS
		curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charsets: utf-8");
		headers = curl_slist_append(headers, (string("Content-Length: ") + boost::lexical_cast<std::string>(contentLength)).c_str());

		// SET OPTIONS
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sBody.c_str());
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, CONNECTION_MANAGER_TIMEOUT);

		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		// Get the return value
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		// SEND 
		res = curl_easy_perform(curl);

		if (res == CURLE_OK)
		{
			long httpCode = 0;
			res = curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
			if (httpCode == 404)
			{
				res = CURLE_READ_ERROR;
				Log::Error(L"ConnectionManager (httpGet/Response) not found");
			}
			else if (httpCode == 200)
			{
				// CLEAN UP
			}
			else
			{
				res = CURLE_READ_ERROR;
				Log::Error(L"ConnectionManager (httpGet/Response) " + StringConverter::decimal2String(httpCode));
			}
		}

		// CLEAN UP
		curl_slist_free_all(headers);
	}

	OutMsg = StringConverter::s2ws_UTF8(response);
	if (printCommDebug) Log::Debug(L"ConnectionManager (httpPost/Response) " + OutMsg);

	return res;
}

int ConnectionManager::httpPut(CurlHandle& curlHandle, const wstring& url, const wstring& body, wstring& OutMsg)
{
	if (printCommDebug) 
	{
		if (body.size())
			Log::Debug(L"ConnectionManager (httpPut) " + url + L" BODY: " + body);
		else
			Log::Debug(L"ConnectionManager (httpPut) " + url);
	}

	InitHandle(curlHandle);
	string response;
	auto curl = curlHandle.m_CurlObj;
	CURLcode res = CURLcode::CURLE_FAILED_INIT;

	if(curl)
	{
		string sUrl = StringConverter::ws2s_UTF8(url);
		string sBody = StringConverter::ws2s_UTF8(body);
		size_t contentLength = sBody.size();

		// CREATE HEADER OPTIONS
		curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charsets: utf-8");
		headers = curl_slist_append(headers, (string("Content-Length: ") + boost::lexical_cast<std::string>(contentLength)).c_str());

		// SET OPTIONS
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sBody.c_str());
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, CONNECTION_MANAGER_TIMEOUT);

		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		// Get the return value
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		// SEND 
		res = curl_easy_perform(curl);

		if (res == CURLE_OK)
		{
			long httpCode = 0;
			res = curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
			if (httpCode == 404)
			{
				res = CURLE_READ_ERROR;
				Log::Error(L"ConnectionManager (httpGet/Response) not found");
			}
			else if (httpCode == 200)
			{
				// CLEAN UP
			}
			else
			{
				res = CURLE_READ_ERROR;
				Log::Error(L"ConnectionManager (httpGet/Response) " + StringConverter::decimal2String(httpCode));
			}
		}

		// CLEAN UP
		curl_slist_free_all(headers);
	}

	OutMsg = StringConverter::s2ws_UTF8(response);
	
	if (printCommDebug) Log::Debug(L"ConnectionManager (httpPut/Response) " + OutMsg);

	return res;
}

int ConnectionManager::httpGet(CurlHandle& curlHandle, const wstring& url, wstring& OutMsg)
{
	if (printCommDebug) Log::Debug(L"ConnectionManager (httpGet) " + url);

	InitHandle(curlHandle);
	string response;
	auto curl = curlHandle.m_CurlObj;
	CURLcode res = CURLcode::CURLE_FAILED_INIT;

	if(curl)
	{
		string sUrl = StringConverter::ws2s_UTF8(url);
		// CREATE HEADER OPTIONS
		curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charsets: utf-8");

		// SET OPTIONS
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers); 
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, CONNECTION_MANAGER_TIMEOUT);

		// Get the return value
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

		// SEND 
		res = curl_easy_perform(curl);		

		if (res == CURLE_OK)
		{
			long httpCode = 0;
			res = curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
			if (httpCode == 404)
			{
				res = CURLE_READ_ERROR;
				Log::Error(L"ConnectionManager (httpGet/Response) not found");
			}
			else if (httpCode == 200)
			{
				// CLEAN UP
			}
			else
			{
				res = CURLE_READ_ERROR;
				Log::Error(L"ConnectionManager (httpGet/Response) " + StringConverter::decimal2String(httpCode));
			}
		}
		curl_slist_free_all(headers);
	}

	OutMsg = StringConverter::s2ws_UTF8(response);
	if (printCommDebug) Log::Debug(L"ConnectionManager (httpGet/Response) " + OutMsg);

	return res;
}

struct FileInfo
{
	wstring fileName;
	std::fstream file;
	std::fstream::pos_type size;
	FileInfo(const wstring& _fileName)
		:fileName(_fileName)
		,size(0)
	{
	}
};

size_t write_to_file(void *ptr, size_t size, size_t count, void *stream)
{
	FileInfo* pInfo = (FileInfo*)stream;
	if (!pInfo->file.is_open())
	{
		#ifdef _WIN32
			_wunlink(pInfo->fileName.c_str());
			pInfo->file.open(pInfo->fileName, std::ios::out | std::ios::app | std::ios::binary);
		#else
			string fileName = StringConverter::ws2s_UTF8(pInfo->fileName);
			unlink(fileName.c_str());			
			pInfo->file.open(fileName.c_str(), std::ios::out | std::ios::app | std::ios::binary);
		#endif
		
	}
	size_t wsize = size*count;
	pInfo->file.write((const char*)ptr, wsize);
	pInfo->size += wsize;
	return wsize;
}

int ConnectionManager::httpGetFile(CurlHandle& curlHandle, const wstring& url, const wstring& fileName)
{
	if (printCommDebug) Log::Debug(L"ConnectionManager (httpGet) " + url);

	InitHandle(curlHandle);
	string response;
	auto curl = curlHandle.m_CurlObj;
	CURLcode res = CURLcode::CURLE_FAILED_INIT;


	if (curl)
	{
		FileInfo info(fileName);
		string sUrl = StringConverter::ws2s_UTF8(url);
		// CREATE HEADER OPTIONS
		curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Accept: application/json");
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "charsets: utf-8");

		// SET OPTIONS
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);

		// Get the return value
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_file);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &info);

		// SEND 
		res = curl_easy_perform(curl);

		if (res == CURLE_OK)
		{
			long httpCode = 0;
			res = curl_easy_getinfo(curl, CURLINFO_HTTP_CODE, &httpCode);
			if (httpCode==404)
			{
				res = CURLE_READ_ERROR;
				Log::Error(L"ConnectionManager (httpGet/Response) " + fileName + L" not found");
			}
			else if (httpCode == 200)
			{
				auto pos = info.file.tellp();
				if (pos != info.size)
				{
					res = CURLE_WRITE_ERROR;
					Log::Error(L"ConnectionManager (httpGet/Response) " + fileName + L" download size mismatch");
				}
				else
					Log::Info(L"ConnectionManager (httpGet/Response) " + fileName + L" download finished");
			}
			else
			{
				res = CURLE_READ_ERROR;
				Log::Error(L"ConnectionManager (httpGet/Response) " + StringConverter::decimal2String(httpCode));
			}
		}
		info.file.close();

		// CLEAN UP
		curl_slist_free_all(headers);
	}

	return res;
}

///////////////////////////////////////////////////////////////////////////////////////
///						ConnectionManager::CurlHandle Implementation
///////////////////////////////////////////////////////////////////////////////////////
ConnectionManager::CurlHandle::CurlHandle()
	: m_Socket(-1)
	, m_CurlObj(nullptr)
{
}

ConnectionManager::CurlHandle::~CurlHandle()
{	
	if (m_CurlObj)
	{
		curl_easy_cleanup(m_CurlObj);
		m_CurlObj = NULL;
	}
}

void ConnectionManager::CurlHandle::Init()
{
	if (m_CurlObj)
		curl_easy_cleanup(m_CurlObj);;

	m_CurlObj = curl_easy_init();
	curl_easy_setopt(m_CurlObj, CURLOPT_OPENSOCKETFUNCTION, opensocketCallBack);
	curl_easy_setopt(m_CurlObj, CURLOPT_CLOSESOCKETFUNCTION, closesocketCallBack);
	curl_easy_setopt(m_CurlObj, CURLOPT_OPENSOCKETDATA, this);
	curl_easy_setopt(m_CurlObj, CURLOPT_CLOSESOCKETDATA, this);
}



void ConnectionManager::CurlHandle::Close()
{
	if (m_Socket != -1)
	{
		shutdown(m_Socket, 2); //SD_BOTH
		sclose(m_Socket);
	}
}

curl_socket_t ConnectionManager::CurlHandle::opensocketCallBack(void *clientp, curlsocktype purpose, struct curl_sockaddr *addr)
{
	CurlHandle* curlHandle=((CurlHandle*)clientp);
	curlHandle->m_Socket=socket(addr->family, addr->socktype, addr->protocol);
	return curlHandle->m_Socket;
}

int ConnectionManager::CurlHandle::closesocketCallBack(void *clientp, curl_socket_t sock)
{
	CurlHandle* curlHandle=((CurlHandle*)clientp);
	curlHandle->m_Socket=-1;
	return sclose(sock);
}


