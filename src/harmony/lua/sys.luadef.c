#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/wait.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luajit.h>

#include "io.h"
#include "sys.luadef.h"

int sysCmd(lua_State *L) {
  luaL_checktype(L, 1, LUA_TTABLE);
  luaL_checktype(L, 2, LUA_TTABLE);
    
  size_t argv_s = lua_objlen(L, 1);
  size_t envv_s = lua_objlen(L, 2);
  char **argv = malloc(argv_s*sizeof(char*));
  char **envv = malloc(envv_s*sizeof(char*));
  
  for(size_t i = 0; i < argv_s; ++i) {
    lua_rawgeti(L, 1, i + 1);
    argv[i] = strdup(lua_tostring(L, -1));
    lua_pop(L, 1);
  }

  for(size_t i = 0; i < envv_s; ++i) {
    lua_rawgeti(L, 2, i + 1);
    argv[i] = strdup(lua_tostring(L, -1));

    lua_pop(L, 1);
  }
  
  pid_t pid = fork();

  if(pid == -1) {
    lua_pushnil(L);
    lua_pushstring(L, strerror(errno));
  } else if(pid == 0) {
    int status;
    waitpid(pid, &status, 0);

    for(size_t i = 0; i < argv_s; ++i) {
      free(argv[i]);
    }
    free(argv);

    for(size_t i = 0; i < envv_s; ++i) {
      free(envv[i]);
    }
    free(envv);

  } else
      execve(argv[0], argv, envv);
  
  return 0;
}
