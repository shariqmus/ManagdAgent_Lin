// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "sys/types.h"
#include "sys/sysinfo.h"
#include "sys/statvfs.h"
#include "sys/utsname.h"

// C/C++ Includes
#include <stdio.h>
#include <unistd.h>

#include <array>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <regex>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
//#include <strsafe.h>
//#include <tchar.h>
#include <vector>

using namespace std;

// Custom Includes

// jsonxx -- https://github.com/hjiang/jsonxx
#include "../include/jsonxx.h"
using namespace jsonxx;

// Logging
//#include "log.h"

// Config file reader
#include "../include/INIReader.h"

// logging class
#include "../include/log.h"

// cURL
#include "../include/curl/curl.h"

//Timer
#include "../include/cxxtimer.h"

//libcurl.lib;crypt32.lib
//#pragma comment(lib, "..\\curl-7.46.0-win64\\lib\\libcurl.lib")
//#pragma comment(lib, "crypt32.lib")
//#pragma comment(lib, "cryptui.lib")
//#pragma comment(lib, "pdh.lib")
