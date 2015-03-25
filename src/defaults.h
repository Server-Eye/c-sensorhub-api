#pragma once

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#include "SEAPIIncludes.h"
#include "StringConverter.h"
#include "targetver.h"
#include <iostream>
#include <fstream>

#include "json_spirit/json_spirit.h"
#include "boost/thread.hpp"
#include "boost/date_time.hpp"
#include "boost/interprocess/sync/scoped_lock.hpp"
#include "boost/interprocess/interprocess_fwd.hpp"

#define URL_SERVER_EYE L"https://api.server-eye.de:443/1/"
#define PUSH L"push/"
#define SUBJECT_AUTH_KEY L"auth/key"
#define SUBJECT_CONTAINER L"container"
#define SUBJECT_AGENT L"agent"
#define APIKEY_HEADER L"?apiKey="
