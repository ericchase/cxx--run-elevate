#include "Headers.h"
#include "Lib.StringBuilderW.h"
#include "Lib.CommandLineArgs.h"

int main(void)
{
  HKEY hKey = NULL;

  // Delete binary files from installation folder.
  {
    // RunElevate | Elevate.exe
    printf("Delete File: \"C:\\Program Files\\RunElevate\\Elevate.exe\"\n");
    if (0 == DeleteFile(L"C:\\Program Files\\RunElevate\\Elevate.exe"))
    {
      printf("Error: Failed to delete Elevate.exe from installation folder.\n");
    }
    // RunElevate | Run.exe
    printf("Delete File: \"C:\\Program Files\\RunElevate\\Run.exe\"\n");
    if (0 == DeleteFile(L"C:\\Program Files\\RunElevate\\Run.exe"))
    {
      printf("Error: Failed to delete Run.exe from installation folder.\n");
    }
    // RunElevate | vcruntime140.dll
    printf("Delete File: \"C:\\Program Files\\RunElevate\\vcruntime140.dll\"\n");
    if (0 == DeleteFile(L"C:\\Program Files\\RunElevate\\vcruntime140.dll"))
    {
      printf("Error: Failed to delete vcruntime140.dll from installation folder.\n");
    }
    // RunElevate
    printf("Remove Directory: \"C:\\Program Files\\RunElevate\"\n");
    if (0 == RemoveDirectory(L"C:\\Program Files\\RunElevate"))
    {
      printf("Error: Failed to delete installation folder.\n");
    }
  }

  // Delete registry keys.
  {
    printf("Registry: Delete Key: HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe\n");
    if (ERROR_SUCCESS != RegDeleteKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Elevate.exe"))
    {
      printf("Error: Failed to delete registry key for Elevate.exe.\n");
    }
    printf("Registry: Delete Key: HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe\n");
    if (ERROR_SUCCESS != RegDeleteKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\App Paths\\Run.exe"))
    {
      printf("Error: Failed to delete registry key for Run.exe.\n");
    }
  }

  // Remove installation folder from system path.
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
        BOOL removed = FALSE;
        pStringBuilderW builder = StringBuilderW_New(size);
        size_t item_count = 0;
        wchar_t **items = SplitDelimitedListW(path, &item_count, L';');
        for (size_t index = 0; index < item_count; ++index)
        {
          if (TRUE == wcs_equals(items[index], L"C:\\Program Files\\RunElevate"))
          {
            removed = TRUE;
          }
          else
          {
            StringBuilderW_Append(builder, items[index]);
            StringBuilderW_Append(builder, L";");
          }
        }
        StringBuilderW_Trim(builder, L';');
        FreeCommandLineArgsW(&items, &item_count);
        if (TRUE == removed)
        {
          printf("Path Contains Installation Folder\n");
          wchar_t *new_path = StringBuilderW_GetString(builder);
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
            printf("Registry: Remove Installation Folder From System Path\n");
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
              printf("Error: Failed to remove installation folder to system path.\n");
            }
            free(new_path);
          }
        }
        else
        {
          printf("Path Does Not Contain Installation Folder\n");
        }
        StringBuilderW_Delete(&builder);
        free(path);
      }
    }
    RegCloseKey(hKey);
  }
  else
  {
    printf("Error: Failed to open registry key for system path.\n");
  }

  printf("Uninstallation Complete\n");
  system("pause");
  return 0;
}
