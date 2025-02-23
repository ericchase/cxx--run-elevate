#ifndef _Lib_TestSuite_h_
#define _Lib_TestSuite_h_

#include "../Headers.h"

void label(char *label)
{
  printf("\n%s\n", label);
}

void check(char *label, int result)
{
  if (1 == result)
  {
    printf("\033[32m  PASS  \033[0m%s\n", label);
  }
  else
  {
    printf("\033[31m  FAIL  \033[0m%s\n", label);
  }
}

#endif /* _Lib_TestSuite_h_ */
