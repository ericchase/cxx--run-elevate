#include "../Headers.h"
#include "../Lib.Standard.h"

#include "Test.h"

void Test_Lib_Standard()
{
  label("memory_zero");
  {
    int nums[4] = {1, 2, 3, 4};

    check("(source = NULL) should return ERROR", ERROR == (memory_zero(NULL, sizeof(int), 0, 4)));
    check("(item_size = 0) should return ERROR", ERROR == (memory_zero(nums, 0, 0, 4)));
    check("(index_end <= index_start) should return ERROR", ERROR == (memory_zero(nums, sizeof(int), 0, 0)));

    check("nums[0] should be 1", 1 == nums[0]);
    check("nums[1] should be 2", 2 == nums[1]);
    check("nums[2] should be 3", 3 == nums[2]);
    check("nums[3] should be 4", 4 == nums[3]);
    check("successful memory_zero should return OK", OK == (memory_zero(nums, sizeof(int), 0, 4)));
    check("nums[0] should be 0", 0 == nums[0]);
    check("nums[1] should be 0", 0 == nums[1]);
    check("nums[2] should be 0", 0 == nums[2]);
    check("nums[3] should be 0", 0 == nums[3]);
  }
  {
    int nums[4] = {1, 2, 3, 4};

    check("nums[0] should be 1", 1 == nums[0]);
    check("nums[1] should be 2", 2 == nums[1]);
    check("nums[2] should be 3", 3 == nums[2]);
    check("nums[3] should be 4", 4 == nums[3]);

    check("clear array[0]", OK == (memory_zero(nums, sizeof(int), 0, 1)));
    check("nums[0] should be 0", 0 == nums[0]);
    check("nums[1] should be 2", 2 == nums[1]);
    check("nums[2] should be 3", 3 == nums[2]);
    check("nums[3] should be 4", 4 == nums[3]);
    check("clear array[2] and array[3]", OK == (memory_zero(nums, sizeof(int), 2, 4)));
    check("nums[0] should be 0", 0 == nums[0]);
    check("nums[1] should be 2", 2 == nums[1]);
    check("nums[2] should be 0", 0 == nums[2]);
    check("nums[3] should be 0", 0 == nums[3]);
    check("clear all", OK == (memory_zero(nums, sizeof(int), 0, 4)));
    check("nums[0] should be 0", 0 == nums[0]);
    check("nums[1] should be 0", 0 == nums[1]);
    check("nums[2] should be 0", 0 == nums[2]);
    check("nums[3] should be 0", 0 == nums[3]);
  }

  label("memory_copy_from");
  {
    int nums1[4] = {1, 2, 3, 4};
    int nums2[4] = {0, 0, 0, 0};

    check("(source = NULL) should return ERROR", ERROR == (memory_copy_from(NULL, sizeof(int), 0, 4, nums2)));
    check("(item_size = 0) should return ERROR", ERROR == (memory_copy_from(nums1, 0, 0, 4, nums2)));
    check("(index_end <= index_start) should return ERROR", ERROR == (memory_copy_from(nums1, sizeof(int), 0, 0, nums2)));
    check("(destination = NULL) should return ERROR", ERROR == (memory_copy_from(nums1, sizeof(int), 0, 4, NULL)));

    check("nums2[0] should be 0", 0 == nums2[0]);
    check("nums2[1] should be 0", 0 == nums2[1]);
    check("nums2[2] should be 0", 0 == nums2[2]);
    check("nums2[3] should be 0", 0 == nums2[3]);
    check("successful memory_copy_from should return OK", OK == (memory_copy_from(nums1, sizeof(int), 0, 4, nums2)));
    check("nums2[0] should be 1", 1 == nums2[0]);
    check("nums2[1] should be 2", 2 == nums2[1]);
    check("nums2[2] should be 3", 3 == nums2[2]);
    check("nums2[3] should be 4", 4 == nums2[3]);
  }

  label("memory_copy_into");
  {
    int nums1[4] = {1, 2, 3, 4};
    int nums2[4] = {0, 0, 0, 0};

    check("(source = NULL) should return ERROR", ERROR == (memory_copy_into(NULL, sizeof(int), 0, 4, nums2)));
    check("(item_size = 0) should return ERROR", ERROR == (memory_copy_into(nums1, 0, 0, 4, nums2)));
    check("(index_end <= index_start) should return ERROR", ERROR == (memory_copy_into(nums1, sizeof(int), 0, 0, nums2)));
    check("(destination = NULL) should return ERROR", ERROR == (memory_copy_into(nums1, sizeof(int), 0, 4, NULL)));

    check("nums1[0] should be 0", 1 == nums1[0]);
    check("nums1[1] should be 0", 2 == nums1[1]);
    check("nums1[2] should be 0", 3 == nums1[2]);
    check("nums1[3] should be 0", 4 == nums1[3]);
    check("successful memory_copy_into should return OK", OK == (memory_copy_into(nums1, sizeof(int), 0, 4, nums2)));
    check("nums1[0] should be 1", 0 == nums1[0]);
    check("nums1[1] should be 2", 0 == nums1[1]);
    check("nums1[2] should be 3", 0 == nums1[2]);
    check("nums1[3] should be 4", 0 == nums1[3]);
  }

  label("wcs_equals");
  {
    check("('str0' == NULL) should return FALSE", FALSE == wcs_equals(NULL, NULL));
    check("('str1' == NULL) should return FALSE", FALSE == wcs_equals(L"abc", NULL));
    check("('str0' != 'str1') should return FALSE", FALSE == wcs_equals(L"abc", L"def"));
    check("('str0' == 'str1') should return TRUE", TRUE == wcs_equals(L"abc", L"abc"));
  }

  label("wcs_concat");
  {
    check("('str0' == NULL) should return NULL", NULL == wcs_concat(NULL, NULL));
    check("('str1' == NULL) should return NULL", NULL == wcs_concat(L"abc", NULL));
    wchar_t *wstr = wcs_concat(L"abc", L"def");
    check("should return concatenation", wcs_equals(wstr, L"abcdef"));
    free(wstr);
  }
}
