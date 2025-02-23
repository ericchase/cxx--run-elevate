#ifndef _Lib_CommandLineArgs_h_
#define _Lib_CommandLineArgs_h_

#include "Headers.h"

#include "Lib.Standard.h"
#include "Lib.PointerManager.h"
#include "Lib.StringBuilderW.h"

/**
 * Splits a `wchar_t*` command-line string based on whitespace and double quote
 * characters.
 *
 * A command-line string is a concatenation of substrings delimited by spaces
 * and tab characters (and sometimes other whitespace characters as well). This
 * function only splits on spaces and tab characters. A substring may not
 * contain spaces or tab characters unless the substring itself is surrounded
 * by a pair of double quote characters. A substring may also not contain any
 * double quote characters unless those characters are escaped with a backslash
 * and the substring itself is surrounded by a pair of double quote characters.
 * An escaped double quote character is a double quote character preceded by a
 * single backslash character (ie. `\\\"`).
 *
 * Use `FreeCommandLineArgsW` to free the allocated memory returned.
 *
 * @param pCmdLine command-line string to parse
 * @param pNumArgs address of variable in which to store the number of valid
 * arguments parsed from the command-line string
 */
wchar_t **ParseCommandLineArgsW(wchar_t *pCmdLine, size_t *pNumArgs)
{
  wchar_t **argv = NULL;
  pPointerManager manager = NULL;

  int code = 0;
  if (NULL == pCmdLine || L'\0' == pCmdLine[0] || NULL == pNumArgs)
  {
    code = -1;
  }
  else if (*pNumArgs = 0, manager = PointerManager_New(4), NULL == manager)
  {
    code = -2;
  }
  else if (0)
  {
  breakout:
    code = -3;
  }
  else
  {
    BOOL in_quotes = FALSE;
    size_t arg_start = 0;
    size_t arg_index = 0;
    for (; L'\0' != pCmdLine[arg_index]; ++arg_index)
    {
      switch (pCmdLine[arg_index])
      {
      case L' ':
      case L'\t':
        if (FALSE == in_quotes)
        {
          size_t const arg_length = arg_index - arg_start;
          if (arg_length > 0)
          {
            wchar_t *arg = calloc(arg_length + 1, sizeof(wchar_t));
            if (NULL == arg)
            {
              goto breakout;
            }
            memcpy(arg, pCmdLine + arg_start, arg_length * sizeof(wchar_t));
            if (ERROR == PointerManager_AddPointer(manager, arg))
            {
              free(arg);
              goto breakout;
            }
          }
          arg_start = arg_index + 1;
        }
        break;
      case L'"':
        if (FALSE == in_quotes)
        {
          in_quotes = TRUE;
        }
        else
        {
          in_quotes = FALSE;
        }
        break;
      }
    }
    size_t const arg_length = arg_index - arg_start;
    if (arg_length > 0)
    {
      wchar_t *arg = calloc(arg_length + 1, sizeof(wchar_t));
      if (NULL == arg)
      {
        goto breakout;
      }
      memcpy(arg, pCmdLine + arg_start, arg_length * sizeof(wchar_t));
      if (ERROR == PointerManager_AddPointer(manager, arg))
      {
        free(arg);
        goto breakout;
      }
    }

    *pNumArgs = manager->item_count;
    argv = (wchar_t **)manager->array;
    PointerManager_ReleasePointers(manager);
  }

  PointerManager_Delete(&manager);
  return argv;
}

/**
 * Splits a `wchar_t*` delimited string on the provided `delimiter` character.
 *
 * Unlike `ParseCommandLineArgsW`, this function does not perform any special
 * escaped character handling. It only performs a basic splitting of the input
 * string on a single delimiter character.
 *
 * Use `FreeCommandLineArgsW` to free the allocated memory returned.
 *
 * @param pStr delimited string to split into substrings
 * @param pNumItems address of variable in which to store the number of valid
 * substrings split from the delimited string
 * @param delimiter the character by which to split the delimited string (may
 * not be the null character `\0`)
 */
wchar_t **SplitDelimitedListW(wchar_t *pStr, size_t *pNumItems, wchar_t delimiter)
{
  wchar_t **items = NULL;
  pPointerManager manager = NULL;

  int code = 0;
  if (NULL == pStr || L'\0' == pStr[0] || NULL == pNumItems)
  {
    code = -1;
  }
  else if (*pNumItems = 0, manager = PointerManager_New(4), NULL == manager)
  {
    code = -2;
  }
  else if (0)
  {
  breakout:
    code = -3;
  }
  else
  {

    size_t arg_start = 0;
    size_t arg_index = 0;
    for (; L'\0' != pStr[arg_index]; ++arg_index)
    {
      if (delimiter == pStr[arg_index])
      {
        size_t const arg_length = arg_index - arg_start;
        if (arg_length > 0)
        {
          wchar_t *arg = calloc(arg_length + 1, sizeof(wchar_t));
          if (NULL == arg)
          {
            goto breakout;
          }
          memcpy(arg, pStr + arg_start, arg_length * sizeof(wchar_t));
          if (ERROR == PointerManager_AddPointer(manager, arg))
          {
            free(arg);
            goto breakout;
          }
        }
        arg_start = arg_index + 1;
      }
    }
    size_t const arg_length = arg_index - arg_start;
    if (arg_length > 0)
    {
      wchar_t *arg = calloc(arg_length + 1, sizeof(wchar_t));
      if (NULL == arg)
      {
        goto breakout;
      }
      memcpy(arg, pStr + arg_start, arg_length * sizeof(wchar_t));
      if (ERROR == PointerManager_AddPointer(manager, arg))
      {
        free(arg);
        goto breakout;
      }
    }

    *pNumItems = manager->item_count;
    items = (wchar_t **)manager->array;
    PointerManager_ReleasePointers(manager);
  }

  PointerManager_Delete(&manager);
  return items;
}

/**
 * Frees an array allocated by a successful call to the `ParseCommandLineArgsW`
 * or `SplitDelimitedListW` functions.
 *
 * Both `argv_reference` and `argc_reference` will be set to NULL on successful
 * calls.
 *
 * @param argv_reference address of array to free
 * @param argc_reference address of variable holding number of items in array
 */
BOOL FreeCommandLineArgsW(wchar_t ***argv_reference, size_t *argc_reference)
{
  if (NULL == argv_reference || NULL == (*argv_reference) || NULL == argc_reference)
  {
    return ERROR;
  }

  // Free each pointer.
  for (size_t index = 0; index < (*argc_reference); ++index)
  {
    memory_free(&((void *)(*argv_reference)[index]));
  }

  // Free the container.
  memory_free((void **)argv_reference);
  (*argc_reference) = 0;

  return OK;
}

/**
 * Does bounds checking and returns the pointer for `index` argument in `argv`.
 * Returns `NULL` if `index` is out of bounds.
 *
 * @param argv array of arguments
 * @param argc number of arguments in array
 * @param index index of argument to get
 */
wchar_t *ArgGetPointerW(wchar_t **argv, size_t argc, size_t index)
{
  if (NULL == argv)
  {
    return NULL;
  }
  if (index >= argc)
  {
    return NULL;
  }
  if (NULL == argv[index])
  {
    return NULL;
  }
  return argv[index];
}

/**
 * Compares `index` argument in `argv` array to `compare_to_str`.
 * Returns `TRUE` or `FALSE`.
 *
 * @param argv array of arguments
 * @param argc number of arguments in array
 * @param index index of argument to compare
 * @param compare_to_str string to compare argument against
 */
BOOL ArgEqualsW(wchar_t **argv, size_t argc, size_t index, wchar_t *compare_to_str)
{
  return wcs_equals(ArgGetPointerW(argv, argc, index), compare_to_str);
}

/**
 * Copies `index` argument in `argv` array into new string and cleans it for
 * use in some Windows APIs related to command line arguments.
 * Use `free` to release the memory.
 *
 * @param argv array of arguments
 * @param argc number of arguments in array
 * @param index index of argument to process
 */
wchar_t *ArgGetCleanPathW(wchar_t **argv, size_t argc, size_t index)
{
  pStringBuilderW builder = StringBuilderW_From(ArgGetPointerW(argv, argc, index));
  if (NULL == builder)
  {
    return NULL;
  }

  StringBuilderW_TrimListInOrder(builder, L" \"");
  wchar_t *clean_path = StringBuilderW_GetString(builder);
  StringBuilderW_Delete(&builder);
  return clean_path;
}

/**
 * Joins a list of `wchar_t*`, separating each by the provided `separator`
 * string.
 *
 * Use `free` to free the returned string.
 *
 * @param argv array of arguments
 * @param argc number of arguments in array
 * @param separator the string used to separate each argument
 * @param init_length number of characters to reserve for builder at start
 */
wchar_t *JoinArgsW(wchar_t **argv, size_t argc, wchar_t *separator, size_t init_length)
{
  wchar_t *joined = NULL;
  if (NULL != argv)
  {
    pStringBuilderW builder = StringBuilderW_New(init_length);
    size_t start = 0;
    while (start < argc && NULL == argv[start])
    {
      ++start;
    }
    StringBuilderW_Append(builder, argv[start]);
    for (size_t index = start + 1; index < argc; ++index)
    {
      if (NULL != argv[index])
      {
        StringBuilderW_Append(builder, separator);
        StringBuilderW_Append(builder, argv[index]);
      }
    }
    joined = StringBuilderW_GetString(builder);
    StringBuilderW_Delete(&builder);
  }
  return joined;
}

/**
 * Removes target string from arguments.
 * Returns the count of items removed.
 *
 * @param argv array of arguments
 * @param argc number of arguments in array
 * @param query the string to remove
 */
size_t RemoveFromArgsW(wchar_t **argv, size_t argc, wchar_t *query)
{
  size_t count = 0;
  if (NULL != argv && NULL != query)
  {
    for (size_t index = 0; index < argc; ++index)
    {
      if (TRUE == wcs_equals(argv[index], query))
      {
        memory_free(&((void *)argv[index]));
        ++count;
      }
    }
  }
  return count;
}

/**
 * Searches arguments for target string.
 *
 * @param argv array of arguments
 * @param argc number of arguments in array
 * @param query the string to search for
 */
BOOL SearchArgsW(wchar_t **argv, size_t argc, wchar_t *query)
{
  if (NULL != argv && NULL != query)
  {
    for (size_t index = 0; index < argc; ++index)
    {
      if (TRUE == wcs_equals(argv[index], query))
      {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/**
 * Calls `SplitDelimitedListW`, `RemoveFromArgsW`, `JoinArgsW`, and returns
 * resulting string.
 *
 * Use `free` to free the returned string.
 *
 * @param pStr delimited string to split into substrings
 * @param delimiter the character by which to split the delimited string (may
 * not be the null character `\0`)
 * @param query the string to search for
 */
wchar_t *RemoveFromDelimitedListW(wchar_t *pStr, wchar_t delimiter, wchar_t *query)
{
  wchar_t *joined = NULL;
  if (NULL != pStr && NULL != query)
  {
    size_t item_count = 0;
    wchar_t **items = SplitDelimitedListW(pStr, &item_count, delimiter);
    size_t remove_count = RemoveFromArgsW(items, item_count, query);
    if (0 != remove_count)
    {
      wchar_t separator[2] = {delimiter, L'\0'};
      joined = JoinArgsW(items, item_count, separator, wcslen(pStr));
    }
    FreeCommandLineArgsW(&items, &item_count);
  }
  return joined;
}

/**
 * Calls `SplitDelimitedListW` and searches resulting items for target string.
 *
 * @param pStr delimited string to split into substrings
 * @param delimiter the character by which to split the delimited string (may
 * not be the null character `\0`)
 * @param query the string to search for
 */
BOOL SearchDelimitedListW(wchar_t *pStr, wchar_t delimiter, wchar_t *query)
{
  BOOL found = FALSE;
  if (NULL != pStr && NULL != query)
  {
    size_t item_count = 0;
    wchar_t **items = SplitDelimitedListW(pStr, &item_count, delimiter);
    for (size_t index = 0; index < item_count; ++index)
    {
      if (TRUE == wcs_equals(items[index], query))
      {
        found = TRUE;
        break;
      }
    }
    FreeCommandLineArgsW(&items, &item_count);
  }
  return found;
}

#endif /* _Lib_CommandLineArgs_h_ */
