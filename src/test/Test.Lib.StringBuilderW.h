#include "../Headers.h"
#include "../Lib.Standard.h"
#include "../Lib.StringBuilderW.h"

#include "Test.h"

void Test_Lib_StringBuilderW(void)
{
  label("StringBuilderW");
  {
    pStringBuilderW builder = NULL;

    label("StringBuilderW_From");
    check("NULL pointer source should return NULL", NULL == StringBuilderW_From(NULL));
    check("0 length source should return NULL", NULL == StringBuilderW_From(L""));
    builder = StringBuilderW_From(L"0123456789");
    check("'builder' should not be NULL", NULL != builder);
    if (NULL != builder)
    {
      check("'array' should not be NULL", NULL != builder->array);
      check("'array_length' should be 11", 11 == builder->array_length);
      check("'string_start' should be 0", 0 == builder->string_start);
      check("'string_end' should be 10", 10 == builder->string_end);

      label("StringBuilderW_Equals");
      check("(container = NULL) should return FALSE", FALSE == StringBuilderW_Equals(NULL, NULL));
      check("(compare_to = NULL) should return FALSE", FALSE == StringBuilderW_Equals(builder, NULL));
      check("should return TRUE if strings are equal", TRUE == StringBuilderW_Equals(builder, L"0123456789"));
      check("should return FALSE if strings are different", FALSE == StringBuilderW_Equals(builder, L"abcdef"));

      label("StringBuilderW_Append");
      check("(container = NULL) should return FALSE", FALSE == StringBuilderW_Append(NULL, NULL));
      check("(to_append = NULL) should return FALSE", FALSE == StringBuilderW_Append(builder, NULL));
      check("should return OK", TRUE == StringBuilderW_Append(builder, L"abcdef"));
      check("'string' should equal '0123456789abcdef'", TRUE == StringBuilderW_Equals(builder, L"0123456789abcdef"));

      label("StringBuilderW_Delete");
      check("(container_reference = NULL) should return ERROR", ERROR == StringBuilderW_Delete(NULL));
      pStringBuilderW unallocated_builder = NULL;
      check("((*container_reference) = NULL) should return ERROR", ERROR == StringBuilderW_Delete(&unallocated_builder));
      check("should return OK", OK == StringBuilderW_Delete(&builder));
      check("should be NULL", NULL == builder);
    }

    label("StringBuilderW_GetString");
    check("(container = NULL) should return NULL", NULL == StringBuilderW_GetString(NULL));
    builder = StringBuilderW_From(L"abc");
    check("builder should not be NULL", NULL != builder);
    if (NULL != builder)
    {
      wchar_t *str = StringBuilderW_GetString(builder);
      StringBuilderW_Delete(&builder);
      check("returned string should not be NULL", NULL != str);
      check("returned string should be 'abc'", NULL != str && 0 == wcscmp(L"abc", str));
      memory_free(&str);
    }

    label("StringBuilderW_New");
    check("(init_length = 0) should return NULL", NULL == StringBuilderW_New(0));
    builder = StringBuilderW_New(1);
    check("builder should not be NULL", NULL != builder);
    if (NULL != builder)
    {
      check("'array' should not be NULL", NULL != builder->array);
      check("'array_length' should be 2", 2 == builder->array_length);
      check("'string_start' should be 0", 0 == builder->string_start);
      check("'string_end' should be 0", 0 == builder->string_end);
      StringBuilderW_Append(builder, L"1");
      check("'string' should equal '1'", TRUE == StringBuilderW_Equals(builder, L"1"));
      check("'array_length' should be 2", 2 == builder->array_length);
      check("'string_start' should be 0", 0 == builder->string_start);
      check("'string_end' should be 1", 1 == builder->string_end);
      StringBuilderW_Delete(&builder);
    }
  }
  {
    pStringBuilderW builder = StringBuilderW_From(L"   \"\"\"abc\"\"\"   ");

    label("StringBuilderW_Trim");
    check("(container = NULL) should return ERROR", ERROR == StringBuilderW_Trim(NULL, L' '));
    check("(array = '   \"\"\"abc\"\"\"   ') should return OK", OK == StringBuilderW_Trim(builder, L' '));
    check("(array = '   \"\"\"abc\"\"\"   ') 'string' should be '\"\"\"abc\"\"\"'", TRUE == StringBuilderW_Equals(builder, L"\"\"\"abc\"\"\""));
    check("(array = '   \"\"\"abc\"\"\"   ') should return OK", OK == StringBuilderW_Trim(builder, L'"'));
    check("(array = '   \"\"\"abc\"\"\"   ') 'string' should be 'abc'", TRUE == StringBuilderW_Equals(builder, L"abc"));

    StringBuilderW_Delete(&builder);
  }
  {
    pStringBuilderW builder = StringBuilderW_From(L"   \"\"\"abc\"\"\"   ");

    label("StringBuilderW_TrimListInOrder");
    check("(container = NULL) should return ERROR", ERROR == StringBuilderW_TrimListInOrder(NULL, NULL));
    check("(to_trim = NULL) should return ERROR", ERROR == StringBuilderW_TrimListInOrder(builder, NULL));
    check("(array = '   \"\"\"abc\"\"\"   ') should return OK", OK == StringBuilderW_TrimListInOrder(builder, L" \""));
    check("(array = '   \"\"\"abc\"\"\"   ') 'string' should be 'abc'", TRUE == StringBuilderW_Equals(builder, L"abc"));

    StringBuilderW_Delete(&builder);
  }
}
