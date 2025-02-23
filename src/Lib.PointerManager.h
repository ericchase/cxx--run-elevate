#ifndef _Lib_PointerManager_h_
#define _Lib_PointerManager_h_

#include "Headers.h"

#include "Lib.Standard.h"

typedef void *Pointer;

typedef struct PointerManager
{
  Pointer *array;
  size_t length;
  size_t item_count;
} PointerManager, *pPointerManager;

pPointerManager PointerManager_New(size_t init_length)
{
  if (0 == init_length)
  {
    return NULL;
  }

  pPointerManager container = calloc(1, sizeof(PointerManager));
  if (NULL == container)
  {
    return NULL;
  }

  Pointer *array_pointer = calloc(init_length, sizeof(Pointer));
  if (NULL == array_pointer)
  {
    return NULL;
  }

  container->array = array_pointer;
  container->length = init_length;
  container->item_count = 0;
  return container;
}

BOOL PointerManager_DeletePointers(pPointerManager container)
{
  if (NULL == container)
  {
    return ERROR;
  }

  if (NULL != container->array)
  {
    // Free each pointer.
    for (size_t index = 0; index < container->length; ++index)
    {
      memory_free(&((void *)(container->array)[index]));
    }
    container->item_count = 0;
  }

  return OK;
}

BOOL PointerManager_Delete(pPointerManager *container_reference)
{
  if (NULL == container_reference)
  {
    return ERROR;
  }

  if (ERROR == PointerManager_DeletePointers((*container_reference)))
  {
    return ERROR;
  }

  memory_free(&((void *)(*container_reference)->array));
  memory_free(container_reference);

  return OK;
}

BOOL PointerManager_ExpandIfFull(pPointerManager container)
{
  if (NULL == container)
  {
    return ERROR;
  }

  if (container->item_count >= container->length)
  {
    if (NULL == container->array || 0 == container->length)
    {
      PointerManager_DeletePointers(container);

      Pointer *array_pointer = realloc(container->array, 4 * sizeof(Pointer));
      if (NULL == array_pointer)
      {
        return ERROR;
      }

      memory_zero(array_pointer, sizeof(Pointer), 0, 4);
      container->array = array_pointer;
      container->length = 4;
      container->item_count = 0;
    }
    else
    {
      Pointer *array_pointer = realloc(container->array, container->length * 2 * sizeof(Pointer));
      if (NULL == array_pointer)
      {
        return ERROR;
      }

      memory_zero(array_pointer, sizeof(Pointer), container->length, container->length * 2);
      container->array = array_pointer;
      container->length = container->length * 2;
    }
  }
  return OK;
}

BOOL PointerManager_AddPointer(pPointerManager container, Pointer pointer)
{
  if (NULL == pointer)
  {
    return ERROR;
  }

  if (ERROR == PointerManager_ExpandIfFull(container))
  {
    return ERROR;
  }

  container->array[container->item_count] = pointer;
  container->item_count += 1;

  return OK;
}

/**
 * Sets `array` to `NULL` without freeing it or its items.
 * Sets `length` to `0`.
 * Sets `item_count` to `0`.
 * Returns array
 */
Pointer *PointerManager_ReleasePointers(pPointerManager container)
{
  if (NULL == container)
  {
    return NULL;
  }

  Pointer *array = container->array;

  container->array = NULL;
  container->length = 0;
  container->item_count = 0;

  PointerManager_ExpandIfFull(container);

  return array;
}

#endif /* _Lib_PointerManager_h_ */
