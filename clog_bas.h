#ifndef __CLOG_BASICS_H__
#define __CLOG_BASICS_H__ 1

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif


typedef char clog_ch;
typedef unsigned int clog_len;

#define CLOG_DEBUG 1

#ifdef CLOG_DEVELOPER
#include <assert.h>
#else
#ifndef assert
#define assert(x)
#endif
#endif /* CLOG_DEVELOPER */

#endif /* __CLOG_BASICS_H__ */
