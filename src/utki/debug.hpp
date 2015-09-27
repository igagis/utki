/**
 * @author Ivan Gagis <igagis@gmail.com>
 * @brief Debug utilities.
 */

#pragma once

#include "config.hpp"

#if M_OS == M_OS_SYMBIAN
#	include <e32std.h>

#elif M_OS_NAME == M_OS_NAME_ANDROID
#	undef NDEBUG // we want assertions to work, if we don't undef NDEBUG the assertions will be translated to nothing
#	include <cassert>

#	include <sstream>
#	include <android/log.h>

#else //assume more or less standard system
#	include <sstream>
#	include <iostream>
#	include <fstream>
#	include <typeinfo>
#	include <cassert>

#endif

#if defined(_DEBUG) && !defined(DEBUG)
#	define DEBUG
#endif

//
//
//  Logging definitions
//
//

#ifndef M_DOXYGEN_DONT_EXTRACT //for doxygen
namespace utki{
namespace ting_debug{
#	if M_OS == M_OS_SYMBIAN
#	elif M_OS_NAME == M_OS_NAME_ANDROID
#	else
inline std::ofstream& DebugLogger(){
	//this allows to make debug output even if main() is not called yet and even if
	//standard std::cout object is not created since static global variables initialization
	//order is undetermined in C++ if these variables are located in separate cpp files!
	static std::ofstream* logger = new std::ofstream("output.log");
	return *logger;
}
#	endif
}//~namespace ting_debug
}//~namespace ting
#endif //~M_DOXYGEN_DONT_EXTRACT //for doxygen



#if M_OS == M_OS_SYMBIAN
#	define LOG_ALWAYS(x)
#	define TRACE_ALWAYS(x)

#elif M_OS_NAME == M_OS_NAME_ANDROID
#	define TRACE_ALWAYS(x) \
		{ \
			std::stringstream ss; \
			ss x; \
			__android_log_write(ANDROID_LOG_INFO, "ting_debug", ss.str().c_str()); \
		}
#	define LOG_ALWAYS(x) //logging is not supported on Android, yet.

#else
#	define LOG_ALWAYS(x) utki::ting_debug::DebugLogger() x; utki::ting_debug::DebugLogger().flush();
#	define TRACE_ALWAYS(x) std::cout x; std::cout.flush();

#endif

#define TRACE_AND_LOG_ALWAYS(x) LOG_ALWAYS(x) TRACE_ALWAYS(x)



#ifdef DEBUG

#	define LOG(x) LOG_ALWAYS(x)
#	define TRACE(x) TRACE_ALWAYS(x)
#	define TRACE_AND_LOG(x) TRACE_AND_LOG_ALWAYS(x)

#	define LOG_IF_TRUE(x, y) if(x){ LOG(y) }

#	define DEBUG_CODE(x) x

#else//#ifdef DEBUG

#	define LOG(x)
#	define TRACE(x)
#	define TRACE_AND_LOG(x)
#	define LOG_IF_TRUE(x, y)
#	define DEBUG_CODE(x)

#endif//~#ifdef DEBUG



//
//
//  Assertion definitions
//
//
namespace utki{
namespace ting_debug{
inline void LogAssert(const char* msg, const char* file, int line){
	TRACE_AND_LOG_ALWAYS(<< "[!!!fatal] Assertion failed at:\n\t"<< file << ":" << line << "| " << msg << std::endl)
}
}
}
#if M_OS == M_OS_SYMBIAN
#	define ASSERT_INFO_ALWAYS(x, y) __ASSERT_ALWAYS((x), User::Panic(_L("ASSERTION FAILED!"),3));

#else //Assume system supporting standard assert() (including Android)

#	define ASSERT_INFO_ALWAYS(x, y) if(!(x)){ \
						std::stringstream ss; \
						ss << y; \
						utki::ting_debug::LogAssert(ss.str().c_str(), __FILE__, __LINE__); \
						assert(false); \
					}

#endif

#define ASSERT_ALWAYS(x) ASSERT_INFO_ALWAYS((x), "no additional info")



#ifdef DEBUG
#	define ASSERT_INFO(x, y) ASSERT_INFO_ALWAYS((x), y)
#	define ASSERT(x) ASSERT_ALWAYS(x)

#	if M_OS == M_OS_SYMBIAN
#		define ASS(x) (x)
#		define ASSCOND(x, cond) (x)

#	else //Assume system supporting standard assert() (including Android)
#		define ASS(x) ( (x) ? (x) : (utki::ting_debug::LogAssert("ASS() assertion macro", __FILE__, __LINE__), (assert(false)), (x)) )
#		define ASSCOND(x, cond) ( ((x) cond) ? (x) : (utki::ting_debug::LogAssert("ASS() assertion macro", __FILE__, __LINE__), (assert(false)), (x)) )

#	endif

#else //No DEBUG macro defined
#	define ASSERT_INFO(x, y)
#	define ASSERT(x)
#	define ASS(x) (x)
#	define ASSCOND(x, cond) (x)

#endif//~#ifdef DEBUG
