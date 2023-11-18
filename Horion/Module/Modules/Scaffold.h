#pragma once
#include "Module.h"
class Scaffold : public IModule {
private:
	bool autoSelect = false;
	bool down = false;
	bool highlight = true;
	bool hive = false;
	//bool rotations = true;
	int extend = 0;
	int beforeSlot = 0;
	bool Ylock = false;
	bool tryScaffold(Vec3 blockBelow);
	bool tryClutchScaffold(Vec3 blockBelow);
	bool findBlock();
	bool UselessBlocks(ItemStack* itemStack);
	float YCoord;
	Vec3i BlockPos;

	float openAnim = 1.f;
	int openTime = 0;
	int openTimeOffset = 0;

public:
	Scaffold();
	~Scaffold(){};

	bool spoof = false;

	SettingEnum rotations = SettingEnum(this);
	bool isTower = false;
	bool isPlaced = false;

	void handleScaffoldDown(Player* player, float speed, const Vec3& velocity);
	void handleScaffoldUp(Player* player, float speed, const Vec3& velocity);
	Vec3 getBlockBelow(Player* player, float yOffset);
	void adjustYCoordinate(Vec3& blockBelow, const Vec3& blockBelowReal);
	void extendBlock(Player* player, const Vec3& velocity, Vec3& blockBelow);
	void attemptScaffoldWhileMoving(Player* player, float speed, const Vec3& velocity, Vec3& blockBelow, Vec3& blockBelowBelow);
	void handleReplaceableBlock(Player* player, float speed, const Vec3& velocity, Vec3& blockBelow);
	void handleNonReplaceableBlock(Player* player, float speed, const Vec3& velocity, Vec3& blockBelow);
	Vec3 getNextBlock(Player* player, const Vec3& velocity, const Vec3& blockBelow);

	virtual const char* getModuleName() {
		return "Scaffold";
	};
	virtual void onEnable() override;
	virtual void onPostRender(MinecraftUIRenderContext* ctx) override;
	virtual void onSendPacket(Packet* packet) override;
	virtual void onPlayerTick(Player* player) override;
	virtual void onDisable() override;
};
