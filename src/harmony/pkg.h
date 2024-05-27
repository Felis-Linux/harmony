/* felis.harmony.pkg
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

#pragma once

#include <stddef.h>

#define LUA_PKG_ERR 221

typedef struct Package Package_t;

typedef struct PackageId {
  union {
    char *name;
    Package_t *package;
  };
  enum {
    pp_id_name,
    pp_id_package_struct,
  } e;
} PackageId_t;

typedef struct UpstreamFile {
  char *url;
  char *target;
} UpstreamFile_t;

struct Package {
  typeof(char *) name, version;
  typeof(UpstreamFile_t *) upstream_files;

  typeof(PackageId_t *) dependencies, conflicts, optdeps;  
  size_t upstream_files_s, dependencies_s, conflicts_s, optdeps_s;  
};

struct PackageMetadata {
 unsigned char u;
};
