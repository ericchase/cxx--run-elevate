#ifndef _Installation_h_
#define _Installation_h_

#include "Headers.h"

void PrintLastError(int code)
{
  DWORD error_code = GetLastError();
  if (0 != error_code)
  {
    LPWSTR error_text = NULL;
    FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM           // use system message tables to retrieve error text
            | FORMAT_MESSAGE_ALLOCATE_BUFFER // allocate buffer on local heap for error text
            | FORMAT_MESSAGE_IGNORE_INSERTS, // Important! will fail otherwise, since we're not (and CANNOT) pass insertion parameters
        NULL,                                // unused with FORMAT_MESSAGE_FROM_SYSTEM
        error_code,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&error_text, // output
        0,                   // minimum size for output buffer
        NULL);               // arguments - see note

    printf("Last step failed with error code: [%i](%ld)\n", code, error_code);
    if (NULL != error_text)
    {
      printf("%ls\n", error_text);
      LocalFree(error_text);
      error_text = NULL;
    }
  }
}

#endif /* _Installation_h_ */
