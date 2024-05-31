/* felis.harmony.arena
 * Copyright (c) 2024, Ika Sek
 *  All rights reserved. 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions are met:
 *  *  Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer. 
 *  *  Redistributions in binary form must reproduce the above copyright 
 *     notice, this list of conditions and the following disclaimer in the 
 *     documentation and/or other materials provided with the distribution. 
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND ANY 
 *  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 *  DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY 
 *  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 *  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR 
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY 
 *  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH 
 *  DAMAGE. 
 */

/*! @file arena.h
 *  @brief Arena Allocator for Harmony
 *
 * Implementation inspired by tsodings, however, we kinda do our own thing */

#pragma once

#include <stddef.h>
#include <stdint.h>

/// @def ARENA_MIN_S
/// @brief Minimal size for a Region_t
#define ARENA_MIN_S (1024*8)

typedef struct Arena Arena_t;
typedef struct Region Region_t;

/// @typedef Region_t
/// @brief A Region storing data
struct Region {
  Region_t *next; /// the next Region
  size_t used, capacity; /// region info
  uintptr_t *start; /// start of the pointer
};

/// @typedef Arena_t
/// @brief An arena context
///
/// harmony.arena rounds up to the nearest power of two!
struct Arena {
  typeof(Region_t *) head, foot; /// the head and the foot or the Arena Allocator
};

/// @fn arenaInit
/// @brief  creates a new Arena_t *
/// @return returns an arena allocator context
Arena_t *arenaInit();

/// @fn arenaAlloc
/// @brief allocates size from arena
/// @param arena the arena to allocate from
/// @param size size to allocate
/// @return the allocated pointer, that can't be free'd
///
/// Internally, if the arena->foot = nullptr, allocate a new region,
/// when we don't have enough size inside the arena->foot, allocate a new region and put it as foot
void *arenaAlloc(Arena_t *arena, size_t size);

/// @fn arenaRealloc
/// @todo make the realloc actually a realloc not a funny memcpy and also don't use osize, figure it out on our own.
/// @brief returns a realloced pointer (not really)
/// @param arena the arena to reallocate from
/// @param ptr the ptr to realloc
/// @param osize the old size of ptr
/// @param nsize the new size of ptr
void *arenaRealloc(Arena_t *arena, void *ptr, size_t osize, size_t nsize);

/// @fn arenaDestroy
/// @brief destroys the whole arena, that is how you free alloced memory by an arena allocator.
/// @param arena the arena to destroy
void arenaDestroy(Arena_t *arena);

/// @fn astrdup
/// @brief strdup, but specifically for an arena allocator
/// @param arena the arena to use while duplicating
/// @param str the string that is supposed to be copied
/// @return a duplicated string that has to be freed like an arenaalloced var
char *astrdup(Arena_t *arena, const char *str);
