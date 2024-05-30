#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "arena.h"

/* TODO: add mmap and munmap support */

Region_t *regionInit(size_t capacity) {
  Region_t *ret = malloc(sizeof(Region_t) + sizeof(uintptr_t)*capacity);
  ret->next = nullptr;
  ret->capacity = capacity;
  ret->used = 0;
  return ret;
}

void arenaAddRegion(Arena_t *arena, size_t size) {
  Region_t *ret = regionInit(size);
  ret->next = arena->head;
  arena->head = ret;
}

void *arenaAlloc(Arena_t *arena, size_t size) {
  if(arena->head == nullptr) {  
    arenaAddRegion(arena, size);
  } else if(arena->head->used + size > arena->head->capacity) {
    arenaAddRegion(arena, arena->head->capacity * 2);
  }

  void *ptr = (void *)(arena->head->start + arena->head->used);
  arena->head->used += size;
  return ptr;
}

void arenaDestroy(Arena_t *arena) {
  while(arena->head != nullptr) {
    Region_t *next = arena->head->next;
    free(arena->head);
    arena->head = next;
  }
}

