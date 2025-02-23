#ifndef _Lib_Standard_h_
#define _Lib_Standard_h_

#include "Headers.h"

typedef int BOOL;
typedef unsigned char BYTE;

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef ERROR
#define ERROR 0
#endif

#ifndef OK
#define OK 1
#endif

/**
 * @param data_reference address of array to free
 */
int memory_free(void **array_reference)
{
  int code = 0;
  if (NULL == array_reference)
  {
    code = -1;
  }
  else if (NULL == (*array_reference))
  {
    code = -2;
  }
  else
  {
    free((*array_reference));
    (*array_reference) = NULL;
  }
  return code;
}

/**
 * Sets all bytes in `array` to `0`, starting at `index_start` and ending
 * before `index_end`. Returns `ERROR` if `item_size == 0` or if
 * `index_start >= index_end`.
 *
 * @param array pointer to array of memory for zeroing
 * @param item_size size in bytes of each array item
 * @param index_start item index to start zeroing (inclusive)
 * @param index_end item index to stop zeroing (exclusive)
 */
BOOL memory_zero(void *array, size_t item_size, size_t index_start, size_t index_end)
{
  int code = 0;
  if (NULL == array || 0 == item_size || index_start >= index_end)
  {
    code = -1;
  }
  else if (memset((BYTE *)array + index_start * item_size, 0, (index_end - index_start) * item_size), 0)
  {
    // impossible due to comma-spliced 0
  }

  if (0 == code)
  {
    code = OK;
  }
  else
  {
    code = ERROR;
  }
  return code;
}

/**
 * Copies bytes from `index_start` inclusive to `index_end` exclusive of `array` into `into_destination`.
 * Returns ERROR if `item_size` is 0 or `index_start` >= `index_end`.
 * @param array pointer to array of memory for clearing
 * @param item_size size in bytes of each item
 * @param index_start item index to start clearing (inclusive)
 * @param index_end item index to stop clearing (exclusive)
 */
BOOL memory_copy_from(void *array, size_t item_size, size_t index_start, size_t index_end, void *into_destination)
{
  int code = 0;
  if (NULL == array || NULL == into_destination || 0 == item_size || index_end <= index_start)
  {
    code = -1;
  }
  else if (memcpy(into_destination, (BYTE *)array + index_start * item_size, (index_end - index_start) * item_size), 0)
  {
    // impossible due to comma-spliced 0
  }
  else if (0 != memcmp(into_destination, (BYTE *)array + index_start * item_size, (index_end - index_start) * item_size))
  {
    code = -2;
  }

  if (0 == code)
  {
    code = OK;
  }
  else
  {
    code = ERROR;
  }
  return code;
}

/**
 * Copies bytes from `index_start` inclusive to `index_end` exclusive of `array` into `from_destination`.
 * Returns ERROR if `item_size` is 0 or `index_start` >= `index_end`.
 * @param array pointer to array of memory for clearing
 * @param item_size size in bytes of each item
 * @param index_start item index to start clearing (inclusive)
 * @param index_end item index to stop clearing (exclusive)
 */
BOOL memory_copy_into(void *array, size_t item_size, size_t index_start, size_t index_end, void *from_destination)
{
  int code = 0;
  if (NULL == array || NULL == from_destination || 0 == item_size || index_end <= index_start)
  {
    code = -1;
  }
  else if (memcpy((BYTE *)array + index_start * item_size, from_destination, (index_end - index_start) * item_size), 0)
  {
    // impossible due to comma-spliced 0
  }
  else if (0 != memcmp((BYTE *)array + index_start * item_size, from_destination, (index_end - index_start) * item_size))
  {
    code = -2;
  }

  if (0 == code)
  {
    code = OK;
  }
  else
  {
    code = ERROR;
  }
  return code;
}

/**
 * Compares two wide strings for equality.
 */
BOOL wcs_equals(wchar_t *str0, wchar_t *str1)
{
  int code = 0;
  if (NULL == str0 || NULL == str1)
  {
    code = -1;
  }
  else if (0 != wcscmp(str0, str1))
  {
    code = -2;
  }

  if (0 == code)
  {
    code = TRUE;
  }
  else
  {
    code = FALSE;
  }
  return code;
}

/**
 * Concats two wide strings and returns pointer to newly allocated memory
 * containing resulting string.
 * Use `free` to release the memory.
 */
wchar_t *wcs_concat(wchar_t *str0, wchar_t *str1)
{
  size_t len0 = 0;
  size_t len1 = 0;
  wchar_t *result = NULL;

  int code = 0;
  if (NULL == str0 || NULL == str1)
  {
    code = -1;
  }
  else if (len0 = wcslen(str0), len1 = wcslen(str1), 0 == len0 || 0 == len1)
  {
    code = -2;
  }
  else if (result = calloc(len0 + len1 + 1, sizeof(wchar_t)), NULL == result)
  {
    code = -3;
  }
  else
  {
    memcpy(result, str0, len0 * sizeof(wchar_t));
    memcpy(result + len0, str1, len1 * sizeof(wchar_t));
  }

  return result;
}

#endif /* _Lib_Standard_h_ */
