#include "Headers.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  UNREFERENCED_PARAMETER(hInstance);
  UNREFERENCED_PARAMETER(hPrevInstance);
  // UNREFERENCED_PARAMETER(pCmdLine);
  UNREFERENCED_PARAMETER(nCmdShow);

  // attempt to elevate using ShellExecute and pass arguments
  ShellExecuteW(
      NULL,          // Parent window (NULL for no parent)
      L"runas",      // Verb to request elevation
      L"run",        // Path to the wrapper application
      pCmdLine,      // Command-line arguments
      NULL,          // Default directory
      SW_SHOWDEFAULT // Show normal window
  );

  return 0;
}
