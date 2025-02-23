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
 * @param pList delimited string to split into substrings
 * @param pNumItems address of variable in which to store the number of valid
 * substrings split from the delimited string
 * @param delimiter the character by which to split the delimited string (may
 * not be the null character `\0`)
 */
wchar_t **SplitDelimitedListW(wchar_t *pList, size_t *pNumItems, wchar_t delimiter)
{
  wchar_t **argv = NULL;
  pPointerManager manager = NULL;

  int code = 0;
  if (NULL == pList || L'\0' == pList[0] || NULL == pNumItems)
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
    for (; L'\0' != pList[arg_index]; ++arg_index)
    {
      if (delimiter == pList[arg_index])
      {
        size_t const arg_length = arg_index - arg_start;
        if (arg_length > 0)
        {
          wchar_t *arg = calloc(arg_length + 1, sizeof(wchar_t));
          if (NULL == arg)
          {
            goto breakout;
          }
          memcpy(arg, pList + arg_start, arg_length * sizeof(wchar_t));
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
      memcpy(arg, pList + arg_start, arg_length * sizeof(wchar_t));
      if (ERROR == PointerManager_AddPointer(manager, arg))
      {
        free(arg);
        goto breakout;
      }
    }

    *pNumItems = manager->item_count;
    argv = (wchar_t **)manager->array;
    PointerManager_ReleasePointers(manager);
  }

  PointerManager_Delete(&manager);
  return argv;
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

#endif /* _Lib_CommandLineArgs_h_ */
