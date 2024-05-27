#include <stdlib.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
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
Package_t *packageInit(char *filename) {
  Package_t *pkg = malloc(sizeof(Package_t));
  memset(pkg, 0, sizeof(Package_t));
  pkg->lua_context = luaL_newstate();
  luaL_loadfile(pkg->lua_context, filename);
  luaL_openlibs(pkg->lua_context);
  pkg->filename = filename;
  return pkg;
}

void packageFree(Package_t *package) {
  lua_close(package->lua_context);
  FOR_IN_PACKAGEID_FREE(dependencies);
  FOR_IN_PACKAGEID_FREE(conflicts);
  FOR_IN_PACKAGEID_FREE(optdeps);

  FOR_IN_UPSTREAMFILE_FREE(upstream_files);

  free(package->version);
  free(package->name);

  free(package);
}

void packageGetTables(Package_t *package) {
  if(! lua_istable(package->lua_context, -1)) {
    packageFree(package);
    error(LUA_PKG_ERR, "%s: %s is not a table!", package->filename, FPKG_PKG_LUA_PKGINF);
  } 

  lua_getfield(package->lua_context, -1, "name");
  if(! lua_isstring(package->lua_context, -1)) {
    packageFree(package);
    error(LUA_PKG_ERR, "%s: %s is not a string", package->filename, "name");
  }
  package->name = strdup(lua_tostring(package->lua_context, -1));
  lua_pop(package->lua_context, 1);

  lua_getfield(package->lua_context, -1, "version");
  if(! lua_isstring(package->lua_context, -1)){
    packageFree(package);
    error(LUA_PKG_ERR, "%s: %s is not a string", package->filename, "name");
  }
  package->version = strdup(lua_tostring(package->lua_context, -1));
  lua_pop(package->lua_context, 1);

  lua_getfield(package->lua_context, -1, "upstream_files");
  if(! lua_istable(package->lua_context, -1)) {
    packageFree(package);
    error(LUA_PKG_ERR, "%s: %s is not a field", package->filename, "upstream_files");
  }
  lua_pop(package->lua_context, 1);
  
  for(size_t i = 1; i <= lua_objlen(package->lua_context, -1); ++i) {
    lua_rawgeti(package->lua_context, -1, i);
    package->upstream_files = realloc(package->upstream_files, sizeof(UpstreamFile_t)*i);
    package->upstream_files_s++;
    if(! lua_istable(package->lua_context, -1)) {
      packageFree(package);
      error(LUA_PKG_ERR, "%s: index %zu in field %s is not a field", package->filename, i, "upstream_files");
    } 

    lua_getfield(package->lua_context, -1, "url");
    if(! lua_isstring(package->lua_context, -1)) {
      packageFree(package);
      error(LUA_PKG_ERR, "%s: field %s.%s is not a a string", package->filename, "upstream_files", "url");
    }
    package->upstream_files[i].url = strdup(lua_tostring(package->lua_context, -1));
    lua_pop(package->lua_context, 1);
    
    lua_getfield(package->lua_context, -1, "target");
    if(! lua_isstring(package->lua_context, -1)) {
      packageFree(package);
      error(LUA_PKG_ERR, "%s: field %s.%s is not a a string", package->filename, "upstream_files", "target");
    }
    package->upstream_files[i].target = strdup(lua_tostring(package->lua_context, -1));
    lua_pop(package->lua_context, 1);

    lua_pop(package->lua_context, 1);
  }
  lua_pop(package->lua_context, 1);

  lua_getfield(package->lua_context, -1, "dependencies");
  for(size_t i = 1; i<= lua_objlen(package->lua_context, -1); ++i) {
    package->dependencies = realloc(package->dependencies, sizeof(PackageId_t)*i);
    package->dependencies_s++;

    lua_rawgeti(package->lua_context, -1, i);
    if(lua_istable(package->lua_context, -1)) {
      packageFree(package);
      error(LUA_PKG_ERR, "%s: UNIMPLEMENTED. TODO.", package->filename);
    } else if(lua_isstring(package->lua_context, -1)){
      package->dependencies[i].e = pp_id_name;
      package->dependencies[i].name = strdup(lua_tostring(package->lua_context, -1));
    }
  }

  lua_pop(package->lua_context, 1);
  
}
