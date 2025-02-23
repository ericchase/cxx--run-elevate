#include "../Headers.h"
#include "../Lib.Standard.h"
#include "../Lib.PointerManager.h"

#include "Test.h"

void Test_Lib_PointerManager()
{
  label("PointerManager Basic");
  {
    pPointerManager manager = NULL;
    wchar_t *pointer1 = calloc(2, sizeof(wchar_t));
    pointer1[0] = L'1';

    label("PointerManager_New");
    check("(init_length = 0) should return NULL", NULL == PointerManager_New(0));
    manager = PointerManager_New(1);
    check("(init_length = 1) should not return NULL", NULL != manager);
    if (NULL != manager)
    {
      check("(init_length = 1) 'array' should not be NULL", NULL != manager->array);
      check("(init_length = 1) 'array'[0] should be NULL", NULL == manager->array[0]);
      check("(init_length = 1) 'length' should be 1", 1 == manager->length);
      check("(init_length = 1) 'item_count' should be 0", 0 == manager->item_count);

      label("PointerManager_AddPointer");
      check("(container = NULL, pointer = NULL) should return ERROR", ERROR == PointerManager_AddPointer(NULL, NULL));
      check("(container = NULL, valid pointer) should return ERROR", ERROR == PointerManager_AddPointer(NULL, pointer1));
      check("(valid container, pointer = NULL) should return ERROR", ERROR == PointerManager_AddPointer(manager, NULL));
      check("(init_length = 1) should return OK", OK == PointerManager_AddPointer(manager, pointer1));
      check("(init_length = 1) 'item_count' should be 1", 1 == manager->item_count);
      check("(init_length = 1) 'array'[0] should not be NULL", NULL != manager->array[0]);
      check("(init_length = 1) 'array'[0][0] should be L'1'", L'1' == ((wchar_t *)manager->array[0])[0]);
      check("(init_length = 1) 'array'[0][1] should be null-terminated", L'\0' == ((wchar_t *)manager->array[0])[1]);

      label("PointerManager_DeletePointers");
      check("(container = NULL) should return ERROR", ERROR == PointerManager_DeletePointers(NULL));
      check("(init_length = 1) should return OK", OK == PointerManager_DeletePointers(manager));
      check("(init_length = 1) 'array'[0] should be NULL", NULL == manager->array[0]);
      check("(init_length = 1) 'item_count' should be 0", 0 == manager->item_count);
    }

    label("PointerManager_Delete");
    check("(container_reference = NULL) should return ERROR", ERROR == PointerManager_Delete(NULL));
    pPointerManager unallocated_manager = NULL;
    check("((*container_reference) = NULL) should return ERROR", ERROR == PointerManager_Delete(&unallocated_manager));
    check("(init_length = 1) should return OK", OK == PointerManager_Delete(&manager));
    check("(init_length = 1) should be NULL", NULL == manager);
  }

  label("PointerManager Advanced");
  {
    pPointerManager manager = PointerManager_New(2);
    wchar_t *pointer1 = calloc(2, sizeof(wchar_t));
    wchar_t *pointer2 = calloc(2, sizeof(wchar_t));
    wchar_t *pointer3 = calloc(2, sizeof(wchar_t));
    pointer1[0] = L'1';
    pointer2[0] = L'2';
    pointer3[0] = L'3';

    if (NULL != manager && NULL != manager->array)
    {
      label("PointerManager_New");
      check("(init_length = 2) 'length' should be 2", 2 == manager->length);
      check("(init_length = 2) 'item_count' should be 0", 0 == manager->item_count);
      check("(init_length = 2) 'array'[0] should be NULL", NULL == manager->array[0]);
      check("(init_length = 2) 'array'[1] should be NULL", NULL == manager->array[1]);

      label("PointerManager_AddPointer");

      check("AddPointer(pointer1) should return OK", OK == PointerManager_AddPointer(manager, pointer1));
      check("(init_length = 2) 'length' should be 2", 2 == manager->length);
      check("(init_length = 2) 'item_count' should be 1", 1 == manager->item_count);
      check("(init_length = 2) 'array'[0] should not be NULL", NULL != manager->array[0]);
      check("(init_length = 2) 'array'[1] should be NULL", NULL == manager->array[1]);

      check("AddPointer(pointer2) should return OK", OK == PointerManager_AddPointer(manager, pointer2));
      check("(init_length = 2) 'length' should be 2", 2 == manager->length);
      check("(init_length = 2) 'item_count' should be 2", 2 == manager->item_count);
      check("(init_length = 2) 'array'[0] should not be NULL", NULL != manager->array[0]);
      check("(init_length = 2) 'array'[1] should not be NULL", NULL != manager->array[1]);

      check("AddPointer(pointer3) should return OK", OK == PointerManager_AddPointer(manager, pointer3));
      check("(init_length = 2) 'length' should be 4", 4 == manager->length);
      check("(init_length = 2) 'item_count' should be 3", 3 == manager->item_count);
      check("(init_length = 2) 'array'[0] should not be NULL", NULL != manager->array[0]);
      check("(init_length = 2) 'array'[1] should not be NULL", NULL != manager->array[1]);
      check("(init_length = 2) 'array'[2] should not be NULL", NULL != manager->array[2]);
      check("(init_length = 2) 'array'[3] should be NULL", NULL == manager->array[3]);

      label("PointerManager_ReleasePointers");
      check("(container = NULL) should return NULL", NULL == PointerManager_ReleasePointers(NULL));
      wchar_t **pointers = (wchar_t **)PointerManager_ReleasePointers(manager);
      check("(init_length = 2) should return array address", NULL != pointers);
      if (NULL != pointers)
      {
        check("(init_length = 2) 'array' should be refreshed", NULL != manager->array);
        check("(init_length = 2) 'length' should be 4", 4 == manager->length);
        check("(init_length = 2) 'item_count' should be 0", 0 == manager->item_count);
        check("(init_length = 2) 'array'[0] should be NULL", NULL == manager->array[0]);
        check("(init_length = 2) 'array'[1] should be NULL", NULL == manager->array[1]);
        check("(init_length = 2) 'array'[2] should be NULL", NULL == manager->array[2]);
        check("(init_length = 2) 'array'[3] should be NULL", NULL == manager->array[3]);
        check("(init_length = 2) pointers[0] should not be NULL", NULL != pointers[0]);
        check("(init_length = 2) pointers[1] should not be NULL", NULL != pointers[1]);
        check("(init_length = 2) pointers[2] should not be NULL", NULL != pointers[2]);
        check("(init_length = 2) pointer1[0] should be L'1'", L'1' == pointer1[0]);
        check("(init_length = 2) pointer2[0] should be L'2'", L'2' == pointer2[0]);
        check("(init_length = 2) pointer3[0] should be L'3'", L'3' == pointer3[0]);

        label("PointerManager_Delete");
        check("(init_length = 2) should return OK", OK == (PointerManager_Delete(&manager)));
        check("(init_length = 2) should be NULL", NULL == (manager));

        check("(init_length = 2) pointers[0] should not be NULL", NULL != pointers[0]);
        check("(init_length = 2) pointers[1] should not be NULL", NULL != pointers[1]);
        check("(init_length = 2) pointers[2] should not be NULL", NULL != pointers[2]);
        check("(init_length = 2) pointer1[0] should be L'1'", L'1' == pointer1[0]);
        check("(init_length = 2) pointer2[0] should be L'2'", L'2' == pointer2[0]);
        check("(init_length = 2) pointer3[0] should be L'3'", L'3' == pointer3[0]);

        free(pointers);

        free(pointer1);
        free(pointer2);
        free(pointer3);
      }
    }
  }
}
