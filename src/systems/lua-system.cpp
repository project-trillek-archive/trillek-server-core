#include "systems/lua-system.hpp"
#include <iostream>

namespace trillek {
namespace script {

int luaopen_Transform(lua_State*);
int luaopen_LuaSys(lua_State*);

LuaSystem::LuaSystem() {}
LuaSystem::~LuaSystem() { }

void LuaSystem::Start() {
    this->L = luaL_newstate();
    luaL_openlibs(L);
    RegisterTypes();
}

void LuaSystem::RegisterSystem(LuaRegisterFunction func) {
    if (this->L) {
        func(this->L);
    }
}

bool LuaSystem::LoadFile(const std::string fname) {
    if (!this->L) {
        return false;
    }
    if (luaL_dofile(L, fname.c_str())) {
        // TODO: Ommit error string about the filename that failed to load.
        std::cerr << lua_tostring(L, -1) << std::endl;
        return false;
    }
    return true;
}

void LuaSystem::HandleEvents(frame_tp timepoint) {
    static frame_tp last_tp;
    this->delta = timepoint - last_tp;
    last_tp = timepoint;
    // Call the Update method from Lua. A time delta is passed in.
    if (this->L) {
        //lua_getglobal(L, "Update");
        //lua_pushnumber(L, delta.count() * 1.0E-9);
        //lua_pcall(L, 1, 0, 0);
    }
}

void LuaSystem::Terminate() {
    lua_close(L);
}

} // End of script
} // End of trillek

