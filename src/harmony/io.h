/* felis.harmony.io
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

#include <curl/multi.h>

#define GENERIC_ERR 220
#define HMN_OK 0
#define HMN_STRING_TOO_LONG 1

#define COLOR_RESET "\x1b[0m"

#define COLOR_BLACK "\x1b[0;30m"
#define COLOR_RED "\x1b[0;31m"
#define COLOR_GREEN "\x1b[0;32m"
#define COLOR_YELLOW "\x1b[0;33m"
#define COLOR_BLUE "\x1b[0;34m"
#define COLOR_MAGENTA "\x1b[0;35m"
#define COLOR_CYAN "\x1b[0;36m"
#define COLOR_WHITE "\x1b[0;37m"

#define COLOR_ORANGE "\x1b[38;2;255;165;0m"

#define COLOR_BOLD_BLACK "\x1b[1;30m"
#define COLOR_BOLD_RED "\x1b[1;31m"
#define COLOR_BOLD_GREEN "\x1b[1;32m"
#define COLOR_BOLD_YELLOW "\x1b[1;33m"
#define COLOR_BOLD_BLUE "\x1b[1;34m"
#define COLOR_BOLD_MAGENTA "\x1b[1;35m"
#define COLOR_BOLD_CYAN "\x1b[1;36m"
#define COLOR_BOLD_WHITE "\x1b[1;37m"

#define COLOR_BOLD_ORANGE "\x1b[1;38;2;255;165;0m"

typedef int os_fd_t;

os_fd_t hmnlock(char *);
os_fd_t mkhmntmp();
char *hmnstrcut(char *, size_t from, size_t to, int *return_code);

[[gnu::format(printf, 1, 2)]] char *smprintf(const char *format, ...);

[[gnu::format(printf, 1, 2)]] void ok(const char *format, ...);
[[gnu::format(printf, 1, 2)]] void warning(const char *format, ...);
[[gnu::format(printf, 2, 3)]] void error(int ecode, const char *format, ...);

typedef struct UpstreamFile {
  typeof(char *) url, target;
} UpstreamFile_t;

typedef struct Downloader {         
  CURLM *curl_multi_handle;         //////////////////////////////////////////////////////////
  CURL **curl_handles;              // ptrs_size is a size for all of those pointers im too //
  UpstreamFile_t *upstream_files;   // lazy to manage all of them using 4 vars,             //
  os_fd_t *target_files;            // sorry o.O... im just a simple girl uwu ~Ika          // 
  os_fd_t *locks;                   //////////////////////////////////////////////////////////

  os_fd_t tempdir;
  
  size_t ptrs_s;
} Downloader_t;

Downloader_t *downloaderInit();
void downloaderAppendUf(Downloader_t *downloader, UpstreamFile_t *uf, size_t uf_s);
void downloaderDownload(Downloader_t *downloader);
void downloaderDestroy(Downloader_t *downloader);
