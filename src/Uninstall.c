#include "Headers.h"
#include "Installation.h"
#include "Lib.StringBuilderW.h"
#include "Lib.CommandLineArgs.h"

static int DeleteSystemAliases(void)
{
  int code = 0;

  int result = 0;
  if (
      printf("Registry: Delete Key: HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe\n"),
      result = RegDeleteKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe", 0, 0),
      0 != result && ERROR_FILE_NOT_FOUND != result)
  {
    code = -1;
    SetLastError(result);
    PrintLastError(code);
  }

  SetLastError(0);
  if (
      printf("Registry: Delete Key: HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe\n"),
      result = RegDeleteKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe", 0, 0),
      0 != result && ERROR_FILE_NOT_FOUND != result)
  {
    code = -2;
    SetLastError(result);
    PrintLastError(code);
  }

  return code;
}

static int RemoveFromSystemPath(void)
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
  else
  {
    wchar_t *new_path = RemoveFromDelimitedListW(path, L';', L"C:\\Program Files\\RunElevate");
    if (NULL == new_path)
    {
      printf("System path does not contain installation folder.\n");
    }
    else if (
        printf("Remove installation folder from system path.\n"),
        RegSetValueExW(hKey, L"PATH_BACKUP_U", 0, REG_EXPAND_SZ, (BYTE *)path, (DWORD)((1 + wcslen(path)) * sizeof(wchar_t))),
        0 != RegSetValueExW(hKey, L"Path", 0, REG_EXPAND_SZ, (BYTE *)new_path, (DWORD)((1 + wcslen(new_path)) * sizeof(wchar_t))))
    {
      code = -4;
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

  printf("Delete installation files and folder.\n");
  DeleteInstallationFiles();
  DeleteSystemAliases();
  RemoveFromSystemPath();

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
