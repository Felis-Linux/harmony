/* felis.harmony.lua.luadef
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

/*! @file sys.luadef.h
 *  @brief the system luadef
 */

#pragma once

/// @def LUA_PKG_ERR
/// @brief error code specifically for lua
#define LUA_PKG_ERR 221

#include <lua.h>

/// @var src_path
/// @brief src path thats managed externally
extern const char *src_path;
/// @var build_target_path
/// @brief build target path thats managed externally
extern const char *build_target_path;

/// @fn sysCmd
/// @param L lua context
/// @return lua index
/// @brief do not use this directly
int sysCmd(lua_State *L);

/// @fn exposeSysLuadefs
/// @param L lua context
/// @brief expose the whole luadefs space (or as you'd like to call it, namespace)
void exposeSysLuadefs(lua_State *l);
