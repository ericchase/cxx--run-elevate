#include "../Headers.h"

#include "Test.Lib.Standard.h"
#include "Test.Lib.PointerManager.h"
#include "Test.Lib.StringBuilderW.h"
#include "Test.Lib.CommandLineArgs.h"

void main(void)
{
  printf("\nBEGIN\n");

  Test_Lib_Standard();
  Test_Lib_PointerManager();
  Test_Lib_StringBuilderW();
  Test_Lib();

  printf("\nEND\n");
}
