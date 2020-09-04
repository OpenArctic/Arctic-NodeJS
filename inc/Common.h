#pragma once

#ifdef _DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)


// Standard library
#include <any>
#include <atomic>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <streambuf>
#include <string>
#include <thread>
#include <type_traits>
#include <variant>
#include <vector>

// Windows
#include <WinSock2.h> // Must be located before <Windows.h>
#include <Windows.h>
#include <afunix.h>
#include <ShlObj_core.h>

#pragma comment(lib, "Ws2_32.lib")