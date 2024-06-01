/* felis.harmony.lua.pkg
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

/*! @file pkg.h
 *  @brief Harmony lua packaging i-face
 */

#pragma once

#include <stddef.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "io.h"
#include "arena.h"

typedef struct Package Package_t;
typedef struct PackageMetadata PackageMetadata_t;

/// @typedef Package_t
/// @brief Package structure that contains a single package
struct Package {
  /// the arena context that allocs everything
  Arena_t *arena;
  /// the lua context that is used for getting the variables in this file.
  lua_State *lua_context;
  /// the lua filename that's not owned by us
  const char *lua_filename;
  /// the package name
  char *name;
  /// the package version
  char *version; 
  
  UpstreamFile_t *upstream_files; /// upstream files

  typeof(char **) dependencies, conflicts, optdeps,
    authors, maintainers, licenses, upstream_urls, maintainers_urls;  
  size_t upstream_files_s, dependencies_s, conflicts_s, optdeps_s,
         authors_s, maintainers_s, licenses_s, upstream_urls_s, maintainers_urls_s;  
};

/// @fn packageInit
/// @param lua_filename
/// @return a package context
/// @brief this function creates a package context
Package_t *packageInit(const char *lua_filename);

/// @fn packageDestroy
/// @param package
/// @brief this function destroys a package context
void packageDestroy(Package_t *package);
