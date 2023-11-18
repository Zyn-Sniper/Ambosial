#pragma once

#include "../../../SDK/Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class Killaura : public IModule {
private:
	bool isMulti = true;
	bool randomize = false;
	int delay = 0;
	int Odelay = 0;
	bool autoweapon = false;
	void findWeapon();
        float smoothing = 15.f;
public:
	bool strafe = false;
	bool visualize = true;
	bool targetListEmpty = false;
	Vec2 angle;
	bool hurttime = true;
	float range = 6;
	float animYaw = 0;
	float animPitch = 0;

	std::vector<float> CircleY;

	SettingEnum visualizeMode = SettingEnum(this);
	SettingEnum rotations = SettingEnum(this);

	Killaura();
	~Killaura();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onPlayerTick(Player* plr);
	virtual void onLevelRender();
	virtual void onPreRender(MinecraftUIRenderContext* renderCtx);
	virtual void onEnable() override;
	virtual void onSendPacket(Packet* packet) override;
};
