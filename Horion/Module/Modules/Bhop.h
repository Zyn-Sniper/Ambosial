#pragma once

#include "..\..\../SDK\Utils\TimerUtil.h"
#include "Module.h"

class Bhop : public IModule {
private:
	std::vector<PlayerAuthInputPacket*> PlayerAuthInputPacketHolder;
	std::vector<InventoryTransactionPacket*> InventoryPacketHolder;
	float speed = 0.325f;
	bool hive = false;
	bool desync = false;
	bool fullstop = false;
	bool strafe = false;
	float speedFriction = 0.65f;
	float cashedStepHeignt = 0.5f;
	Vec2 smoothMoveVec = Vec2(0.f, 0.f);
	float friction = 0.95f;
	float drag = 7.f;
	Vec3 OldPos = Vec3(0, 0, 0);

public:
	Bhop();
	~Bhop();

	float height = 0.4f;

	SettingEnum mode = SettingEnum(this);
	SettingEnum delayMode = SettingEnum(this);

	bool doDesync = false;
	int desyncMs = 550;
	int desyncPacket = 50;
	float desyncTimer = 30.f;
	bool resetDesyncSetting = false;
	__int64 desyncLastMS;

	inline std::vector<PlayerAuthInputPacket*>* getPlayerAuthInputPacketHolder() { return &PlayerAuthInputPacketHolder; };
	inline std::vector<InventoryTransactionPacket*>* getInventoryPacketHolder() { return &InventoryPacketHolder; };

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(MoveInputHandler* input) override;
	virtual void onEnable() override;
	virtual void onDisable() {
		auto player = Game.getLocalPlayer();
		if (player == nullptr)
			return;
		else
			player->getMaxAutoStepComponent()->stepHeight = cashedStepHeignt;
		if (fullstop) {
			player->location->velocity.x = 0.f;
			player->location->velocity.z = 0.f;
		}
		Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
		doDesync = false;
	};
};
