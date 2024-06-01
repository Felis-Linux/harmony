#include <stdlib.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luaconf.h>
#include <string.h>

#include "pkg.h"
#include "arena.h"
#include "io.h"
#include "luadef.h"
#include "sys.luadef.h"

#define FPKG_PKG_LUA_PKGINF "package_info"
#define FPKG_PKG_LUA_BUILD "build"

Package_t *packageInit(const char *lua_filename) {
  Package_t *pkg = malloc(sizeof(Package_t));
  memset(pkg, 0, sizeof(Package_t));
  pkg->lua_context = luaL_newstate();
  luaL_loadfile(pkg->lua_context, lua_filename);
  luaL_openlibs(pkg->lua_context);
  pkg->lua_filename = lua_filename;
  pkg->arena = arenaInit();
  return pkg;
}

void packageDestroy(Package_t *pkg) {
  lua_close(pkg->lua_context);
  
  arenaDestroy(pkg->arena);

  free(pkg);
}


void packageGetTables(Package_t *pkg) {
  if(! lua_istable(pkg->lua_context, -1)) {
    packageDestroy(pkg);
    error(LUA_PKG_ERR, "%s: %s is not a table!", pkg->lua_filename, FPKG_PKG_LUA_PKGINF);
  } 

  lua_getfield(pkg->lua_context, -1, "name");
  if(! lua_isstring(pkg->lua_context, -1)) {
    packageDestroy(pkg);
    error(LUA_PKG_ERR, "%s: %s is not a string", pkg->lua_filename, "name");
  }
  pkg->name = astrdup(pkg->arena, lua_tostring(pkg->lua_context, -1));
  lua_pop(pkg->lua_context, 1);

  lua_getfield(pkg->lua_context, -1, "version");
  if(! lua_isstring(pkg->lua_context, -1)){
    packageDestroy(pkg);
    error(LUA_PKG_ERR, "%s: %s is not a string", pkg->lua_filename, "name");
  }
  pkg->version = astrdup(pkg->arena, lua_tostring(pkg->lua_context, -1));
  lua_pop(pkg->lua_context, 1);

  lua_getfield(pkg->lua_context, -1, "upstream_files");
  if(! lua_istable(pkg->lua_context, -1)) {
    packageDestroy(pkg);
    error(LUA_PKG_ERR, "%s: %s is not a field", pkg->lua_filename, "upstream_files");
  }
  
  pkg->upstream_files_s = lua_objlen(pkg->lua_context, -1);
  pkg->upstream_files = arenaAlloc(pkg->arena, sizeof(char*)*pkg->upstream_files_s);
  for(size_t i = 1; i <= lua_objlen(pkg->lua_context, -1); ++i) {
    lua_rawgeti(pkg->lua_context, -1, i);
    if(! lua_istable(pkg->lua_context, -1)) {
      packageDestroy(pkg);
      error(LUA_PKG_ERR, "%s: index %zu in field %s is not a field", pkg->lua_filename, i, "upstream_files");
    } 

    lua_getfield(pkg->lua_context, -1, "url");
    if(! lua_isstring(pkg->lua_context, -1)) {
      packageDestroy(pkg);
      error(LUA_PKG_ERR, "%s: field %s.%s is not a a string", pkg->lua_filename, "upstream_files", "url");
    }
    pkg->upstream_files[i].url = astrdup(pkg->arena, lua_tostring(pkg->lua_context, -1));
    lua_pop(pkg->lua_context, 1);
    
    lua_getfield(pkg->lua_context, -1, "target");
    if(! lua_isstring(pkg->lua_context, -1)) {
      packageDestroy(pkg);
      error(LUA_PKG_ERR, "%s: field %s.%s is not a a string", pkg->lua_filename, "upstream_files", "target");
    }
    pkg->upstream_files[i].target = astrdup(pkg->arena, lua_tostring(pkg->lua_context, -1));
    lua_pop(pkg->lua_context, 1);

    lua_pop(pkg->lua_context, 1);
  }
  lua_pop(pkg->lua_context, 1);

#define FILL_CHAR_ARR(w) \
  lua_getfield(pkg->lua_context, -1, #w); \
  pkg->w = arenaAlloc(pkg->arena, sizeof(char*)*lua_objlen(pkg->lua_context, -1)); \
  pkg->w##_s += lua_objlen(pkg->lua_context, -1); \
  for(size_t i = 1; i <= lua_objlen(pkg->lua_context, -1); ++i) { \
    lua_rawgeti(pkg->lua_context, -1, i); \
    if(!lua_isstring(pkg->lua_context, -1)){ \
      packageDestroy(pkg); \
      error(LUA_PKG_ERR, "%s: elem of array %s is not a string", pkg->lua_filename, #w);\
    } \
    pkg->dependencies[i-1] = astrdup(pkg->arena, lua_tostring(pkg->lua_context, -1)); \
  } \
  lua_pop(pkg->lua_context, 1);

  FILL_CHAR_ARR(dependencies);
  FILL_CHAR_ARR(conflicts);
  FILL_CHAR_ARR(optdeps);

  lua_pop(pkg->lua_context, 1);
}

void packageEvalBuildFile(Package_t *pkg) {
  exposeLuadefs(pkg->lua_context);

  lua_getglobal(pkg->lua_context, "prebuild");
  if(!lua_isnil(pkg->lua_context, -1))
      lua_call(pkg->lua_context, 0, 0);
   lua_pop(pkg->lua_context, 1);

  lua_getglobal(pkg->lua_context, "build");
  if(!lua_isnil(pkg->lua_context, -1))
      lua_call(pkg->lua_context, 0, 0);
  lua_pop(pkg->lua_context, 1);

  lua_getglobal(pkg->lua_context, "postbuild");
  if(!lua_isnil(pkg->lua_context, -1))
      lua_call(pkg->lua_context, 0, 0);
  lua_pop(pkg->lua_context, 1);
}
