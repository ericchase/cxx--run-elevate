#include "Headers.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
  UNREFERENCED_PARAMETER(hInstance);
  UNREFERENCED_PARAMETER(hPrevInstance);
  // UNREFERENCED_PARAMETER(pCmdLine);
  UNREFERENCED_PARAMETER(nCmdShow);

  HANDLE file = NULL;
  DWORD bytes = 0;
  if (file = CreateFile(L"C:\\@\\echo.txt", GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0), NULL == file)
  {
    printf("Could not open file: %ld", GetLastError());
  }
  else if (0 == WriteFile(file, pCmdLine, (DWORD)(wcslen(pCmdLine) * sizeof(wchar_t)), &bytes, 0))
  {
    printf("Could not write bytes: %ld", GetLastError());
  }
  CloseHandle(file);

  return 0;
}
