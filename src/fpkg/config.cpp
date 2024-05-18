#include <string>
#include <cstddef>
#include <filesystem>
#include <format>
#include <iostream>

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include "config.hpp"

namespace felis::fpkg::config {
  Config parseConfig(std::filesystem::path path) {
    lua_State *state = luaL_newstate();
    luaL_openlibs(state);
    if(luaL_dofile(state, path.c_str()) != LUA_OK) {
      //TODO: provide error api
    }

    luabridge::getGlobal(state, "config");
    Config conf;
    try {
      conf = luabridge::get<Config>(state, -1);
    } catch(luabridge::LuaException) {
      //TODO: provide error api
    }

    return conf;
  }
}
 
