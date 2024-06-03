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

/*! @file io.h
 *  @brief The harmony input/output header
 *  @todo Add error full handling in the io def space
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


/// @brief System filedescriptor type (int)
typedef int os_fd_t;

/// @brief "Locks" (actually just creates a dummy file in a lock directory)
/// @brief and returns the file descriptor to it.
/// @param filename the filename to lock
/// @return the fd of the lock
os_fd_t hmnlock(char *filename);

/// @brief Makes a temporary directory
/// @return the temporary directory fd
os_fd_t mkhmntmp();

/// @brief Prints an ok message.
[[gnu::format(printf, 1, 2)]] void ok(const char *format, ...);
/// @brief Prints a warning message.
[[gnu::format(printf, 1, 2)]] void warning(const char *format, ...);
/// @brief Prints an error message and exits with ecode
/// @param ecode if used with errno, ensure usage like this: errno % GENERIC_ERR
[[gnu::format(printf, 2, 3)]] void error(int ecode, const char *format, ...);

/// @typedef UpstreamFile_t
/// @brief  A type that holds two strings, a url, and target.
///
/// url and target are both malloced.
typedef struct UpstreamFile {
  typeof(char *) url, target;
} UpstreamFile_t;

/// @typedef Downloader_t
/// @brief A type that holds all fields necessary to download files
/// by default, use CURL
typedef struct Downloader {
  /// the curl multi handle
  CURLM *curl_multi_handle;       
  /// the respective curl handles 
  CURL **curl_handles;               
  /// where to get the files from
  UpstreamFile_t *upstream_files;    
  /// target file descriptors
  os_fd_t *target_files;             
  /// locks created by hmnlock()
  os_fd_t *locks;                    
                                    
  /// tempdir filedescriptor made by mkhmntmp()
  os_fd_t tempdir;
  /// stores the size of curl_handles, target_files, locks, upstream_files.
  size_t ptrs_s; 
  // ptrs_s is a size for all of those pointers im too
  // lazy to manage all of them using 4 vars,            
  // sorry o.O... im just a simple girl uwu ~Ik
} Downloader_t;

/// @brief Initiates a Downloader_t
/// @return Returns a malloced pointer of a Downloader_t type
Downloader_t *downloaderInit();

/// @brief Appends UpstreamFile_ts to the Downloader_t structure
/// @param downloader the target of the appending
/// @param uf the UpstreamFile_ts
/// @param uf_s the size of the UpstreamFile_ts
void downloaderAppendUf(Downloader_t *downloader, UpstreamFile_t *uf, size_t uf_s);
/// @brief Downloads the files
/// @param downloader The downloader
void downloaderDownload(Downloader_t *downloader);
/// @brief Destroys a Downloader_t *
/// @param downloader The target of the destruction
void downloaderDestroy(Downloader_t *downloader);
