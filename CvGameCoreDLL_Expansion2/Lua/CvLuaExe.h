#pragma once

#ifndef CV_LUA_EXE_H
#define CV_LUA_EXE_H

#include "CvLuaSupport.h"

class CvLuaExe
{
public:
	static void RegisterGlobals(lua_State* L);

protected:
	static int lGetBuildName(lua_State* L);
	static int lCanDisableEngineYieldIconManager(lua_State* L);
	static int lTryDisableEngineYieldIconManager(lua_State* L);
};

#endif // CV_LUA_EXE_H
