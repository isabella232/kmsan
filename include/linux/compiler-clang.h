/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_COMPILER_TYPES_H
#error "Please don't include <linux/compiler-clang.h> directly, include <linux/compiler.h> instead."
#endif

#define CLANG_VERSION (__clang_major__ * 10000	\
		     + __clang_minor__ * 100	\
		     + __clang_patchlevel__)

#if CLANG_VERSION < 100001
# error Sorry, your version of Clang is too old - please use 10.0.1 or newer.
#endif

/* Compiler specific definitions for Clang compiler */

/* same as gcc, this was present in clang-2.6 so we can assume it works
 * with any version that can compile the kernel
 */
#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)

/* all clang versions usable with the kernel support KASAN ABI version 5 */
#define KASAN_ABI_VERSION 5

#if __has_feature(address_sanitizer) || __has_feature(hwaddress_sanitizer)
/* Emulate GCC's __SANITIZE_ADDRESS__ flag */
#define __SANITIZE_ADDRESS__
#define __no_sanitize_address \
		__attribute__((no_sanitize("address", "hwaddress")))
#else
#define __no_sanitize_address
#endif

#if __has_feature(thread_sanitizer)
/* emulate gcc's __SANITIZE_THREAD__ flag */
#define __SANITIZE_THREAD__
#define __no_sanitize_thread \
		__attribute__((no_sanitize("thread")))
#else
#define __no_sanitize_thread
#endif

#if __has_feature(undefined_behavior_sanitizer)
/* GCC does not have __SANITIZE_UNDEFINED__ */
#define __no_sanitize_undefined \
		__attribute__((no_sanitize("undefined")))
#else
#define __no_sanitize_undefined
#endif

#if __has_feature(memory_sanitizer)
# define __SANITIZE_MEMORY__
# define __no_sanitize_memory __attribute__((no_sanitize("kernel-memory")))
#else
# define __no_sanitize_memory
#endif

/*
 * Not all versions of clang implement the type-generic versions
 * of the builtin overflow checkers. Fortunately, clang implements
 * __has_builtin allowing us to avoid awkward version
 * checks. Unfortunately, we don't know which version of gcc clang
 * pretends to be, so the macro may or may not be defined.
 */
#if __has_builtin(__builtin_mul_overflow) && \
    __has_builtin(__builtin_add_overflow) && \
    __has_builtin(__builtin_sub_overflow)
#define COMPILER_HAS_GENERIC_BUILTIN_OVERFLOW 1
#endif

/* The following are for compatibility with GCC, from compiler-gcc.h,
 * and may be redefined here because they should not be shared with other
 * compilers, like ICC.
 */
#define barrier() __asm__ __volatile__("" : : : "memory")

#if __has_feature(shadow_call_stack)
# define __noscs	__attribute__((__no_sanitize__("shadow-call-stack")))
#endif
