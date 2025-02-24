#include "Headers.h"
#include "Installation.h"
#include "Lib.StringBuilderW.h"
#include "Lib.CommandLineArgs.h"

static int DeleteInstallationFiles(void)
{
  DeleteFileW(L"C:\\Program Files\\RunElevate\\Elevate.exe");
  DeleteFileW(L"C:\\Program Files\\RunElevate\\Run.exe");
  DeleteFileW(L"C:\\Program Files\\RunElevate\\vcruntime140.dll");
  RemoveDirectoryW(L"C:\\Program Files\\RunElevate");
  return 0;
}

static int CopyInstallationFiles(void)
{
  int code = 0;

  if (
      printf("Create Directory: \"C:\\Program Files\\RunElevate\\\"\n"),
      0 == CreateDirectoryW(L"C:\\Program Files\\RunElevate", NULL) && ERROR_ALREADY_EXISTS != GetLastError())
  {
    code = -1;
  }
  else if (
      printf("Copy File: Elevate.exe\n"),
      0 == CopyFileW(L".\\Elevate.exe", L"C:\\Program Files\\RunElevate\\Elevate.exe", TRUE) && ERROR_ALREADY_EXISTS != GetLastError())
  {
    code = -2;
  }
  else if (
      printf("Copy File: Run.exe\n"),
      0 == CopyFileW(L".\\Run.exe", L"C:\\Program Files\\RunElevate\\Run.exe", TRUE) && ERROR_ALREADY_EXISTS != GetLastError())
  {
    code = -3;
  }
  else if (
      printf("Copy File: vcruntime140.dll\n"),
      0 == CopyFileW(L".\\vcruntime140.dll", L"C:\\Program Files\\RunElevate\\vcruntime140.dll", TRUE) && ERROR_ALREADY_EXISTS != GetLastError())
  {
    code = -4;
  }

  if (0 != code)
  {
    PrintLastError(code);
  }

  return code;
}

static int CreateSystemAliases(void)
{
  int code = 0;

  HKEY hKey = NULL;

  // NOTE: I used HKEY_CURRENT_USER at first, but the aliases didn't work on
  // Windows Sandbox, so I decided to switch over to HKEY_LOCAL_MACHINE.

  if (
      printf("Registry: Create Key: HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe\n"),
      0 != RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL))
  {
    code = -1;
  }
  else if (
      printf("Registry: Set Default Value: C:\\Program Files\\RunElevate\\Elevate.exe\n"),
      0 != RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE *)L"C:\\Program Files\\RunElevate\\Elevate.exe", (DWORD)((1 + wcslen(L"C:\\Program Files\\RunElevate\\Elevate.exe")) * sizeof(wchar_t))))
  {
    code = -2;
  }
  else if (
      printf("Registry: Close Key\n"),
      0 != RegCloseKey(hKey))
  {
    code = -3;
  }
  else if (
      printf("Registry: Create Key: HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe\n"),
      0 != RegCreateKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL))
  {
    code = -4;
  }
  else if (
      printf("Registry: Set Default Value: C:\\Program Files\\RunElevate\\Run.exe\n"),
      0 != RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE *)L"C:\\Program Files\\RunElevate\\Run.exe", (DWORD)((1 + wcslen(L"C:\\Program Files\\RunElevate\\Run.exe")) * sizeof(wchar_t))))
  {
    code = -5;
  }
  else if (
      printf("Registry: Close Key\n"),
      0 != RegCloseKey(hKey))
  {
    code = -6;
  }

  if (0 != code)
  {
    PrintLastError(code);
  }

  return code;
}

static int AddToSystemPath(void)
{
  int code = 0;

  HKEY hKey = NULL;
  DWORD size = 0;
  wchar_t *path = NULL;

  if (
      printf("Registry: Open Key: HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\n"),
      0 != RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hKey))
  {
    code = -1;
  }
  else if (
      printf("Get size of path.\n"),
      0 != RegQueryValueExW(hKey, L"Path", NULL, NULL, NULL, &size) || 0 == size)
  {
    code = -2;
  }
  else if (
      printf("Get value of path.\n"),
      path = calloc(size + sizeof(L'\0'), 1),
      NULL == path || 0 != RegQueryValueExW(hKey, L"Path", NULL, NULL, (LPBYTE)path, &size))
  {
    code = -3;
  }
  else if (
      TRUE == SearchDelimitedListW(path, L';', L"C:\\Program Files\\RunElevate"))
  {
    printf("System path contains installation folder.\n");
  }
  else
  {
    printf("System path does not contain installation folder.\n");
    pStringBuilderW builder = StringBuilderW_From(path);
    StringBuilderW_Expand(builder, 30);
    StringBuilderW_Append(builder, L";");
    StringBuilderW_Append(builder, L"C:\\Program Files\\RunElevate");
    wchar_t *new_path = StringBuilderW_GetString(builder);
    StringBuilderW_Delete(&builder);
    if (NULL == new_path)
    {
      code = -4;
    }
    else if (
        printf("Add installation folder to system path.\n"),
        RegSetValueExW(hKey, L"PATH_BACKUP_I", 0, REG_EXPAND_SZ, (BYTE *)path, (DWORD)((1 + wcslen(path)) * sizeof(wchar_t))),
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

  DeleteInstallationFiles();

  if (0 != CopyInstallationFiles())
  {
    code = -1;
  }
  else if (0 != CreateSystemAliases())
  {
    code = -2;
  }
  else if (0 != AddToSystemPath())
  {
    code = -3;
  }

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
