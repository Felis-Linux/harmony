/* felis.harmony.index
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
/*! @file index.h
 *  @brief The harmony binary index format */
#pragma once

#include <stddef.h>
#include <stdbool.h>

#define INDEX_ERR 244

#include "io.h"
#include "arena.h"


typedef struct Index Index_t;

/// @typedef IndexedPackage_t
/// @brief a type that stores a package index
typedef struct IndexedPackage {
  typeof(char *) package_name;
  size_t         package_name_len;
  typeof(char *) package_version;
  size_t         package_version_len;
} IndexedPackage_t;

/// @typedef IndexedFile_t
/// @brief a type that stores a file index
typedef struct IndexedFile {
  typeof(char *)  file_name;
  size_t          file_name_len;
  bool is_binary, is_linked;
  typeof(struct LinkedFiles {
    char *target;
    size_t target_len;
  } *) linked_files;
  size_t linked_files_s;
} IndexedFile_t;

/// @typedef IndexMode_t
/// @brief the Mode option
typedef enum IndexMode {
  im_nul = 0x0, /// nul, typically this means a wrongly written file
  im_packages_mode = 0x9a, /// packages-mode, enables the package index 
  im_files_mode = 0x7b, /// files-mode, enables the file index
} IndexMode_t;

/// @typedef Index_t
/// @brief the Index format state
///
/// stores two possible subformats,
/// 1. package index
/// 2. file index
struct Index {
  IndexMode_t index_mode_option; /// the option that enables the unions
  /// @union IndexU
  /// @brief The union responsible for storing everything
  union {
    /// @struct Index1
    /// @brief The file index struct
    struct {
      IndexedFile_t  *indexed_files;
      size_t          indexed_files_s;
    };
    /// @struct Index 2
    /// @brief The package index struct
    struct {
        char  *repo_name;
        size_t repo_name_len;
        IndexedPackage_t *indexed_packages;
        size_t            indexed_packages_s;
    };
  };
  os_fd_t     index_file_fd;   /// the indexed fd which is retained by the given Index_t
  const char *index_file_name; /// the external filename.
  
  Arena_t *arena;              /// the arena allocator context
};
