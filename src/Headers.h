#ifndef _Headers_h_
#define _Headers_h_

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

// information on spectre mitigation
#pragma warning(disable : 5045)

// treat as error
#pragma warning(error : 4047) // "var1" differs in levels of indirection from "var2"
#pragma warning(error : 4018) // signed/unsigned mismatch
#pragma warning(error : 4133) // incompatible types - from "type1" to "type2"
#pragma warning(error : 4244) // conversion from "type1" to "type2", possible loss of data
#pragma warning(error : 4700) // uninitialized local variable used
#pragma warning(error : 4715) // not all control paths return a value

#include <windows.h>

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#endif /* _Headers_h_ */
