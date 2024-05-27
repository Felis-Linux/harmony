#include <stdlib.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luaconf.h>
#include <string.h>

#include "pkg.h"
#include "io.h"

#define FPKG_PKG_LUA_PKGINF "package_info"
#define FPKG_PKG_LUA_BUILD "build"

#define FOR_IN_FREE(w) \
  for(size_t i = 0; i < package->w##_s; ++i){\
    free(package->w[i]); \
  }\
  free(package->w);

#define FOR_IN_PACKAGEID_FREE(w) \
  for(size_t i = 0; i < package->w##_s; ++i) { \
    if(package->w[i].e == pp_id_name) { \
      free(package->w[i].name); \
    } else { \
      free(package->w[i].package); \
    } \
  }

#define FOR_IN_UPSTREAMFILE_FREE(w) \
  for(size_t i = 0; i < package->w##_s; ++i) { \
    free(package->w[i].url); \
    free(package->w[i].target); \
  } \
  free(package->w);

void packageFree(Package_t *package) {
  FOR_IN_PACKAGEID_FREE(dependencies);
  FOR_IN_PACKAGEID_FREE(conflicts);
  FOR_IN_PACKAGEID_FREE(optdeps);

  FOR_IN_UPSTREAMFILE_FREE(upstream_files);

  free(package->version);
  free(package->name);

  free(package);
}

Package_t *packageEval(char *filename) {
  Package_t *ret = malloc(sizeof(Package_t));
  memset(ret, 0, sizeof(Package_t));
  lua_State *L = luaL_newstate();
  luaL_openlibs(L);
  /* TODO:
   * push our functions onto the buildfile stack, eg. adduser
   */
  luaL_loadfile(L, filename);
  lua_getglobal(L, FPKG_PKG_LUA_PKGINF);
  if(! lua_istable(L, -1)) {
    packageFree(ret);
    lua_close(L);
    error(LUA_PKG_ERR, "%s: %s is not a table!", filename, FPKG_PKG_LUA_PKGINF);
  } 

  lua_getfield(L, -1, "name");
  if(! lua_isstring(L, -1)) {
    packageFree(ret);
    lua_close(L);
    error(LUA_PKG_ERR, "%s: %s is not a string", filename, "name");
  }
  ret->name = strdup(lua_tostring(L, -1));
  lua_pop(L, 1);

  lua_getfield(L, -1, "version");
  if(! lua_isstring(L, -1)){
    packageFree(ret);
    lua_close(L);
    error(LUA_PKG_ERR, "%s: %s is not a string", filename, "name");
  }
  ret->version = strdup(lua_tostring(L, -1));
  lua_pop(L, 1);

  lua_getfield(L, -1, "upstream_files");
  if(! lua_istable(L, -1)) {
    packageFree(ret);
    lua_close(L);
    error(LUA_PKG_ERR, "%s: %s is not a field", filename, "upstream_files");
  }
  lua_pop(L, 1);
  
  for(size_t i = 1; i <= lua_objlen(L, -1); ++i) {
    lua_rawgeti(L, -1, i);
    ret->upstream_files = realloc(ret->upstream_files, sizeof(UpstreamFile_t)*i);
    ret->upstream_files_s++;
    if(! lua_istable(L, -1)) {
      packageFree(ret);
      lua_close(L);
      error(LUA_PKG_ERR, "%s: index %zu in field %s is not a field", filename, i, "upstream_files");
    } 

    lua_getfield(L, -1, "url");
    if(! lua_isstring(L, -1)) {
      packageFree(ret);
      lua_close(L);
      error(LUA_PKG_ERR, "%s: field %s.%s is not a a string", filename, "upstream_files", "url");
    }
    ret->upstream_files[i].url = strdup(lua_tostring(L, -1));
    lua_pop(L, 1);
    
    lua_getfield(L, -1, "target");
    if(! lua_isstring(L, -1)) {
      packageFree(ret);
      lua_close(L);
      error(LUA_PKG_ERR, "%s: field %s.%s is not a a string", filename, "upstream_files", "target");
    }
    ret->upstream_files[i].target = strdup(lua_tostring(L, -1));
    lua_pop(L, 1);

    lua_pop(L, 1);
  }
  lua_pop(L, 1);

  lua_pop(L, 1);

  lua_getglobal(L, FPKG_PKG_LUA_BUILD);
  if(!lua_isfunction(L, -1)){
    /* TODO:
     * call error. 
     */
  }

  lua_pcall(L, 0, 0, 0);
  lua_pop(L, 1);
}
