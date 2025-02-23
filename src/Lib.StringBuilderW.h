#ifndef _Lib_StringBuilderW_h_
#define _Lib_StringBuilderW_h_

#include "Headers.h"

#include "Lib.Standard.h"

/**
 * The members `string` and `string_length` do not include a null terminator.
 * Use `StringBuilderW_GetString` to produce a final, null terminated string
 * from the builder.
 */
typedef struct StringBuilderW
{
  wchar_t *array;
  size_t array_length;
  size_t string_start;
  size_t string_end;
  // wchar_t *string;
  // size_t string_length;
} StringBuilderW, *pStringBuilderW;

pStringBuilderW StringBuilderW_New(size_t init_length)
{
  pStringBuilderW container = NULL;
  wchar_t *array_pointer = NULL;

  int code = 0;
  if (0 == init_length)
  {
    code = -1;
  }
  else if (container = calloc(1, sizeof(StringBuilderW)), NULL == container)
  {
    code = -2;
  }
  else if (array_pointer = calloc(init_length + 1, sizeof(wchar_t)), NULL == array_pointer)
  {
    code = -3;
  }
  else
  {
    container->array = array_pointer;
    container->array_length = init_length + 1;
    container->string_start = 0;
    container->string_end = 0;
  }

  if (0 != code)
  {
    memory_free(&((void *)array_pointer));
    memory_free(&((void *)container));
  }

  return container;
}

pStringBuilderW StringBuilderW_From(wchar_t *copy_from)
{
  size_t copy_from_length = 0;
  pStringBuilderW container = NULL;
  wchar_t *array_pointer = NULL;

  int code = 0;
  if (NULL == copy_from)
  {
    code = -1;
  }
  else if (copy_from_length = wcslen(copy_from), 0 == copy_from_length)
  {
    code = -2;
  }
  else if (container = calloc(1, sizeof(StringBuilderW)), NULL == container)
  {
    code = -3;
  }
  else if (array_pointer = calloc(copy_from_length + 1, sizeof(wchar_t)), NULL == array_pointer)
  {
    code = -4;
  }
  else
  {
    memcpy(array_pointer, copy_from, copy_from_length * sizeof(wchar_t));
    container->array = array_pointer;
    container->array_length = copy_from_length + 1;
    container->string_start = 0;
    container->string_end = copy_from_length;
  }

  if (0 != code)
  {
    memory_free(&((void *)array_pointer));
    memory_free(&((void *)container));
  }

  return container;
}

BOOL StringBuilderW_Delete(pStringBuilderW *container_reference)
{
  int code = 0;
  if (NULL == container_reference)
  {
    code = -1;
  }
  else if (NULL == (*container_reference))
  {
    code = -2;
  }
  else
  {
    (*container_reference)->array_length = 0;
    (*container_reference)->string_start = 0;
    (*container_reference)->string_end = 0;
    memory_free(&((void *)(*container_reference)->array));
    memory_free(container_reference);
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

BOOL StringBuilderW_Expand(pStringBuilderW container, size_t by_length)
{
  wchar_t *array_pointer = NULL;

  int code = 0;
  if (NULL == container)
  {
    code = -1;
  }
  else if (0 == by_length)
  {
    code = -2;
  }
  else if (array_pointer = realloc(container->array, (container->array_length + by_length) * sizeof(wchar_t)), NULL == array_pointer)
  {
    code = -3;
  }
  else
  {
    // clear only newly allocated data
    memory_zero(array_pointer, sizeof(wchar_t), container->array_length, container->array_length + by_length);
    container->array = array_pointer;
    container->array_length = container->array_length + by_length;
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

BOOL StringBuilderW_Append(pStringBuilderW container, wchar_t *to_append)
{
  wchar_t *array_pointer = NULL;
  size_t to_append_length = 0;

  int code = 0;
  if (NULL == container)
  {
    code = -1;
  }
  else if (NULL == to_append)
  {
    code = -2;
  }
  else if (to_append_length = wcslen(to_append), 0 == to_append_length)
  {
    code = -3;
  }
  else if (container->string_end + to_append_length < container->array_length)
  {
    // don't need to realloc array, break chain here
    // not an error
  }
  else if (array_pointer = realloc(container->array, (container->string_end + to_append_length + 1) * sizeof(wchar_t)), NULL == array_pointer)
  {
    code = -4;
  }
  else
  {
    memory_zero(array_pointer, sizeof(wchar_t), container->array_length, container->string_end + to_append_length + 1);
    container->array = array_pointer;
    container->array_length = container->string_end + to_append_length + 1;
  }

  if (0 == code)
  {
    memory_copy_into(container->array, sizeof(wchar_t), container->string_end, container->string_end + to_append_length, to_append);
    container->string_end = container->string_end + to_append_length;
    code = OK;
  }
  else
  {
    code = ERROR;
  }
  return code;
}

wchar_t *StringBuilderW_GetString(pStringBuilderW container)
{
  wchar_t *string_pointer = NULL;

  int code = 0;
  if (NULL == container)
  {
    code = -1;
  }
  else if (1 > container->string_end - container->string_start)
  {
    code = -2;
  }
  else if (string_pointer = calloc(container->string_end - container->string_start + 1, sizeof(wchar_t)), NULL == string_pointer)
  {
    code = -3;
  }
  else
  {
    memory_copy_from(container->array, sizeof(wchar_t), container->string_start, container->string_end, string_pointer);
  }

  return string_pointer;
}

// Utility Functions

BOOL StringBuilderW_Equals(pStringBuilderW container, wchar_t *compare_to)
{
  size_t compare_to_length = 0;

  int code = 0;
  if (NULL == container)
  {
    code = -1;
  }
  else if (NULL == compare_to)
  {
    code = -2;
  }
  else if (compare_to_length = wcslen(compare_to), 0 == compare_to_length)
  {
    code = -3;
  }
  else
  {
    size_t index_compare_to = 0;
    size_t index_container = container->string_start;
    while (index_compare_to < compare_to_length && index_container < container->string_end)
    {
      if (compare_to[index_compare_to] != container->array[index_container])
      {
        code = -4;
        break;
      }
      ++index_compare_to;
      ++index_container;
    }
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

BOOL StringBuilderW_Trim(pStringBuilderW container, wchar_t to_trim)
{
  int code = 0;
  if (NULL == container)
  {
    code = -1;
  }
  else
  {
    // trim start
    while (container->string_start < container->string_end && to_trim == container->array[container->string_start])
    {
      ++(container->string_start);
    }
    // trim end
    while (container->string_start < container->string_end && to_trim == container->array[container->string_end - 1])
    {
      --(container->string_end);
    }
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

BOOL StringBuilderW_TrimListInOrder(pStringBuilderW container, wchar_t *to_trim)
{
  int code = 0;
  if (NULL == container)
  {
    code = -1;
  }
  else if (NULL == to_trim)
  {
    code = -2;
  }
  else
  {
    // trim start
    for (size_t index_to_trim = 0; L'\0' != to_trim[index_to_trim]; ++index_to_trim)
    {
      while (container->string_start < container->string_end && to_trim[index_to_trim] == container->array[container->string_start])
      {
        ++(container->string_start);
      }
    }
    // trim end
    for (size_t index_to_trim = 0; L'\0' != to_trim[index_to_trim]; ++index_to_trim)
    {
      while (container->string_start < container->string_end && to_trim[index_to_trim] == container->array[container->string_end - 1])
      {
        --(container->string_end);
      }
    }
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

#endif /* _Lib_StringBuilderW_h_ */
