#pragma once

#include "SEAPIIncludes.h"

namespace ServerEye
{

	class StringConverter
	{
		StringConverter();
	public:

		#ifdef _WIN32
			static wstring s2ws(UINT CodePage, const char* s, size_t slength);		
			static wstring s2ws_ANSI(const string& s);
			static wstring s2ws_OEM(const string& s);
		#endif
		static wstring c2ws_UTF8(char *s, size_t slength = string::npos);
		static wstring s2ws_UTF8(const string& s);
		
		#ifdef _WIN32
			static string ws2s(UINT CodePage, const wchar_t* s, size_t slength);
			static string ws2s_ANSI(const wstring& s);
		#endif

		static string ws2s_UTF8(const wstring& s);

		static int string2Int(const wstring& s, int errorDefault = 0);
		static long long string2LongLong(const wstring& s, long long errorDefault = 0);

		static wstring decimal2String(int v);
		static wstring decimal2String(unsigned int v);
		static wstring decimal2String(long v);
		static wstring decimal2String(long long v);
		static wstring decimal2String(unsigned long long v);

		static void ByteBufferToBase64String(const ByteBuffer& buffer, string& base64Str);
		static void ByteBufferToBase64String(const ByteBuffer& buffer, wstring& base64Str);
		static bool base64StringToByteBuffer(const string& base64Str, ByteBuffer& buffer);
		static bool base64StringToByteBuffer(const wstring& base64Str, ByteBuffer& buffer);
		static void stringToBase64String(const string& str, string& base64Str);
		static void stringToBase64String(const wstring& str, wstring& base64Str);
		static bool base64StringToString(const string& base64Str, string& str);
		static bool base64StringToString(const wstring& base64Str, wstring& str);

		static vector<wstring>& split(const wstring& s, wchar_t delim, vector<wstring> &elems);
		static vector<wstring> split(const wstring& s, wchar_t delim);

		#ifdef _WIN32
			static size_t ci_find_substr(const string& s, const string& sub, size_t offset = 0);
			static size_t ci_find_substr(const wstring& s, const wstring& sub, size_t offset = 0);
		#endif

	};

}
