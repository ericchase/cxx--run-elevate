## C Api

### malloc

```c
#include <stdlib.h>

void *malloc( size_t size );
```

Allocates size bytes of uninitialized storage.

If allocation succeeds, returns a pointer that is suitably aligned for any object type with fundamental alignment.

**If size is zero, the behavior of malloc is implementation-defined.** For example, a null pointer may be returned. Alternatively, a non-null pointer may be returned; but such a pointer should not be dereferenced, and should be passed to free to avoid memory leaks.

> Parameters

- size - number of bytes to allocate

> Return value

On success, returns the pointer to the beginning of newly allocated memory. **To avoid a memory leak, the returned pointer must be deallocated with free() or realloc().**

On failure, returns a null pointer.

---

### calloc

```c
#include <stdlib.h>

void* calloc( size_t num, size_t size );
```

Allocates memory for an array of num objects of size and initializes all bytes in the allocated storage to zero.

If allocation succeeds, returns a pointer to the lowest (first) byte in the allocated memory block that is suitably aligned for any object type with fundamental alignment.

**If size is zero, the behavior is implementation defined** (null pointer may be returned, or some non-null pointer may be returned that may not be used to access storage).

> Parameters

- num - number of objects
- size - size of each object

> Return value

On success, returns the pointer to the beginning of newly allocated memory. **To avoid a memory leak, the returned pointer must be deallocated with free() or realloc().**

On failure, returns a null pointer.

---

### realloc

```c
#include <stdlib.h>

void *realloc( void *ptr, size_t new_size );
```

Reallocates the given area of memory. If ptr is not NULL, it must be previously allocated by malloc, calloc or realloc and not yet freed with a call to free or realloc. Otherwise, the results are undefined.

The reallocation is done by either:

- a) expanding or contracting the existing area pointed to by ptr, if possible. The contents of the area remain unchanged up to the lesser of the new and old sizes. If the area is expanded, the contents of the new part of the array are undefined.
- b) allocating a new memory block of size new_size bytes, copying memory area with size equal the lesser of the new and the old sizes, and freeing the old block.

**If there is not enough memory, the old memory block is not freed and null pointer is returned.**

If ptr is NULL, the behavior is the same as calling malloc(new_size).

**If new_size is zero, the behavior of realloc is either implementation-defined or undefined.**

> Parameters

- ptr - pointer to the memory area to be reallocated
- new_size - new size of the array in bytes

> Return value

On success, returns the pointer to the beginning of newly allocated memory. To avoid a memory leak, the returned pointer must be deallocated with free or realloc. The original pointer ptr is invalidated and any access to it is undefined behavior (even if reallocation was in-place).

On failure, returns a null pointer. The original pointer ptr remains valid and may need to be deallocated with free or realloc.

---

### free

```c
#include <stdlib.h>

void free( void *ptr );
```

Deallocates the space previously allocated by malloc(), calloc(), aligned_alloc(),(since C11) or realloc().

If ptr is a null pointer, the function does nothing.

The behavior is undefined if the value of ptr does not equal a value returned earlier by malloc(), calloc(), realloc(), or aligned_alloc()(since C11).

The behavior is undefined if the memory area referred to by ptr has already been deallocated, that is, free(), free_sized(), free_aligned_sized()(since C23), or realloc() has already been called with ptr as the argument and no calls to malloc(), calloc(), realloc(), or aligned_alloc()(since C11) resulted in a pointer equal to ptr afterwards.

The behavior is undefined if after free() returns, an access is made through the pointer ptr (unless another allocation function happened to result in a pointer value equal to ptr).

> Parameters

- ptr - pointer to the memory to deallocate

> Return value

(none)

---

### memcpy

```c
#include <string.h>

void* memcpy( void *dest, const void *src, size_t count ); // (until C99)
void* memcpy( void *restrict dest, const void *restrict src, size_t count ); // (since C99)
```

Copies count characters from the object pointed to by src to the object pointed to by dest. Both objects are interpreted as arrays of unsigned char.

**The behavior is undefined if access occurs beyond the end of the dest array. If the objects overlap (which is a violation of the restrict contract)(since C99), the behavior is undefined. The behavior is undefined if either dest or src is an invalid or null pointer.**

> Parameters

- dest - pointer to the object to copy to
- src - pointer to the object to copy from
- count - number of bytes to copy

> Return value

- Returns a copy of dest

> Notes

memcpy may be used to set the effective type of an object obtained by an allocation function.

memcpy is the fastest library routine for memory-to-memory copy. It is usually more efficient than strcpy, which must scan the data it copies or memmove, which must take precautions to handle overlapping inputs.

Several C compilers transform suitable memory-copying loops to memcpy calls.

## Where strict aliasing prohibits examining the same memory as values of two different types, memcpy may be used to convert the values.

### memset

```c
#include <string.h>

void *memset( void *dest, int ch, size_t count );
```

Copies the value (unsigned char)ch into each of the first count characters of the object pointed to by dest. **The behavior is undefined if access occurs beyond the end of the dest array. The behavior is undefined if dest is a null pointer.**

> Parameters

- dest - pointer to the object to fill
- ch - fill byte
- count - number of bytes to fill
- destsz - size of the destination array

> Return value

- A copy of dest

> Notes

memset may be optimized away (under the as-if rules) if the object modified by this function is not accessed again for the rest of its lifetime (e.g., gcc bug 8537). For that reason, this function cannot be used to scrub memory (e.g., to fill an array that stored a password with zeroes).

---

### wcschr

```c
#include <wchar.h>

wchar_t *wcschr( const wchar_t *str, wchar_t ch );
```

Finds the first occurrence of the wide character ch in the wide string pointed to by str.

> Parameters

- str - pointer to the null-terminated wide string to be analyzed
- ch - wide character to search for

> Return value

Pointer to the found character in str, or a null pointer if no such character is found.

---

### wcsstr

```c
#include <wchar.h>

wchar_t *wcsstr( const wchar_t *dest, const wchar_t *src );
```

Finds the first occurrence of the wide string src in the wide string pointed to by dest. The terminating null characters are not compared.

> Parameters

- dest - pointer to the null-terminated wide string to examine
- src - pointer to the null-terminated wide string to search for

> Return value

Pointer to the first character of the found substring in dest, or a null pointer if no such substring is found. If src points to an empty string, dest is returned.

---

### wcslen

```c
#include <wchar.h>

size_t wcslen( const wchar_t *str ); // (1) (since C95)

size_t wcsnlen_s(const wchar_t *str, size_t strsz); // (2) (since C11)
```

(1) Returns the length of a wide string, that is the number of non-null wide characters that precede the terminating null wide character.

(2) Same as (1), except that the function returns zero if str is a null pointer and returns strsz if the null wide character was not found in the first strsz wide characters of src

- As with all bounds-checked functions, wcslen_s is only guaranteed to be available if **STDC_LIB_EXT1** is defined by the implementation and if the user defines **STDC_WANT_LIB_EXT1** to the integer constant 1 before including <stdio.h>.

> Parameters

- str - pointer to the null-terminated wide string to be examined
- strsz - maximum number of wide characters to examine

> Return value

(1) The length of the null-terminated wide string str.

(2) The length of the null-terminated wide string str on success, zero if str is a null pointer, strsz if the null wide character was not found.

---

### wcscmp

```c
#include <wchar.h>

int wcscmp( const wchar_t* lhs, const wchar_t* rhs ); // (since C95)
```

Compares two null-terminated wide strings lexicographically.

The sign of the result is the sign of the difference between the values of the first pair of wide characters that differ in the strings being compared.

The behavior is undefined if lhs or rhs are not pointers to null-terminated wide strings.

> Parameters

- lhs, rhs - pointers to the null-terminated wide strings to compare

> Return value

Negative value if lhs appears before rhs in lexicographical order.

Zero if lhs and rhs compare equal.

Positive value if lhs appears after rhs in lexicographical order.

---
