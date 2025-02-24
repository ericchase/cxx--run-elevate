#include "Headers.h"
#include "Installation.h"
#include "Lib.StringBuilderW.h"
#include "Lib.CommandLineArgs.h"

static int GetUserPaths(wchar_t **pInstallationDirectory_reference, wchar_t **pElevateExe_reference, wchar_t **pRunExe_reference, wchar_t **pRuntimeDLL_reference)
{
  int code = 0;

  DWORD size = 0;
  wchar_t *path = NULL;
  pStringBuilderW builder = NULL;

  if (
      NULL == pInstallationDirectory_reference ||
      NULL == pElevateExe_reference ||
      NULL == pRunExe_reference ||
      NULL == pRuntimeDLL_reference)
  {
    code = -1;
  }
  else if (
      size = GetEnvironmentVariableW(L"LOCALAPPDATA", NULL, 0),
      path = calloc(size + 1, 1),
      NULL == path)
  {
    code = -2;
  }
  else if (
      printf("Variable: Get %%LOCALAPPDATA%%\n"),
      (size - 1) != GetEnvironmentVariableW(L"LOCALAPPDATA", path, size))
  {
    code = -3;
  }
  else if (
      builder = StringBuilderW_From(path),
      NULL == builder)
  {
    code = -4;
  }
  else
  {
    StringBuilderW_Append(builder, L"\\RunElevate");
    (*pInstallationDirectory_reference) = StringBuilderW_GetString(builder);
    size_t reset = builder->string_end;

    StringBuilderW_Append(builder, L"\\Elevate.exe");
    (*pElevateExe_reference) = StringBuilderW_GetString(builder);
    builder->string_end = reset;

    StringBuilderW_Append(builder, L"\\Run.exe");
    (*pRunExe_reference) = StringBuilderW_GetString(builder);
    builder->string_end = reset;

    StringBuilderW_Append(builder, L"\\vcruntime140.dll");
    (*pRuntimeDLL_reference) = StringBuilderW_GetString(builder);
    builder->string_end = reset;

    if (
        NULL == (*pInstallationDirectory_reference) ||
        NULL == (*pElevateExe_reference) ||
        NULL == (*pRunExe_reference) ||
        NULL == (*pRuntimeDLL_reference))
    {
      code = -5;
    }
  }

  StringBuilderW_Delete(&builder);
  memory_free(&path);

  if (0 != code)
  {
    PrintLastError(code);
  }

  return code;
}

static int DeleteUserInstallationFiles(wchar_t *pInstallationDirectory, wchar_t *pElevateExe, wchar_t *pRunExe, wchar_t *pRuntimeDLL)
{
  int code = 0;

  if (
      NULL == pInstallationDirectory ||
      NULL == pElevateExe ||
      NULL == pRunExe ||
      NULL == pRuntimeDLL)
  {
    code = -1;
  }
  else
  {
    DeleteFileW(pElevateExe);
    DeleteFileW(pRunExe);
    DeleteFileW(pRuntimeDLL);
    RemoveDirectoryW(pInstallationDirectory);
  }

  if (0 != code)
  {
    PrintLastError(code);
  }

  return code;
}

static int CopyUserInstallationFiles(wchar_t *pInstallationDirectory, wchar_t *pElevateExe, wchar_t *pRunExe, wchar_t *pRuntimeDLL)
{
  int code = 0;

  if (
      NULL == pInstallationDirectory ||
      NULL == pElevateExe ||
      NULL == pRunExe ||
      NULL == pRuntimeDLL)
  {
    code = -1;
  }
  else if (
      printf("Create Directory: \"%ls\"\n", pInstallationDirectory),
      0 == CreateDirectoryW(pInstallationDirectory, NULL) && ERROR_ALREADY_EXISTS != GetLastError())
  {
    code = -2;
  }
  else if (
      printf("Copy File: Elevate.exe\n"),
      0 == CopyFileW(L".\\Elevate.exe", pElevateExe, TRUE) && ERROR_ALREADY_EXISTS != GetLastError())
  {
    code = -3;
  }
  else if (
      printf("Copy File: Run.exe\n"),
      0 == CopyFileW(L".\\Run.exe", pRunExe, TRUE) && ERROR_ALREADY_EXISTS != GetLastError())
  {
    code = -4;
  }
  else if (
      printf("Copy File: vcruntime140.dll\n"),
      0 == CopyFileW(L".\\vcruntime140.dll", pRuntimeDLL, TRUE) && ERROR_ALREADY_EXISTS != GetLastError())
  {
    code = -5;
  }

  if (0 != code)
  {
    PrintLastError(code);
  }

  return code;
}

static int CreateUserAliases(wchar_t *pElevateExe, wchar_t *pRunExe)
{
  int code = 0;

  HKEY hKey = NULL;

  if (
      NULL == pElevateExe ||
      NULL == pRunExe)
  {
    code = -1;
  }
  else if (
      printf("Registry: Create Key: HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe\n"),
      0 != RegCreateKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL))
  {
    code = -2;
  }
  else if (
      printf("Registry: Set Default Value: %ls\n", pElevateExe),
      0 != RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE *)pElevateExe, (DWORD)((1 + wcslen(pElevateExe)) * sizeof(wchar_t))))
  {
    code = -3;
  }
  else if (
      printf("Registry: Close Key\n"),
      0 != RegCloseKey(hKey))
  {
    code = -4;
  }
  else if (
      printf("Registry: Create Key: HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe\n"),
      0 != RegCreateKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL))
  {
    code = -5;
  }
  else if (
      printf("Registry: Set Default Value: %ls\n", pRunExe),
      0 != RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE *)pRunExe, (DWORD)((1 + wcslen(pRunExe)) * sizeof(wchar_t))))
  {
    code = -6;
  }
  else if (
      printf("Registry: Close Key\n"),
      0 != RegCloseKey(hKey))
  {
    code = -7;
  }

  if (0 != code)
  {
    PrintLastError(code);
  }

  return code;
}

static int AddToUserPath(wchar_t *pInstallationDirectory)
{
  int code = 0;

  HKEY hKey = NULL;
  DWORD size = 0;
  wchar_t *path = NULL;

  if (
      NULL == pInstallationDirectory)
  {
    code = -1;
  }
  if (
      printf("Registry: Open Key: HKEY_CURRENT_USER\\Environment\n"),
      0 != RegOpenKeyExW(HKEY_CURRENT_USER, L"Environment", 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hKey))
  {
    code = -2;
  }
  else if (
      printf("Get size of path.\n"),
      0 != RegQueryValueExW(hKey, L"Path", NULL, NULL, NULL, &size) || 0 == size)
  {
    code = -3;
  }
  else if (
      printf("Get value of path.\n"),
      path = calloc(size + sizeof(L'\0'), 1),
      NULL == path || 0 != RegQueryValueExW(hKey, L"Path", NULL, NULL, (LPBYTE)path, &size))
  {
    code = -4;
  }
  else if (
      TRUE == SearchDelimitedListW(path, L';', pInstallationDirectory))
  {
    printf("System path contains installation folder.\n");
  }
  else
  {
    printf("System path does not contain installation folder.\n");
    pStringBuilderW builder = StringBuilderW_From(path);
    StringBuilderW_Expand(builder, 30);
    StringBuilderW_Append(builder, L";");
    StringBuilderW_Append(builder, pInstallationDirectory);
    wchar_t *new_path = StringBuilderW_GetString(builder);
    StringBuilderW_Delete(&builder);
    if (NULL == new_path)
    {
      code = -5;
    }
    else if (
        printf("Add installation folder to system path.\n"),
        RegSetValueExW(hKey, L"PATH_BACKUP_I", 0, REG_EXPAND_SZ, (BYTE *)path, (DWORD)((1 + wcslen(path)) * sizeof(wchar_t))),
        0 != RegSetValueExW(hKey, L"Path", 0, REG_EXPAND_SZ, (BYTE *)new_path, (DWORD)((1 + wcslen(new_path)) * sizeof(wchar_t))))
    {
      code = -6;
    }
    else if (
        printf("Refresh environment.\n"),
        // Issue with Ansi/Unicode processes. Send both to refresh environment.
        SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) "Environment", SMTO_ABORTIFHUNG, 1000, NULL),
        SendMessageTimeoutW(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"Environment", SMTO_ABORTIFHUNG, 1000, NULL),
        0)
    {
    }

    memory_free(&new_path);
  }

  memory_free(&path);
  RegCloseKey(hKey);

  if (0 != code)
  {
    PrintLastError(code);
  }

  return code;
}

int main(void)
{
  int code = 0;

  wchar_t *pInstallationDirectory = NULL;
  wchar_t *pElevateExe = NULL;
  wchar_t *pRunExe = NULL;
  wchar_t *pRuntimeDLL = NULL;
  if (0 != GetUserPaths(&pInstallationDirectory, &pElevateExe, &pRunExe, &pRuntimeDLL))
  {
    code = -1;
  }
  else
  {
    DeleteUserInstallationFiles(pInstallationDirectory, pElevateExe, pRunExe, pRuntimeDLL);
    if (0 != CopyUserInstallationFiles(pInstallationDirectory, pElevateExe, pRunExe, pRuntimeDLL))
    {
      code = -1;
    }
    else if (0 != CreateUserAliases(pElevateExe, pRunExe))
    {
      code = -2;
    }
    else if (0 != AddToUserPath(pInstallationDirectory))
    {
      code = -3;
    }
  }
  memory_free(&pInstallationDirectory);
  memory_free(&pElevateExe);
  memory_free(&pRunExe);
  memory_free(&pRuntimeDLL);

  if (0 == code)
  {
    printf("\nInstallation Successful\n");
  }
  else
  {
    printf("\nErrors During Installation\n");
    printf("The installer requires administrator privileges to run.\n");
    printf("Please write an issue on GitHub if you cannot resolve the problem.\n");
  }

  system("pause");

  return code;
}
