#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"

/*! @file arena.c
 *  @todo add mmap and munmap support instead of malloc */

static inline size_t _next_pow2(size_t x) {
  return x == 1 ? 1 : 1<<(64-__builtin_clzl(x-1)); 
} 

Arena_t *arenaInit() {
  Arena_t *ret = malloc(sizeof(Arena_t));
  memset(ret, 0, sizeof(Arena_t));
  ret->head = nullptr;
  ret->foot = ret->head;
  return ret;
}

/// @fn regionInit
/// @brief initiates a new region of capacity (the capacity rounds up to nearest power of 2)
/// @param capacity the desired region capacity
/// @return a new region

Region_t *regionInit(size_t capacity) {
  capacity = (capacity > ARENA_MIN_S) ? _next_pow2(capacity) : ARENA_MIN_S;
  Region_t *ret = malloc(sizeof(Region_t) + sizeof(uintptr_t)*capacity);
  ret->next = nullptr;
  ret->capacity = capacity;
  ret->used = 0;
  ret->start = (uintptr_t *)ret + sizeof(Region_t);
  return ret;
}

void *arenaAlloc(Arena_t *arena, size_t size) {
  if(arena->head == nullptr) {
    arena->head = regionInit(size);
  }

  arena->foot = arena->head;
  while(arena->foot->used + size > arena->foot->capacity && arena->foot->next != nullptr) {
    arena->foot = arena->foot->next;
  }

  if(arena->foot->used + size > arena->foot->capacity) {
    arena->foot->next = regionInit(size);
    arena->foot = arena->foot->next;
  }

  void *ptr = (void *)(arena->foot->start + arena->foot->used);
  arena->foot->used += size;
  return ptr;
}

void *arenaRealloc(Arena_t *arena, void *ptr, size_t osize, size_t nsize) {
  if(ptr == nullptr)
      return arenaAlloc(arena, nsize);
  else if(nsize == 0)
      return nullptr;
  else if(osize >= nsize)
      return ptr;
  
  void *nptr = arenaAlloc(arena, nsize);
  char *nptr_ch = nptr;
  char *optr_ch = ptr;
  for(size_t i = 0; i < nsize; ++i) {
    nptr_ch[i] = optr_ch[i];
  }

  return nptr;
}

void arenaDestroy(Arena_t *arena) {
  while(arena->head != nullptr) {
    Region_t *next = arena->head->next;
    free(arena->head);
    arena->head = next;
  }
  free(arena);
}

char *astrdup(Arena_t *arena, const char *str) {
  char *target = arenaAlloc(arena, strlen(str));
  strcpy(target, str);
  return target;
}

[[gnu::format(printf, 2, 3)]] char *asmprintf(Arena_t *arena, const char *format, ...) {
  va_list args;
  va_start(args, format);
  
  size_t len = vsnprintf(nullptr, 0, format, args);
  char *buf = arenaAlloc(arena, sizeof(char)*(len + 1));
  vsnprintf(buf, len + 1, format, args);
  va_end(args);
  return buf;
}
