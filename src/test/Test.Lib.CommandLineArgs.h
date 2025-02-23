#include "../Headers.h"
#include "../Lib.Standard.h"
#include "../Lib.PointerManager.h"
#include "../Lib.StringBuilderW.h"
#include "../Lib.CommandLineArgs.h"

#include "Test.h"

void Test_Lib()
{
  label("FreeCommandLineArgsW");
  {
    size_t argc = 0;
    wchar_t **argv = NULL;
    argc = 3;
    argv = calloc(3, sizeof(wchar_t *));
    argv[0] = calloc(2, sizeof(wchar_t));
    argv[0][0] = L'1';
    argv[1] = calloc(2, sizeof(wchar_t));
    argv[1][0] = L'2';
    argv[2] = calloc(2, sizeof(wchar_t));
    argv[2][0] = L'3';
    check("Check arg 1.", ArgEqualsW(argv, argc, 0, L"1"));
    check("Check arg 2.", ArgEqualsW(argv, argc, 1, L"2"));
    check("Check arg 3.", ArgEqualsW(argv, argc, 2, L"3"));
    FreeCommandLineArgsW(&argv, &argc);
    check("Should free the array.", NULL == argv);
    check("Should set the count to 0.", 0 == argc);
  }

  label("ParseCommandLineArgsW");
  {
    size_t argc = 0;
    wchar_t **argv = NULL;
    // NULL
    argv = ParseCommandLineArgsW(NULL, &argc);
    check("NULL pointer should set argc to 0.", 0 == argc);
    check("NULL pointer should return NULL.", NULL == argv);
    FreeCommandLineArgsW(&argv, &argc);
    // Empty
    argv = ParseCommandLineArgsW(L"", &argc);
    check("Empty string should set argc to 0.", 0 == argc);
    check("Empty string should return NULL.", NULL == argv);
    FreeCommandLineArgsW(&argv, &argc);
    // No quotes, 1 arg
    argv = ParseCommandLineArgsW(L"one", &argc);
    check("No quotes, 1 arg. (argc)", 1 == argc);
    check("No quotes, 1 arg. (argv)", ArgEqualsW(argv, argc, 0, L"one"));
    FreeCommandLineArgsW(&argv, &argc);
    argv = ParseCommandLineArgsW(L" one ", &argc);
    check("No quotes, 1 arg, extra spacing. (argc)", 1 == argc);
    check("No quotes, 1 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 0, L"one"));
    FreeCommandLineArgsW(&argv, &argc);
    // No quotes, 3 arg
    argv = ParseCommandLineArgsW(L"one two three", &argc);
    check("No quotes, 3 arg. (argc)", 3 == argc);
    check("No quotes, 3 arg. (argv)", ArgEqualsW(argv, argc, 0, L"one"));
    check("No quotes, 3 arg. (argv)", ArgEqualsW(argv, argc, 1, L"two"));
    check("No quotes, 3 arg. (argv)", ArgEqualsW(argv, argc, 2, L"three"));
    FreeCommandLineArgsW(&argv, &argc);
    argv = ParseCommandLineArgsW(L"  one  two  three  ", &argc);
    check("No quotes, 3 arg, extra spacing. (argc)", 3 == argc);
    check("No quotes, 3 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 0, L"one"));
    check("No quotes, 3 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 1, L"two"));
    check("No quotes, 3 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 2, L"three"));
    FreeCommandLineArgsW(&argv, &argc);
    // Quotes, 1 arg
    argv = ParseCommandLineArgsW(L"\"one\"", &argc);
    check("Quotes, 1 arg. (argc)", 1 == argc);
    check("Quotes, 1 arg. (argv)", ArgEqualsW(argv, argc, 0, L"\"one\""));
    FreeCommandLineArgsW(&argv, &argc);
    argv = ParseCommandLineArgsW(L" \" one \" ", &argc);
    check("Quotes, 1 arg, extra spacing. (argc)", 1 == argc);
    check("Quotes, 1 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 0, L"\" one \""));
    FreeCommandLineArgsW(&argv, &argc);
    // Quotes, 3 arg
    argv = ParseCommandLineArgsW(L"\"one\" \"two\" \"three\"", &argc);
    check("Quotes, 3 arg. (argc)", 3 == argc);
    check("Quotes, 3 arg. (argv)", ArgEqualsW(argv, argc, 0, L"\"one\""));
    check("Quotes, 3 arg. (argv)", ArgEqualsW(argv, argc, 1, L"\"two\""));
    check("Quotes, 3 arg. (argv)", ArgEqualsW(argv, argc, 2, L"\"three\""));
    FreeCommandLineArgsW(&argv, &argc);
    argv = ParseCommandLineArgsW(L"  \"  one  \"  \"  two  \"  \"  three  \"  ", &argc);
    check("Quotes, 3 arg, extra spacing. (argc)", 3 == argc);
    check("Quotes, 3 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 0, L"\"  one  \""));
    check("Quotes, 3 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 1, L"\"  two  \""));
    check("Quotes, 3 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 2, L"\"  three  \""));
    FreeCommandLineArgsW(&argv, &argc);
    // Mismatched quotes
    argv = ParseCommandLineArgsW(L"  \"  one  \"  \"  two  \"  \"  three  \"  ", &argc);
    check("Quotes, 3 arg, extra spacing. (argc)", 3 == argc);
    check("Quotes, 3 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 0, L"\"  one  \""));
    check("Quotes, 3 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 1, L"\"  two  \""));
    check("Quotes, 3 arg, extra spacing. (argv)", ArgEqualsW(argv, argc, 2, L"\"  three  \""));
    FreeCommandLineArgsW(&argv, &argc);
  }

  label("SplitDelimitedListW");
  {
    size_t item_count = 0;
    wchar_t **item_list = NULL;
    // NULL
    item_list = SplitDelimitedListW(NULL, &item_count, L';');
    check("NULL pointer should set item_count to 0.", 0 == item_count);
    check("NULL pointer should return NULL.", NULL == item_list);
    FreeCommandLineArgsW(&item_list, &item_count);
    // Empty
    item_list = SplitDelimitedListW(L"", &item_count, L';');
    check("Empty string should set item_count to 0.", 0 == item_count);
    check("Empty string should return NULL.", NULL == item_list);
    FreeCommandLineArgsW(&item_list, &item_count);
    // No quotes, 1 arg
    item_list = SplitDelimitedListW(L"one", &item_count, L';');
    check("1 arg. (item_count)", 1 == item_count);
    check("1 arg. (item_list)", ArgEqualsW(item_list, item_count, 0, L"one"));
    FreeCommandLineArgsW(&item_list, &item_count);
  }

  label("Clean up path.");
  {
    size_t argc = 0;
    wchar_t **argv = NULL;
    argv = ParseCommandLineArgsW(L"C:\\ \"C:\\\" \"C:\\path\\to\\file.txt\"", &argc);

    label("ArgGetPointerW");
    check("should return argv[0] pointer", argv[0] == ArgGetPointerW(argv, 3, 0));
    check("should return argv[1] pointer", argv[1] == ArgGetPointerW(argv, 3, 1));
    check("should return argv[2] pointer", argv[2] == ArgGetPointerW(argv, 3, 2));

    label("ArgEqualsW");
    check("argv[0] should equal 'C:\\'", TRUE == ArgEqualsW(argv, 3, 0, L"C:\\"));
    check("argv[1] should equal '\"C:\\\"'", TRUE == ArgEqualsW(argv, 3, 1, L"\"C:\\\""));
    check("argv[2] should equal '\"C:\\path\\to\\file.txt\"'", TRUE == ArgEqualsW(argv, 3, 2, L"\"C:\\path\\to\\file.txt\""));

    label("ArgCleanUpPathW");
    wchar_t *clean0 = ArgGetCleanPathW(argv, 3, 0);
    wchar_t *clean1 = ArgGetCleanPathW(argv, 3, 1);
    wchar_t *clean2 = ArgGetCleanPathW(argv, 3, 2);
    check("argv[0] should equal 'C:\\'", TRUE == wcs_equals(clean0, L"C:\\"));
    check("argv[1] should equal 'C:\\'", TRUE == wcs_equals(clean1, L"C:\\"));
    check("argv[2] should equal 'C:\\path\\to\\file.txt'", TRUE == wcs_equals(clean2, L"C:\\path\\to\\file.txt"));
    free(clean0);
    free(clean1);
    free(clean2);

    FreeCommandLineArgsW(&argv, &argc);
  }
}
