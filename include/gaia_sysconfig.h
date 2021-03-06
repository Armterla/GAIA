#ifndef		__GAIA_SYS_CONFIG_H__
#define		__GAIA_SYS_CONFIG_H__

#define GINL inline
#define GSINL static GINL

/* System codepage. */
#define GAIA_SYS_CODEPAGE "GBK"

/* Thread safe. */
#define GAIA_THREADSAFE_ON 1
#define GAIA_THREADSAFE_OFF 2
#ifndef __GAIA_THREADSAFE_OFF__
#	define GAIA_THREADSAFE GAIA_THREADSAFE_ON
#endif

/* APP mode. */
#define GAIA_APPMODE_CONSOLE 1
#define GAIA_APPMODE_GUI 2
#ifdef __GAIA_CONSOLE__
#	define GAIA_APPMODE GAIA_APPMODE_CONSOLE
#elif defined(__GAIA_GUI__)
#	define GAIA_APPMODE GAIA_APPMODE_GUI
#else
#	define GAIA_APPMODE GAIA_APPMODE_CONSOLE
#endif

/* Stream mode. */
#ifndef __GAIA_NOSTREAM__
#	define GAIA_STREAM
#endif

/* OS flag. */
#define GAIA_OS_WINDOWS 1
#define GAIA_OS_OSX 2
#define GAIA_OS_IOS 3
#define GAIA_OS_ANDROID 4
#define GAIA_OS_LINUX 5
#define GAIA_OS_UNIX 6
#ifdef __GAIA_OS_WINDOWS__
#	define GAIA_OS GAIA_OS_WINDOWS
#elif defined(__GAIA_OS_LINUX__)
#	define GAIA_OS GAIA_OS_LINUX
#elif defined(__GAIA_OS_OSX__)
#	define GAIA_OS GAIA_OS_OSX
#elif defined(__GAIA_OS_IOS__)
#	define GAIA_OS GAIA_OS_IOS
#elif defined(__GAIA_OS_ANDROID__)
#	define GAIA_OS GAIA_OS_ANDROID
#else
#	ifdef _WIN32
#		define GAIA_OS GAIA_OS_WINDOWS
#	elif defined(__linux__)
#		define GAIA_OS GAIA_OS_LINUX
#	elif defined(TARGET_OS_MAC)
#		if TARGET_OS_OSX
#			define GAIA_OS GAIA_OS_OSX
#		elif TARGET_OS_IOS
#			define GAIA_OS GAIA_OS_IOS
#		endif
#	else
#		define GAIA_OS GAIA_OS_OSX
#	endif
#endif

/* OS can't support features(TEMP USAGE). */
#if GAIA_OS == GAIA_OS_IOS
#	define __GAIA_NO_HEAP__
#	define __GAIA_NO_DEBUG_MEMORYLEAK__
#endif

/* Simulator. */
#define GAIA_SIMULATOR_DISABLE 0
#define GAIA_SIMULATOR_ENABLE 1
#if GAIA_OS_IOS
#	if defined(TARGET_OS_SIMULATOR)
#		if TARGET_OS_SIMULATOR
#			define GAIA_SIMULATOR GAIA_SIMULATOR_ENABLE
#		else
#			define GAIA_SIMULATOR GAIA_SIMULATOR_DISABLE
#		endif
#	else
#		define GAIA_SIMULATOR GAIA_SIMULATOR_DISABLE
#	endif
#else
#	define GAIA_SIMULATOR GAIA_SIMULATOR_DISABLE
#endif

/* Compiler profile. */
#define GAIA_PROFILE_DEBUG 1
#define GAIA_PROFILE_RELEASE 2
#ifdef __GAIA_PROFILE_DEBUG__
#	define GAIA_PROFILE GAIA_PROFILE_DEBUG
#elif defined(__GAIA_PROFILE_RELEASE__)
#	define GAIA_PROFILE GAIA_PROFILE_RELEASE
#else
#	if GAIA_OS == GAIA_OS_WINDOWS
#		ifdef _DEBUG
#			define GAIA_PROFILE GAIA_PROFILE_DEBUG
#		else
#			define GAIA_PROFILE GAIA_PROFILE_RELEASE
#		endif
#	elif GAIA_OS == GAIA_OS_OSX || GAIA_OS == GAIA_OS_IOS
#		ifdef DEBUG
#			define GAIA_PROFILE GAIA_PROFILE_DEBUG
#		else
#			define GAIA_PROFILE GAIA_PROFILE_RELEASE
#		endif
#	else
#		define GAIA_PROFILE GAIA_PROFILE_RELEASE
#	endif
#endif

/* Compiler type. */
#define GAIA_COMPILER_VC 1
#define GAIA_COMPILER_GCC 2
#define GAIA_COMPILER_CLANG 3
#ifdef __GAIA_COMPILER_VC__
#	define GAIA_COMPILER GAIA_COMPILER_VC
#elif defined(__GAIA_COMPILER_GCC__)
#	define GAIA_COMPILER GAIA_COMPILER_GCC
#elif defined(__GAIA_COMPILER_CLANG__)
#	define GAIA_COMPILER GAIA_COMPILER_CLANG
#else
#	ifdef _MSC_VER
#		define GAIA_COMPILER GAIA_COMPILER_VC
#	else
#		define GAIA_COMPILER GAIA_COMPILER_GCC
#	endif
#endif

/* Compiler version. */
#define GAIA_COMPILER_VCVER_FIRST 0x0010
#define GAIA_COMPILER_VCVER_LATEST 0x0010
#define GAIA_COMPILER_VCVER GAIA_COMPILER_VCVER_LATEST

#define GAIA_COMPILER_GCCVER_FIRST 0x0010
#define GAIA_COMPILER_GCCVER_USESYNCXX 0x0080
#define GAIA_COMPILER_GCCVER_LATEST 0x0080
#define GAIA_COMPILER_GCCVER GAIA_COMPILER_GCCVER_LATEST

#define GAIA_COMPILER_CLANGVER_FIRST 0x0010
#define GAIA_COMPILER_CLANGVER_LATEST 0x0010
#define GAIA_COMPILER_CLANGVER GAIA_COMPILER_CLANGVER_LATEST

/* Language code flag. */
#define GAIA_CHARSET_ANSI 1
#define GAIA_CHARSET_UNICODE 2
#ifdef __GAIA_UNICODE__
#	define GAIA_CHARSET GAIA_CHARSET_UNICODE
#elif defined(__GAIA_ANSI__)
#	define GAIA_CHARSET GAIA_CHARSET_ANSI
#else
#	define GAIA_CHARSET GAIA_CHARSET_UNICODE
#endif

/* Machine bit count flag. */
#define GAIA_MACHINE32 32
#define	GAIA_MACHINE64 64
#define GAIA_MACHINE128 128
#ifdef __GAIA_MACHINE32__
#	define GAIA_MACHINE GAIA_MACHINE32
#elif defined(__GAIA_MACHINE64__)
#	define GAIA_MACHINE GAIA_MACHINE64
#elif defined(__GAIA_MACHINE128__)
#	define GAIA_MACHINE GAIA_MACHINE128
#else
#	ifdef _MSC_VER
#		if defined(_WIN64)
#			define GAIA_MACHINE GAIA_MACHINE64
#		else
#			define GAIA_MACHINE GAIA_MACHINE32
#		endif
#	elif __GAIA_X32__
#		define GAIA_MACHINE GAIA_MACHINE32
#	elif __GAIA_X64__
#		define GAIA_MACHINE GAIA_MACHINE64
#	elif __GAIA_X128__
#		define GAIA_MACHINE GAIA_MACHINE128
#	else
#		define GAIA_MACHINE GAIA_MACHINE64
#	endif
#endif

#if GAIA_MACHINE == GAIA_MACHINE32
#	define GAIA_MAX_UNSIGNED_INTEGER 0xFFFFFFFF
#elif GAIA_MACHINE == GAIA_MACHINE64
#	define GAIA_MAX_UNSIGNED_INTEGER 0xFFFFFFFFFFFFFFFF
#else
#	define GAIA_MAX_UNSIGNED_INTEGER 0xFFFFFFFF
#endif

/* Heap. */
#ifndef __GAIA_NO_HEAP__
#	ifndef GAIA_HEAP
#		define GAIA_HEAP
#	endif
#endif

#ifndef __GAIA_NO_LIMIT4G_HEAP__
#	ifndef GAIA_HEAP_LIMIT4G
#		define GAIA_HEAP_LIMIT4G
#	endif
#endif

#ifndef __GAIA_NO_HEAP_THREADSAFE__
#	ifndef GAIA_HEAP_THREADSAFE
#		define GAIA_HEAP_THREADSAFE
#	endif
#endif

#ifndef __GAIA_NO_HEAP_FORGLOBAL__
#	ifndef GAIA_HEAP_FORGLOBAL
#		define GAIA_HEAP_FORGLOBAL
#	endif
#endif

#ifndef	__GAIA_NO_HEAP_FORCLASS__
#	ifndef GAIA_HEAP_FORCLASS
#		define GAIA_HEAP_FORCLASS
#	endif
#endif

#ifndef	__GAIA_NO_HEAP_FORCUSTOM__
#	ifndef GAIA_HEAP_FORCUSTOM
#		define GAIA_HEAP_FORCUSTOM
#	endif
#endif

/* Strategy */
#define GAIA_STRATEGY_NORMAL 1
#define GAIA_STRATEGY_MEMPRIORITY 2
#define GAIA_STRATEGY_SPEEDPRIORITY 3
#ifdef __GAIA_STRATEGY_NORMAL__
#	define GAIA_STRATEGY GAIA_STRATEGY_NORMAL
#elif defined(__GAIA_STRATEGY_MEMPRIORITY__)
#	define GAIA_STRATEGY GAIA_STRATEGY_MEMPRIORITY
#elif defined(__GAIA_STRATEGY_SPEEDPRIORITY__)
#	define GAIA_STRATEGY GAIA_STRATEGY_SPEEDPRIORITY
#else
#	define GAIA_STRATEGY GAIA_STRATEGY_SPEEDPRIORITY
#endif

/* Feature mode. */
#ifndef __GAIA_NO_FEATURE_SQLITE__
#	ifndef GAIA_FEATURE_SQLITE
#		define GAIA_FEATURE_SQLITE
#	endif
#endif

#ifndef	__GAIA_NO_FEATURE_PCRE__
#	ifndef GAIA_FEATURE_PCRE
#		define GAIA_FEATURE_PCRE
#	endif
#endif

#ifndef __GAIA_NO_FEATURE_JPEG__
#	ifndef GAIA_FEATURE_JPEG
#		define GAIA_FEATURE_JPEG
#	endif
#endif

#ifndef __GAIA_NO_FEATURE_PNG__
#	ifndef GAIA_FEATURE_PNG
#		define GAIA_FEATURE_PNG
#	endif
#endif

#ifndef __GAIA_NO_FEATURE_HPDF__
#	ifndef GAIA_FEATURE_HPDF
#		define GAIA_FEATURE_HPDF
#	endif
#endif

/* Debug mode. */
#ifndef __GAIA_NO_DEBUG_SOLUTION__
#	ifndef GAIA_DEBUG_SOLUTION
#		define GAIA_DEBUG_SOLUTION
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_SELFCHECK__
#	ifndef GAIA_DEBUG_SELFCHECK
#		define GAIA_DEBUG_SELFCHECK
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_ROUTINE__
#	ifndef GAIA_DEBUG_ROUTINE
#		define GAIA_DEBUG_ROUTINE
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_WARNING__
#	ifndef GAIA_DEBUG_WARNING
#		define GAIA_DEBUG_WARNING
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_CODING__
#	ifndef GAIA_DEBUG_CODING
#		define GAIA_DEBUG_CODING
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_AST__
#	ifndef GAIA_DEBUG_AST
#		define GAIA_DEBUG_AST
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_STATICAST__
#	ifndef GAIA_DEBUG_STATICAST
#		define GAIA_DEBUG_STATICAST
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_MACHINELENGTH__
#	ifndef GAIA_DEBUG_MACHINELENGTH
#		define GAIA_DEBUG_MACHINELENGTH
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_MEMORYLEAK__
#	ifndef GAIA_DEBUG_MEMORYLEAK
#		define GAIA_DEBUG_MEMORYLEAK
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_PLATFORM__
#	ifndef GAIA_DEBUG_PLATFORM
#		define GAIA_DEBUG_PLATFORM
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_LOG__
#	ifndef GAIA_DEBUG_LOG
#		define GAIA_DEBUG_LOG
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_PRINTEXCEPTION__
#	ifndef GAIA_DEBUG_PRINTEXCEPTION
#		define GAIA_DEBUG_PRINTEXCEPTION
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_THROWEXCEPTION__
#	ifndef GAIA_DEBUG_THROWEXCEPTION
#		define GAIA_DEBUG_THROWEXCEPTION
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_CACHEEXCEPTION__
#	ifndef GAIA_DEBUG_CATCHEXCEPTION
#		define GAIA_DEBUG_CATCHEXCEPTION
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_ASTDEBUG__
#	ifndef GAIA_DEBUG_ASTDEBUG
#		define GAIA_DEBUG_ASTDEBUG
#	endif
#endif

#ifndef	__GAIA_NO_DEBUG_PERF__
#	ifndef GAIA_DEBUG_PERF
#		define GAIA_DEBUG_PERF
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_OBJWATCH__
#	ifndef GAIA_DEBUG_OBJWATCH
#		define GAIA_DEBUG_OBJWATCH
#	endif
#endif

#ifndef __GAIA_NO_DEBUG_INSTANCECOUNT__
#	ifndef GAIA_DEBUG_INSTANCECOUNT
#		define GAIA_DEBUG_INSTANCECOUNT
#	endif
#endif

/* Debug const flag. */
#ifdef GAIA_DEBUG_CODING
#	define GAIA_DEBUG_CONST
#else
#	define GAIA_DEBUG_CONST const
#endif

/* Enum helper. */
#define GAIA_ENUM_BEGIN(enumname) enum enumname{enumname##_INVALID = 0,
#define GAIA_ENUM_END(enumname) enumname##_MAXENUMCOUNT,enumname##_FORCEUNSIGNEDINTEGER=GAIA_MAX_UNSIGNED_INTEGER,};
#define GAIA_ENUM_VALID(enumname, name) (((name) > enumname##_INVALID) && ((name) < enumname##_MAXENUMCOUNT))

/* Warning adjust. */
#ifdef GAIA_DEBUG_WARNING
#	if GAIA_COMPILER == GAIA_COMPILER_VC
#		pragma warning(disable : 4100)
#		pragma warning(disable : 4189)
#		pragma warning(disable : 4996)
#		pragma warning(disable : 4127)
#		pragma warning(disable : 4201)
#		pragma warning(disable : 4800)
#		pragma warning(disable : 4244)
#		pragma warning(disable : 4291)

#		pragma warning(disable : 4005)
#		pragma warning(disable : 4211)
#		pragma warning(disable : 4267)
#		pragma warning(disable : 4505)
#		pragma warning(disable : 4701)
#		pragma warning(disable : 4702)
#		pragma warning(disable : 4706)
#	endif
#endif

/* Real type. */
#define GAIA_REAL_F32 1
#define GAIA_REAL_F64 2
#ifdef __GAIA_REAL_F32__
#	define GAIA_REAL GAIA_REAL_F32
#elif defined(__GAIA_REAL_F64__)
#	define GAIA_REAL GAIA_REAL_F64
#else
#	define GAIA_REAL GAIA_REAL_F32
#endif

/* Function call type. */
#if GAIA_OS == GAIA_OS_WINDOWS
#	define GAIA_BASEAPI __stdcall
#else
#	define GAIA_BASEAPI
#endif

/* Integration flag. */
#ifndef __GAIA_NO_USESTL__
#	define GAIA_USESTL
#endif

/* Line break flag. */
#define GAIA_FILELINEBREAK_RN "\r\n"
#define GAIA_FILELINEBREAK_R "\r"
#define GAIA_FILELINEBREAK_N "\n"
#ifdef __GAIA_FILELINEBREAK_RN__
#	define GAIA_FILELINEBREAK GAIA_FILELINEBREAK_RN
#elif defined(__GAIA_FILELINEBREAK_R__)
#	define GAIA_FILELINEBREAK GAIA_FILELINEBREAK_R
#elif defined(__GAIA_FILELINEBREAK_N__)
#	define GAIA_FILELINEBREAK GAIA_FILELINEBREAK_N
#else
#	define GAIA_FILELINEBREAK GAIA_FILELINEBREAK_N
#endif

/* Coordinate flag. */
#define GAIA_COORDINATE_LEFTHAND 1
#define GAIA_COORDINATE_RIGHTHAND 2
#ifdef __GAIA_LEFTHAND__
#	define GAIA_COORDINATE GAIA_COORDINATE_LEFTHAND
#elif defined(__GAIA_RIGHTHAND__)
#	define GAIA_COORDINATE GAIA_COORDINATE_RIGHTHAND
#else
#	define GAIA_COORDINATE GAIA_COORDINATE_LEFTHAND
#endif

#endif
