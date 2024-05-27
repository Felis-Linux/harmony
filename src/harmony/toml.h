/* felis.harmony.toml
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

#define TOK_LSQB 0x100
#define TOK_RSQB 0x200
#define TOK_EQUA 0x300

#define TOK_TRUE 0x800
#define TOK_FALSE 0x900

typedef struct Key TomlKeys_t;
typedef struct Array TomlArray_t;

typedef struct Node {
  char *name;
  union {
    char *str;
    long double ld;
    long long ll;
    TomlKeys_t *t;
    TomlArray_t *arr;
  } value_u;
  enum {
    tt_nd_str,
    tt_nd_ld,
    tt_nd_ll,
    tt_nd_t,
  } value_e;

  struct Node *next;
} TomlNode_t;

struct Array {
  TomlNode_t *nodes; 
};

struct Key {
  char *name;
  struct Key *parent;
  TomlNode_t *nodes;
};
