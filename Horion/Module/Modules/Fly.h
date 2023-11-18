#pragma once

#include "Module.h"

class Fly : public IModule {
private:
	std::vector<PlayerAuthInputPacket*> PlayerAuthInputPacketHolder;
	float horizontalSpeed = 1.5f;
	float verticalSpeed = 0.5;

	int gameTick = 0;
	float nowpos = 0;

public:
	Fly();
	~Fly();

	bool isDamaged = false;
	bool dodesync = false;
	__int64 lastFlyMs;

	inline std::vector<PlayerAuthInputPacket*>* getPlayerAuthInputPacketHolder() { return &PlayerAuthInputPacketHolder; };

	SettingEnum mode = SettingEnum(this);

	// Inherited via IModule
	virtual void onEnable() override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onSendPacket(Packet* packet);
	virtual void onDisable() override;
	virtual void onMove(MoveInputHandler* input) override;
};
