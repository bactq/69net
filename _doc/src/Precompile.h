﻿#ifndef _PRECOMPILE_H_
#define _PRECOMPILE_H_

/*
index:

__IA            __IA32              __IA64
__X64           __X86 / __X32
__DEBUG         __RELEASE
__BE            __LE
__IOS           __ANDROID           __WIN / __WIN32 / __WINDOWS
__GCC           __MSVC / __RVCT

INLINE          NOINLINE
LIKELY          UNLIKELY
ALIGN2/4/8/16/32

*/

/* CPU(X86) - i386 / x86 32-bit */
#if defined(__i386__) || defined(i386) || defined(_M_IX86) || defined(_X86_) || defined(__THW_INTEL)
#define __IA32 1
#endif

/* CPU(X86_64) - AMD64 / Intel64 / x86_64 64-bit */
#if defined(__x86_64__) || defined(_M_X64)
#define __IA64 1
#endif

/* CPU - INTEL / AMD */
#if defined(__IA32) || defined(__IA64)
#define __IA 1
#endif

/* Compile - X86 / X64 */
#if defined(__LP64__) || defined(_WIN64) || defined(_M_X64)
#define __X64 1
#else
#define __X32 1
#define __X86 1
#endif

/* Compile - DEBUG / RELEASE */
#if defined(NDEBUG) || !defined(DEBUG)
#define __RELEASE 1
#else
#define __DEBUG 1
#endif

/* CPU - big endian / little endian */
#if defined(BIGENDIAN) || defined(BIG_ENDIAN)
#define __BE 1
#else
#define __LE 1
#endif

/* OS - windows, ios, android, linux */
#ifdef _WIN32
#define __WIN 1
#define __WIN32 1
#define __WINDOWS 1
#elif defined(TARGET_OS_IPHONE) || defined(TARGET_OS_IPAD)
#define __IOS 1
#elif defined(__ANDROID__)
#define __ANDROID 1
#endif
#ifdef __linux
#define __LINUX 1
#endif

/* Compiler - ARM RealView Compilation Tools */
#if defined(__CC_ARM) || defined(__ARMCC__)
#define __RVCT 1
#endif

/* Compiler - GNU Compiler Collection */
/* --gnu option of the RVCT compiler also defines __GNUC__ */
#if defined(__GNUC__) && !defined(__RVCT)
#define __GCC 1
#define __GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

/* Compiler - Microsoft Visual C++ */
#if defined(_MSC_VER)
#define __MSVC 1
#endif


/* Compile - enforce inline */
#ifndef INLINE
#if __GCC && __RELEASE && !defined(__MINGW32__)
#define INLINE inline __attribute__((__always_inline__))
#elif (__MSVC || __GCC) && __RELEASE
#define INLINE __forceinline
#else
#define INLINE inline
#endif
#endif

/* Compile - enforce noinline */
#ifndef NOINLINE
#if __GCC
#define NOINLINE __attribute__((__noinline__))
#elif __MSVC || __RVCT
#define NOINLINE __declspec(noinline)
#else
#define NOINLINE
#endif
#endif

/* CPU - branch prediction: unlikely */
#ifndef UNLIKELY
#if __GCC
#define UNLIKELY(x) __builtin_expect((x), 0)
#else
#define UNLIKELY(x) (x)
#endif
#endif

/* CPU - branch prediction: likely */
#ifndef LIKELY
#if __GCC
#define LIKELY(x) __builtin_expect((x), 1)
#else
#define LIKELY(x) (x)
#endif
#endif

/* Memory address align */
#if __MSVC
#define ALIGN2( x )		        __declspec(align(2)) x
#define ALIGN4( x )		        __declspec(align(4)) x
#define ALIGN8( x )		        __declspec(align(8)) x
#define ALIGN16( x )		    __declspec(align(16)) x
#define ALIGN32( x )		    __declspec(align(32)) x
#elif __GCC
#define ALIGN2( x )             x __attribute__ ((aligned (2)))
#define ALIGN4( x )             x __attribute__ ((aligned (4)))
#define ALIGN8( x )             x __attribute__ ((aligned (8)))
#define ALIGN16( x )            x __attribute__ ((aligned (16)))
#define ALIGN32( x )            x __attribute__ ((aligned (32)))
#else
#define ALIGN2( x )	 x
#define ALIGN4( x )	 x
#define ALIGN8( x )	 x
#define ALIGN16( x ) x
#define ALIGN32( x ) x
#endif



/* Common headers */
#if __WIN32
#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <windows.h>
#include <tchar.h>
#include <malloc.h>
#include <direct.h>
#include <intrin.h>
#elif __IOS
#include <netdb.h>
#include <unistd.h>
#include <malloc/malloc.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#elif __ANDROID
#include <netdb.h>
#include <asm/page.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <malloc.h>
#else
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <string.h>
#endif

#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <bitset>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <random>
#include <memory>
#include <new>

#include <type_traits>
#include <array>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <functional>
#include <chrono>
#include <locale>

#include <assert.h>
#include <stdint.h>
#include <fcntl.h>


/* Common types */
typedef int8_t  int8, sbyte;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t  uint8, byte;
typedef uint16_t uint16, ushort;
typedef uint32_t uint32, uint;
typedef uint64_t uint64;

typedef unsigned long ulong;    // x64: vc 32bit, gcc 64bit
typedef wchar_t wchar;          // win/ios: 16bit, linux, android 32bit


/* Common namespace & precompile headers */

#include "BaseUtils.h"
#include "BitMask.h"
#include "Converts.h"
#include "String.h"
#include "DateTime.h"
#include "BinaryHelper.h"
#include "Binary.h"
#include "Bytes.h"
#include "List.h"
#include "HashCode.h"
#include "Dict.h"
#include "Stopwatch.h"
#include "DbTable.h"
//#include "MyResult.h"
//#include "MyHelper.h"
//#include "DB.h"
//#include "MyQuery.h"
#include "Console.h"

#include "test1.h"
#include "Main.h"




/* hpp headers */
#include "String.hpp"
#include "Binary.hpp"
#include "Bytes.hpp"
#include "List.hpp"
#include "Dict.hpp"

#endif
