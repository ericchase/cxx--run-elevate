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

static int DeleteUserAliases(void)
{
  int code = 0;

  int result = 0;
  if (
      printf("Registry: Delete Key: HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe\n"),
      result = RegDeleteKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe", 0, 0),
      0 != result && ERROR_FILE_NOT_FOUND != result)
  {
    code = -1;
    SetLastError(result);
    PrintLastError(code);
  }

  SetLastError(0);
  if (
      printf("Registry: Delete Key: HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe\n"),
      result = RegDeleteKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe", 0, 0),
      0 != result && ERROR_FILE_NOT_FOUND != result)
  {
    code = -2;
    SetLastError(result);
    PrintLastError(code);
  }

  return code;
}

static int RemoveFromUserPath(wchar_t *pInstallationDirectory)
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
  else if (
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
  else
  {
    wchar_t *new_path = RemoveFromDelimitedListW(path, L';', pInstallationDirectory);
    if (NULL == new_path)
    {
      printf("System path does not contain installation folder.\n");
    }
    else if (
        printf("Remove installation folder from system path.\n"),
        RegSetValueExW(hKey, L"PATH_BACKUP_U", 0, REG_EXPAND_SZ, (BYTE *)path, (DWORD)((1 + wcslen(path)) * sizeof(wchar_t))),
        0 != RegSetValueExW(hKey, L"Path", 0, REG_EXPAND_SZ, (BYTE *)new_path, (DWORD)((1 + wcslen(new_path)) * sizeof(wchar_t))))
    {
      code = -5;
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
    printf("Delete installation files and folder.\n");
    DeleteUserInstallationFiles(pInstallationDirectory, pElevateExe, pRunExe, pRuntimeDLL);
    DeleteUserAliases();
    RemoveFromUserPath(pInstallationDirectory);
  }
  memory_free(&pInstallationDirectory);
  memory_free(&pElevateExe);
  memory_free(&pRunExe);
  memory_free(&pRuntimeDLL);

  if (0 == code)
  {
    printf("\nUninstallation Successful\n");
  }
  else
  {
    printf("\nErrors During Uninstallation\n");
    printf("The uninstaller requires administrator privileges to run.\n");
    printf("Please write an issue on GitHub if you cannot resolve the problem.\n");
  }

  system("pause");

  return code;
}
