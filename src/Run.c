#include "Headers.h"
#include "Lib.CommandLineArgs.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  UNREFERENCED_PARAMETER(hInstance);
  UNREFERENCED_PARAMETER(hPrevInstance);
  // UNREFERENCED_PARAMETER(pCmdLine);
  UNREFERENCED_PARAMETER(nCmdShow);

  size_t argc = 0;
  wchar_t **argv = ParseCommandLineArgsW(pCmdLine, &argc);

  wchar_t *working_directory = NULL;
  wchar_t *executable_path = NULL;
  wchar_t *parameters = NULL;

  if (TRUE == ArgEqualsW(argv, argc, 0, L"/d") ||
      TRUE == ArgEqualsW(argv, argc, 0, L"-d"))
  {
    if (argc > 2)
    {
      working_directory = ArgGetCleanPathW(argv, argc, 1);
      executable_path = ArgGetCleanPathW(argv, argc, 2);
      parameters = ArgGetPointerW(argv, argc, 3);
    }
  }
  else
  {
    executable_path = ArgGetCleanPathW(argv, argc, 0);
    parameters = ArgGetPointerW(argv, argc, 1);
  }

  if (NULL != executable_path)
  {
    if (NULL != parameters)
    {
      // find location of argument after executable_path in pCmdLine
      parameters = wcsstr(pCmdLine, parameters);
    }

    // use ShellExecuteW to run executable with provided arguments and working directory
    ShellExecuteW(
        NULL,               // no parent window
        (wchar_t *)L"open", // operation to perform
        executable_path,    // executable to run
        parameters,         // command-line parameters
        working_directory,  // working directory (NULL if not specified)
        SW_SHOWDEFAULT      // show command
    );
  }

  memory_free(&working_directory);
  memory_free(&executable_path);
  FreeCommandLineArgsW(&argv, &argc);

  return 0;
}
