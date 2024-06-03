/* felis.harmony.strings
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

/*! @file strings.h
 *  @brief The harmony string manipulation library
 */

/// @def HMN_UNUSED_MALLOC
/// @brief the "This is a malloc function, and the return value went unused." nodiscard. 
#define HMN_UNUSED_MALLOC \
  [[nodiscard("This is a malloc function, and the return value went unused.")]]

#pragma once

#include <stddef.h>

/// @typedef string_t
/// @brief potentially unused type alias
typedef char * string_t;

/// @brief Cuts up a string into a smaller string
/// @param string the string to cut up
/// @param from where to start cutting from (has to be in the string)
/// @param to where to end cutting (has to be in the string and larger than from)
/// @return the string that's cut up, has to be freed via free() or a nullptr
HMN_UNUSED_MALLOC char *hmnstrcut(char *string, size_t from, size_t to);

/// @brief Dynamically allocates a dynamic string
/// @param format the format of the string
/// @return returns a malloced string or a nullptr
[[gnu::format(printf, 1, 2)]] HMN_UNUSED_MALLOC char *smprintf(const char *format, ...);

/// @typedef StringsError_t
/// @brief Strings Error structure, for error handling
typedef struct {
  /// @enum option
  enum {
    /// ok -> everything's fine, no need to panic
    strs_ok,
    /// regx_fail -> read the errstr
    strs_regx_fail,
    /// alloc_fail -> do literally nothing as any allocation will be a nullptr
    strs_alloc_fail,
  } option;

  /// ok -> read this string
  char *str;
  /// any other enumeration (except for alloc_fail) -> read this
  char *errstr;
} StringsError_t;

/// @fn pkgstringGetVersion
/// @param pkgstring the pkg string, e.g. "harmony^0.0.1"
/// @return the error pseudounion type that handles three cases: str, errstr and nullptr
StringsError_t pkgstringGetVersion(const char *pkgstring);
