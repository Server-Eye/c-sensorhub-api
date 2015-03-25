#pragma once

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
#include <windows.h>
#endif
#include <string>
#include <memory>
#include <vector>
#include <map>
#include <set>

using std::shared_ptr;
using std::wstring;
using std::string;
using std::vector;
using std::map;
using std::set;

typedef unsigned char U8;
typedef std::vector<U8> ByteBuffer;

#include "SEAPIForwards.h"
#include "SEObject.h"
