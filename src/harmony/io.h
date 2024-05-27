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

#define GENERIC_ERR 220

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

os_fd_t lockByPath(const char *);
void unlockAll();

[[gnu::format(printf, 1, 2)]] char *smprintf(const char *format, ...);

[[gnu::format(printf, 1, 2)]] void ok(const char *format, ...);
[[gnu::format(printf, 1, 2)]] void warning(const char *format, ...);
[[gnu::format(printf, 2, 3)]] void error(int ecode, const char *format, ...);
