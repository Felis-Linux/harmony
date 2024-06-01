#include "lua.h"
#include "sys.luadef.h"
#include "luadef.h"

void exposeLuadefs(lua_State *L) {
  lua_newtable(L);
  lua_setglobal(L, "hmn");

  lua_newtable(L);
  lua_setfield(L, -1, "sys");

  lua_pushcfunction(L, sysCmd);
  lua_setfield(L, -2, "cmd");

  lua_pop(L, 2);
  lua_pop(L, 1);
}
