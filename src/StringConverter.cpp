#include "defaults.h"
#include "StringConverter.h"
#include "boost/lexical_cast.hpp"
#ifndef _WIN32
	#include "boost/locale.hpp"
#endif

using namespace ServerEye;

#ifdef _WIN32
wstring StringConverter::s2ws(UINT CodePage, const char* s, size_t slength)
{
	int len = MultiByteToWideChar(CodePage, 0, s, (int)slength, 0, 0);
	wstring r(len, L'\0');
	MultiByteToWideChar(CodePage, 0, s, (int)slength, &r[0], len);
	return r;
}

wstring StringConverter::s2ws_ANSI(const string& s)
{
	return s2ws(CP_ACP, s.c_str(), s.size());
}

wstring StringConverter::s2ws_OEM(const string& s)
{
	return s2ws(CP_OEMCP, s.c_str(), s.size());
}


wstring StringConverter::s2ws_UTF8(const string& s)
{
	return s2ws(CP_UTF8, s.c_str(), s.size());
}

wstring StringConverter::c2ws_UTF8(char *s, size_t slength)
{
	if (slength == string::npos)
		slength = strlen(s);
	return s2ws(CP_UTF8, s, slength);
}
#else
wstring StringConverter::s2ws_UTF8(const string& s)
{
	return boost::locale::conv::utf_to_utf<wchar_t>(s);
}

wstring StringConverter::c2ws_UTF8(char *s, size_t slength)
{
	return boost::locale::conv::utf_to_utf<wchar_t>(s);
}
#endif

#ifdef _WIN32
string StringConverter::ws2s(UINT CodePage, const wchar_t* s, size_t slength)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, s, (int)slength, 0, 0, 0, 0);
	string r(len, '\0');
	WideCharToMultiByte(CP_UTF8, 0, s, (int)slength, &r[0], len, 0, 0);
	return r;
}

string StringConverter::ws2s_UTF8(const wstring& s)
{
	return ws2s(CP_UTF8, s.c_str(), s.size());
}

string StringConverter::ws2s_ANSI(const wstring& s)
{
	return ws2s(CP_ACP, s.c_str(), s.size());
}
#else
string StringConverter::ws2s_UTF8(const wstring& s)
{
	return boost::locale::conv::utf_to_utf<char>(s);
}
#endif

int StringConverter::string2Int(const wstring& s, int errorDefault)
{
	try
	{
		return boost::lexical_cast<int>(s);
	}
	catch (...)
	{
		return errorDefault;
	}
}

long long StringConverter::string2LongLong(const wstring& s, long long errorDefault)
{
	try
	{
		return boost::lexical_cast<long long>(s);
	}
	catch (...)
	{
		return errorDefault;
	}
}

wstring StringConverter::decimal2String(int v)
{
	return boost::lexical_cast<wstring>(v);
}

wstring StringConverter::decimal2String(unsigned int v)
{
	return boost::lexical_cast<wstring>(v);
}

wstring StringConverter::decimal2String(long v)
{
	return boost::lexical_cast<wstring>(v);
}

wstring StringConverter::decimal2String(long long v)
{
	return boost::lexical_cast<wstring>(v);
}


wstring StringConverter::decimal2String(unsigned long long v)
{
	return boost::lexical_cast<wstring>(v);
}

#define SMALL_BUFFER_SIZE 0x80

static const char base64Chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char base64CharsReverse[] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -1, -1, -2, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1,
	-1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17
	, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28, 29, 30, 31
	, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
	, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
	, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

void convToBase64(const void* input, int& _inputSize, void* output, int& _outputSize)
{
	U8 remain;
	char* pOutBuffer = (char*)output;
	const char* pOutEndBufferMod = (pOutBuffer + _outputSize - 4) - (_outputSize % 4);
	const unsigned char* pInBuffer = (const unsigned char*)input;
	const unsigned char* pInEndBuffer = pInBuffer + _inputSize;
#ifdef _WIN32
	// boosts only with vc optimization
	const unsigned char* pInEndBufferMod = pInEndBuffer - 3 - (_inputSize % 3);
	while (pInBuffer <= pInEndBufferMod && pOutBuffer <= pOutEndBufferMod)
	{
		*pOutBuffer++ = base64Chars[(*pInBuffer) >> 2];
		*pOutBuffer++ = base64Chars[((*pInBuffer & 0x03) << 4) + ((*(pInBuffer + 1)) >> 4)];
		*pOutBuffer++ = base64Chars[((*(pInBuffer + 1) & 0x0F) << 2) + ((*(pInBuffer + 2)) >> 6)];
		*pOutBuffer++ = base64Chars[(*(pInBuffer + 2) & 0x3F)];
		pInBuffer += 3;
	}
#endif
	while (pInBuffer < pInEndBuffer && pOutBuffer <= pOutEndBufferMod)
	{
		*pOutBuffer++ = base64Chars[(*pInBuffer) >> 2];
		remain = ((*pInBuffer & 0x03) << 4);
		pInBuffer++;
		if (pInBuffer < pInEndBuffer)
		{
			*pOutBuffer++ = base64Chars[remain + ((*pInBuffer) >> 4)];
			remain = ((*pInBuffer & 0x0F) << 2);
			pInBuffer++;
			if (pInBuffer < pInEndBuffer)
			{
				*pOutBuffer++ = base64Chars[remain + ((*pInBuffer) >> 6)];
				*pOutBuffer++ = base64Chars[(*pInBuffer & 0x3F)];
				pInBuffer++;
			}
			else
			{
				*pOutBuffer++ = base64Chars[remain];
				*pOutBuffer++ = '=';
				// do not break helps optimizer
			}
		}
		else
		{
			*pOutBuffer++ = base64Chars[remain];
			*pOutBuffer++ = '=';
			*pOutBuffer++ = '=';
			// do not break helps optimizer
		}
	}
	_inputSize = (int)(pInBuffer - (unsigned char*)input);
	_outputSize = (int)(pOutBuffer - ((char*)output));
}

void convFromBase64(const void* input, int& _inputSize, void* output, int& _outputSize)
{
	const char* pInBuffer = (const char*)input;
	const char* pInEndBuffer = pInBuffer + _inputSize;
	char* pOutBuffer = (char*)output;
	const char* pOutEndBuffer = pOutBuffer + _outputSize;
	const char* pOutEndBufferMod = pOutEndBuffer - (_outputSize % 3);
	char idx[4];
	while (pInBuffer < pInEndBuffer && pOutBuffer < pOutEndBufferMod)
	{
		char* id = idx;
		const char* pInStartBuffer = pInBuffer;
		for (int i = 0; i<4; i++)
		{
			if (pInBuffer >= pInEndBuffer) // not enough data, in release continue is faster than break TESTED under windows
				continue; // do not break helps optimizer
			*id = base64CharsReverse[*pInBuffer];
			pInBuffer++;
			if (*id == -2) // ignore \r \n
				continue;
			id++;
		}
		if (pInBuffer > pInEndBuffer) // not enough data
		{
			_inputSize = (int)(pInStartBuffer - (char*)input);
			_outputSize = (int)(pOutBuffer - (char*)output);
			return;
		}
		*pOutBuffer++ = (idx[0] << 2) | (idx[1] >> 4);
		if (idx[2] >= 0)
			*pOutBuffer++ = (idx[1] << 4) | (idx[2] >> 2);
		if (idx[3] >= 0)
			*pOutBuffer++ = ((idx[2] & 0x03) << 6) | idx[3];
	}
	_inputSize = (int)(pInBuffer - (char*)input);
	_outputSize = (int)(pOutBuffer - (char*)output);
}


void convertBufferToBase64(ByteBuffer& buffer, const U8* In_pBuffer, int In_BufferLen)
{
	U8 localbuffer[SMALL_BUFFER_SIZE];
	int inPos = 0;
	int outLen, inLen;
	while (inPos < In_BufferLen)
	{
		outLen = SMALL_BUFFER_SIZE;
		inLen = In_BufferLen - inPos;
		convToBase64(In_pBuffer + inPos, inLen, (U8 *)localbuffer, outLen);
		buffer.insert(buffer.end(), localbuffer, localbuffer + outLen);
		inPos += inLen;
	}
}

bool convertBufferFromBase64(ByteBuffer& Out_Buffer, const char* In_pBuffer, int In_BufferLen)
{
	U8 localbuffer[SMALL_BUFFER_SIZE];
	int inPos = 0, bufpos = 0;
	int inLen, outLen;
	while (inPos < In_BufferLen)
	{
		outLen = SMALL_BUFFER_SIZE;
		inLen = In_BufferLen - inPos;
		convFromBase64(In_pBuffer + inPos, inLen, (U8 *)localbuffer, outLen);
		Out_Buffer.resize(bufpos + outLen);
		memcpy(&Out_Buffer[bufpos], localbuffer, outLen);
		bufpos += outLen;
		inPos += inLen;
	}
	return true;
}

void StringConverter::ByteBufferToBase64String(const ByteBuffer& buffer, string& base64Str)
{
	ByteBuffer tmp;
	convertBufferToBase64(tmp, &buffer[0], (int)buffer.size());
	base64Str.assign((char*)&tmp[0], tmp.size());
}

void StringConverter::ByteBufferToBase64String(const ByteBuffer& buffer, wstring& base64Str)
{
	ByteBuffer tmp;
	convertBufferToBase64(tmp, &buffer[0], (int)buffer.size());
	base64Str = c2ws_UTF8((char*)&tmp[0], tmp.size());
}

bool StringConverter::base64StringToByteBuffer(const string& base64Str, ByteBuffer& buffer)
{
	return convertBufferFromBase64(buffer, base64Str.c_str(), (int)base64Str.size());
}

bool StringConverter::base64StringToByteBuffer(const wstring& base64Str, ByteBuffer& buffer)
{
	return base64StringToByteBuffer(ws2s_UTF8(base64Str), buffer);
}

void StringConverter::stringToBase64String(const string& str, string& base64Str)
{
	ByteBuffer tmp;
	convertBufferToBase64(tmp, (U8*)str.c_str(), (int)str.size());
	base64Str.assign((char*)&tmp[0], tmp.size());
}

void StringConverter::stringToBase64String(const wstring& str, wstring& base64Str)
{
	string tmpStr = StringConverter::ws2s_UTF8(str);
	ByteBuffer tmp;
	convertBufferToBase64(tmp, (U8*)tmpStr.c_str(), (int)tmpStr.size());
	base64Str = StringConverter::c2ws_UTF8((char*)&tmp[0], tmp.size());
}

bool StringConverter::base64StringToString(const string& base64Str, string& str)
{
	ByteBuffer tmp;
	if (!base64StringToByteBuffer(base64Str, tmp))
		return false;
	str.assign((char*)&tmp[0], tmp.size());
	return true;
}

bool StringConverter::base64StringToString(const wstring& base64Str, wstring& str)
{
	ByteBuffer tmp;
	if (!base64StringToByteBuffer(base64Str, tmp))
		return false;
	str = c2ws_UTF8((char*)&tmp[0], tmp.size());
	return true;
}

vector<wstring>& StringConverter::split(const wstring& s, wchar_t delim, vector<wstring> &elems)
{
	std::wstringstream ss(s);
	wstring item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

vector<wstring> StringConverter::split(const wstring& s, wchar_t delim)
{
	vector<wstring> elems;
	split(s, delim, elems);
	return elems;
}

#ifdef _WIN32
template<typename charT>
struct ci_equal
{
	ci_equal(const std::locale& loc) : loc_(loc) {}
	bool operator()(charT ch1, charT ch2)
	{
		return std::toupper(ch1, loc_) == std::toupper(ch2, loc_);
	}
private:
	const std::locale& loc_;
};

// find substring (case insensitive)
template<typename T>
size_t _ci_find_substr(const T& str1, const T& str2, size_t _Off = 0, const std::locale& loc = std::locale())
{
	T::const_iterator it = std::search(str1.begin() + _Off, str1.end(), str2.begin(), str2.end(), ci_equal<T::value_type>(loc));
	if (it != str1.end())
		return it - str1.begin();
	else
		return T::npos; // not found
}

string::size_type StringConverter::ci_find_substr(const string& s, const string& sub, size_t offset)
{
	return _ci_find_substr(s, sub, offset);
}

wstring::size_type StringConverter::ci_find_substr(const wstring& s, const wstring& sub, size_t offset)
{
	return _ci_find_substr(s, sub, offset);
}
#endif

