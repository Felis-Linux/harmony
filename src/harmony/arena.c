#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"

/* TODO: add mmap and munmap support */

inline size_t _next_pow2(size_t x) {
  return x == 1 ? 1 : 1<<(64-__builtin_clzl(x-1)); 
} 

Arena_t *arenaInit() {
  Arena_t *ret = malloc(sizeof(Arena_t));
  memset(ret, 0, sizeof(Arena_t));
  ret->head = nullptr;
  ret->foot = ret->head;
  return ret;
}

Region_t *regionInit(size_t capacity) {
  capacity = (capacity > ARENA_MIN_S) ? _next_pow2(capacity) : ARENA_MIN_S;
  Region_t *ret = malloc(sizeof(Region_t) + sizeof(uintptr_t)*capacity);
  ret->next = nullptr;
  ret->capacity = capacity;
  ret->used = 0;
  return ret;
}

void *arenaAlloc(Arena_t *arena, size_t size) {
  size_t capacity = (size > ARENA_MIN_S) ? ARENA_MIN_S : _next_pow2(size);
  if(arena->head == nullptr) {
    arena->foot->next = regionInit(capacity);
    arena->foot = arena->foot->next;
  }

  arena->foot = arena->head;
  while(arena->foot->used + size > arena->foot->capacity && arena->foot->next != nullptr) {
    arena->foot = arena->foot->next;
  }

  if(arena->foot->used + size > arena->foot->capacity) {
    arena->foot->next = regionInit(capacity);
    arena->foot = arena->foot->next;
  }

  void *ptr = (void *)(arena->foot->start + arena->foot->used);
  arena->head->used += size;
  return ptr;
}

void *arenaRealloc(Arena_t *arena, void *ptr, size_t osize, size_t nsize) {
  if(ptr == nullptr)
      return arenaAlloc(arena, nsize);
  else if(nsize == 0)
      return nullptr;
  else if(osize <= nsize)
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
}

char *astrdup(Arena_t *arena, const char *str) {
  char *target = arenaAlloc(arena, strlen(str));
  strcpy(target, str);
  return target;
}
