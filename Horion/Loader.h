#pragma once

#include "../SDK/Utils/Logger.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <iostream>
#include <string>
#include <vector>

#include "../Memory/GameData.h"
#include "../Memory/Hooks.h"
#include "../Memory/MinHook.h"
#include "../SDK/ChestBlockActor.h"
#include "../SDK/ClientInstance.h"
#include "../SDK/ClientInstanceScreenModel.h"
#include "../SDK/Entity.h"
#include "../SDK/GameMode.h"
#include "../SDK/Packet.h"
#include "../SDK/Utils/HMath.h"
#include "../SDK/Utils/Target.h"
#include "../SDK/Utils/TextFormat.h"
#include "../SDK/Utils/Utils.h"
#include "../include/WinHttpClient.h"
#include "Command/CommandMgr.h"
#include "Config/ConfigManager.h"
#include "Menu/ClickGui.h"
#include "Menu/TabGui.h"
#include "Module/ModuleManager.h"
#include "ImmediateGui.h"

class Loader {
public:
	static bool isRunning;
};