## About

These are utility programs for running other programs. Check the files under
the `release` folder. `CMD.reg` is an example use case for these programs.

`Run.exe` runs a program normally with command line arguments and optional
working directory.

`Elevate.exe` tries to run a program with administrator privileges, along with
command line arguments and optional working directory.

There are only three valid ways to run either. The order of arguments matters.

```cmd
Run <program> [<program arguments>]
Run /d <working directory> <program> [<program arguments>]
Run -d <working directory> <program> [<program arguments>]
```

```cmd
Elevate <program> [<program arguments>]
Elevate /d <working directory> <program> [<program arguments>]
Elevate -d <working directory> <program> [<program arguments>]
```

Arguments wrapped in `<>` are placeholders. Replace them along with the `<>`
with the actual values you want to use.

Arguments wrapped in `[]` are optional. They may be omitted.

`<working directory>` must be a directory path, and must follow the `/d` or
`-d` argument. The `/d` or `-d` must be the first argument, and the working
directory path must be the second argument.

`<program>` must be a program path or a program alias. Aliases can be set up
through the registry or 3rd party programs. This must be the fist argument or
the third argument.

`[<program arguments>]` are the optional command line arguments that you would
normally supply when running `<program>` from the terminal, and must follow the
`<program>` argument. This must be the second argument or the fourth argument.

## Copyright & License

**TL;DR:**

> This code is truly free and open source, licensed under the Apache 2.0 License. If you make a copy, **I humbly ask** that you include the text from the `NOTICE` file somewhere in your project. **_You are not required to!_** You are also not required to include the original `LICENSE-APACHE` or `NOTICE` files, and I would prefer just a copy of the `NOTICE` file text or a link to this repository instead. You can use and modify this code however you like, including using a proprietary license for your changes. The only restriction I maintain is under clause 3 of the Apache 2.0 License regarding patents. If you find any potential license violations, please contact me so that I may resolve them.

---

**Full Disclosure**

- _mission_

The code in this repository will always be truly free and open source (unless I myself have somehow violated an upstream copyright license, in which case I will gladly try to resolve any issues in a timely manner; please email me about any potential license violations you find).

- _please leave a trail_

When making a copy of this project, I _kindly ask_ that you include the text within the `NOTICE` file somewhere (perhaps in your own README.md or LICENSE or NOTICE file?) or a link to this repository so that other readers of your project may also be able to find this original template.

```
CXX RunElevate
https://github.com/ericchase/cxx--run-elevate

Copyright © 2025 ericchase

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```

- _your usage of this source code_

That said, this license and copyright notice **DOES NOT** restrict your usage of this template in any way, except for the terms and conditions under clause 3 of the Apache 2.0 License regarding patents: `3. Grant of Patent License.` As you may or may not know, every piece of work is automatically protected and restricted by **copyright** law. The purpose of a **license** is to "unrestrict" the copyright owner's protections under that law, granting others access to use their work. The **patent system**, on the other hand, is a system for **applying restrictions** to the implementation of ideas. Specifically:

> A patent is a type of intellectual property that gives its owner the legal right to exclude others from making, using, or selling an invention for a limited period of time in exchange for publishing an enabling disclosure of the invention. - https://en.wikipedia.org/wiki/Patent

- _patent law_

I don't know enough about patent law to know if this could ever become an issue, but I would rather be safe than sorry. What I do know is that copyright law and patent law are completely separate issues, and copyright law does not protect your work from patents (AFAIK). The Apache 2.0 License does its best to provide some protections from patents of derivative works, which is why I use it for my projects.

- _other terms and conditions_

To reiterate, I hereby informally waive the other terms and conditions under the Apache 2.0 License. You are not required to include the original `LICENSE-APACHE` or `NOTICE` files or text in your derivative work.

- _your derivative works_

As for your own projects, any new additions and/or modifications you make **ARE NOT** subject to my license and copyright notice. You do not need to mention additions and/or modifications to the original source code. You will need to apply your own license and copyright notices if you wish to make your project code open source. If you wish to keep your source code private, you may do so. You may use a proprietary and/or closed source license if you wish. All of this is entirely up to you.

_This is what it means to be truly free and open source._

## Quick C API Reference

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
