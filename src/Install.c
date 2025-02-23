#include "Headers.h"
#include "Lib.StringBuilderW.h"
#include "Lib.CommandLineArgs.h"

int CreateInstallationFolder()
{
  int code = 0;

  int result = 0;
  int lasterror = 0;

  printf("Create Directory: \"C:\\Program Files\\RunElevate\"\n");
  if (0 == CreateDirectory(L"C:\\Program Files\\RunElevate", NULL))
  {
    if (ERROR_ALREADY_EXISTS == GetLastError())
    {
    }
    if (ERROR_PATH_NOT_FOUND == GetLastError())
    {
    }
  }

  if (CreateDirectory(L"C:\\Program Files\\RunElevate", NULL) || GetLastError() == ERROR_ALREADY_EXISTS)
  {
    // RunElevate | Elevate.exe
    DeleteFile(L"C:\\Program Files\\RunElevate\\Elevate.exe");
    printf("Copy File: Elevate.exe\n");
    if (0 == CopyFile(L".\\Elevate.exe", L"C:\\Program Files\\RunElevate\\Elevate.exe", TRUE))
    {
      printf("Error: Failed to copy Elevate.exe into installation folder.\n");
    }
    // RunElevate | Run.exe
    DeleteFile(L"C:\\Program Files\\RunElevate\\Run.exe");
    printf("Copy File: Run.exe\n");
    if (0 == CopyFile(L".\\Run.exe", L"C:\\Program Files\\RunElevate\\Run.exe", TRUE))
    {
      printf("Error: Failed to copy Run.exe into installation folder.\n");
    }
    // RunElevate | vcruntime140.dll
    DeleteFile(L"C:\\Program Files\\RunElevate\\vcruntime140.dll");
    printf("Copy File: vcruntime140.dll\n");
    if (0 == CopyFile(L".\\vcruntime140.dll", L"C:\\Program Files\\RunElevate\\vcruntime140.dll", TRUE))
    {
      printf("Error: Failed to copy vcruntime140.dll into installation folder.\n");
    }
  }
  else
  {
    printf("Error: Failed to create installation folder.\n");
    printf("The installer requires administrator privileges to run.\n");
  }
}

int main(void)
{
  HKEY hKey = NULL;

  // NOTE: I used HKEY_CURRENT_USER at first, but the aliases didn't work on
  // Windows Sandbox, so I decided to switch over to HKEY_LOCAL_MACHINE.

  // Create alias for Elevate.exe
  if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL))
  {
    printf("Registry: Create Key: HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe\n");
    if (ERROR_SUCCESS == RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE *)L"C:\\Program Files\\RunElevate\\Elevate.exe", (DWORD)((1 + wcslen(L"C:\\Program Files\\RunElevate\\Elevate.exe")) * sizeof(wchar_t))))
    {
      printf("Set Default Value: C:\\Program Files\\RunElevate\\Elevate.exe\n");
      RegCloseKey(hKey);
    }
    else
    {
      printf("Error: Failed to write registry value for Elevate.exe.\n");
    }
  }
  else
  {
    printf("Error: Failed to create registry key for Elevate.exe.\n");
  }

  // Create alias for Run.exe
  if (ERROR_SUCCESS == RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL))
  {
    printf("Registry: Create Key: HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe\n");
    if (ERROR_SUCCESS == RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE *)L"C:\\Program Files\\RunElevate\\Run.exe", (DWORD)((1 + wcslen(L"C:\\Program Files\\RunElevate\\Run.exe")) * sizeof(wchar_t))))
    {
      printf("Set Default Value: C:\\Program Files\\RunElevate\\Run.exe\n");
      RegCloseKey(hKey);
    }
    else
    {
      printf("Error: Failed to write registry value for Run.exe.\n");
    }
  }
  else
  {
    printf("Error: Failed to create registry key for Run.exe.\n");
  }

  // Add installation folder to system path.
  printf("Registry: Open Key: HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment\n");
  if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment", 0, KEY_SET_VALUE | KEY_QUERY_VALUE, &hKey))
  {
    printf("Registry: Check System Path\n");
    // Get system path.
    DWORD size = 0;
    RegQueryValueEx(hKey, L"Path", NULL, NULL, NULL, &size);
    if (size > 0)
    {
      wchar_t *path = calloc(size + sizeof(L'\0'), 1);
      RegQueryValueEx(hKey, L"Path", NULL, NULL, (LPBYTE)path, &size);
      if (NULL != path)
      {
        BOOL found = FALSE;
        size_t item_count = 0;
        wchar_t **items = SplitDelimitedListW(path, &item_count, L';');
        for (size_t index = 0; index < item_count; ++index)
        {
          if (TRUE == wcs_equals(items[index], L"C:\\Program Files\\RunElevate"))
          {
            printf("Path Contains Installation Folder\n");
            found = TRUE;
            break;
          }
        }
        FreeCommandLineArgsW(&items, &item_count);
        if (FALSE == found)
        {
          printf("Path Does Not Contain Installation Folder\n");
          pStringBuilderW builder = StringBuilderW_From(path);
          StringBuilderW_Expand(builder, 30);
          StringBuilderW_Append(builder, L";");
          StringBuilderW_Append(builder, L"C:\\Program Files\\RunElevate");
          wchar_t *new_path = StringBuilderW_GetString(builder);
          StringBuilderW_Delete(&builder);
          if (NULL != new_path)
          {
            // Write to registry.
            printf("Registry: Back Up System Path as \"PATH_BACKUP\"\n");
            if (ERROR_SUCCESS == RegSetValueEx(hKey, L"PATH_BACKUP", 0, REG_EXPAND_SZ, (BYTE *)path, (DWORD)((1 + wcslen(path)) * sizeof(wchar_t))))
            {
            }
            else
            {
              printf("Error: Failed to back up system path.\n");
            }
            printf("Registry: Add Installation Folder To System Path\n");
            // RegDeleteValue(hKey, L"Path");
            if (ERROR_SUCCESS == RegSetValueEx(hKey, L"Path", 0, REG_EXPAND_SZ, (BYTE *)new_path, (DWORD)((1 + wcslen(new_path)) * sizeof(wchar_t))))
            {
              printf("Refresh Environment\n");
              // Issue with Ansi/Unicode processes. Send both to refresh environment.
              SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) "Environment", SMTO_ABORTIFHUNG, 1000, NULL);
              SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM)L"Environment", SMTO_ABORTIFHUNG, 1000, NULL);
            }
            else
            {
              printf("Error: Failed to add installation folder to system path.\n");
            }
            free(new_path);
          }
        }
        free(path);
      }
    }
    RegCloseKey(hKey);
  }
  else
  {
    printf("Error: Failed to open registry key for system path.\n");
  }

  printf("Installation Complete\n");
  system("pause");
  return 0;
}
