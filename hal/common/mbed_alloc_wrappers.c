/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "api/mbed_mem_trace.h"
#include <stddef.h>
#include <stdio.h>

#if defined(TOOLCHAIN_GCC)

#ifdef   FEATURE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif/* FEATURE_UVISOR */

// TODO: memory tracing doesn't work with uVisor enabled.
#if !defined(FEATURE_UVISOR)
void * __wrap__malloc_r(struct _reent * r, size_t size) {
    extern void * __real__malloc_r(struct _reent * r, size_t size);
#ifdef MBED_MEM_TRACING_ENABLED
    return mbed_mem_trace_malloc(__real__malloc_r(r, size), size);
#else
    return __real__malloc_r(r, size);
#endif
}
void * __wrap__realloc_r(struct _reent * r, void * ptr, size_t size) {
    extern void * __real__realloc_r(struct _reent * r, void * ptr, size_t size);
#ifdef MBED_MEM_TRACING_ENABLED
    return mbed_mem_trace_realloc(__real__realloc_r(r, ptr, size), ptr, size);
#else
    return __real__realloc_r(r, ptr, size);
#endif
}
void __wrap__free_r(struct _reent * r, void * ptr) {
    extern void __real__free_r(struct _reent * r, void * ptr);
    __real__free_r(r, ptr);
#ifdef MBED_MEM_TRACING_ENABLED
    mbed_mem_trace_free(ptr);
#endif
}
void * __wrap__calloc_r(struct _reent * r, size_t nmemb, size_t size) {
    extern void* __real__calloc_r(struct _reent * r, size_t nmemb, size_t size);
#ifdef MBED_MEM_TRACING_ENABLED
    return mbed_mem_trace_calloc(__real__calloc_r(r, nmemb, size), nmemb, size);
#else
    return __real__calloc_r(r, nmemb, size);
#endif
}
#endif // if !defined(FEATURE_UVISOR)

#elif defined(TOOLCHAIN_ARM) // #if defined(TOOLCHAIN_GCC)

void* $Sub$$malloc(size_t size) {
#ifdef MBED_MEM_TRACING_ENABLED
    return trace_malloc($Super$$malloc(size), size);
#else
    return $Super$$malloc(size);
#endif
}

void* $Sub$$realloc(void *ptr, size_t size) {
#ifdef MBED_MEM_TRACING_ENABLED
    return trace_realloc($Super$$realloc(ptr, size), ptr, size);
#else
    return $Super$$realloc(ptr, size);
#endif
}

void *$Sub$$calloc(size_t nmemb, size_t size) {
#ifdef MBED_MEM_TRACING_ENABLED
    return trace_calloc($Super$$calloc(nmemb, size), nmemb, size);
#else
    return $Super$$calloc(nmemb, size);
#endif
}

void $Sub$$free(void *ptr) {
    $Super$$free(ptr);
#ifdef MBED_MEM_TRACING_ENABLED
    trace_free(ptr);
#endif
}

#else // #if defined(TOOLCHAIN_GCC)

#ifdef MBED_MEM_TRACING_ENABLED
#error Memory tracing not supported with the current toolchain.
#endif

#endif // #if defined(TOOLCHAIN_GCC)

