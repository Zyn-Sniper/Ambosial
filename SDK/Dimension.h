#pragma once

#include "BlockLegacy.h"
#include "../SDK/Utils/HMath.h"

class Entity;
class HashedString;
class Packet;
class Player;
class Weather;

class Dimension {
public:
	BUILD_ACCESS(this, TextHolder, dimensionName, 0x20);
	BUILD_ACCESS(this, int16_t, minHeight, 0xC8);
	BUILD_ACCESS(this, int16_t, maxHeight, 0xCA);
	//BUILD_ACCESS(this, class BrightnessRamp*, brightnessRamp, 0x128);
	BUILD_ACCESS(this, int, dimensionId, 0x160);
	BUILD_ACCESS(this, bool, hasCeiling, 0x165);
	BUILD_ACCESS(this, bool, hasSkylight, 0x167);
	BUILD_ACCESS(this, Weather *, weather, 0x1A8);
	//BUILD_ACCESS(this, __int64, seasons, 0x1B0);

private:
	virtual void Destructor();

public:
	virtual bool isNaturalDimension(void);
	virtual int getDimensionId(void);
	virtual void sendPacketForPosition(Vec3i const &position, Packet const &packet, Player const *except);
	virtual void flushLevelChunkGarbageCollector(void);
	virtual void initializeWithLevelStorageManager(__int64 &levelStorageManager);
	virtual void init(void);
	virtual void tick(void);
	virtual void tickRedstone(void);

private:
	virtual void TryroFunc9();
	virtual void TryroFunc10();
	virtual void TryroFunc11();
	virtual void TryroFunc12();

public:
	virtual bool isValidSpawn(int x, int z);
	//virtual MC_Color getBrightnessDependentFogColor(MC_Color const &, float);
	virtual bool isFoggyAt(int x, int z);
	virtual bool hasPrecipitationFog(void);
	virtual float getCloudHeight(void);
	virtual int getDefaultBiome(void);
	virtual bool mayRespawnViaBed(void);
	virtual bool hasGround(void);
	virtual Vec3i getSpawnPos(void);
	virtual int getSpawnYPosition(void);
	virtual bool hasBedrockFog(void);
	virtual float getClearColorScale(void);
	virtual bool showSky(void);
	virtual bool isDay(void);
	virtual float getSunIntensity(float a, Vec3 const &viewVector, float minInfluenceAngle);
	virtual __int64 forceCheckAllNeighChunkSavedStat(void);

private:
	virtual void TryroFunc29();

public:
	virtual void onChunkLoaded(__int64 &cs, __int64 &lc);
	virtual void deserialize(CompoundTag const &tag);
	virtual void serialize(CompoundTag &serialize);
	virtual void sendBroadcast(Packet const &packet, Player *except);
	virtual bool is2DPositionRelevantForPlayer(Vec3i const &, Player &);
	virtual bool isActorRelevantForPlayer(Player &, Entity const &);
	virtual void onBlockEvent(BlockSource &source, int x, int y, int z, int b0, int b1);
	virtual void onBlockChanged(BlockSource &source, Vec3i const &pos, uint32_t layer, Block const &block, Block const &oldBlock, int updateFlags, __int64 const *syncMsg, __int64 blockChangedEventTarget, Entity *);
	virtual void onLevelDestruction(TextHolder *);
	virtual __int64 *getLightTextureImageBuilder(void);
	virtual const float *getBrightnessRamp(void);
	virtual void startLeaveGame(void);
	virtual float getTimeOfDay(int time, float a);
	virtual __int64 _createChunkBuildOrderPolicy(void);
};

class Weather {
public:
	BUILD_ACCESS(this, float, rainVar1, 0x34);
	BUILD_ACCESS(this, float, rainLevel, 0x38);
	BUILD_ACCESS(this, float, lightningVar1, 0x40);
	BUILD_ACCESS(this, float, lightningLevel, 0x44);
	BUILD_ACCESS(this, float, fogLevel, 0x4C);

	float getRainLevel(float a1) {
		return (rainLevel - rainVar1) * a1 + rainVar1;
	}

	float getLightningLevel(float a1) {
		return (lightningLevel - lightningVar1) * a1 + lightningVar1;
	}

	bool isRaining() {
		return rainLevel > 0.2f;
	}

	bool isLightning() {
		return lightningLevel * rainLevel > 0.89999998f;
	}

	bool isRainingAt(BlockSource* source, Vec3i pos) {
		using isRainingAt = void(__thiscall *)(Weather *, BlockSource *, Vec3i *);
		static isRainingAt isRainingAtFunc = reinterpret_cast<isRainingAt>(FindSignature("48 89 5C 24 ? 57 48 83 EC 20 49 8B D8 48 8B FA E8 ? ? ? ? 84 C0 74 2E 48 8B D3 48 8B CF E8 ? ? ? ? 4C 8B C3 48 8B D7 48 8B C8 E8 ? ? ? ? 0F 2F 05 ? ? ? ? 0F 97"));
		isRainingAtFunc(this, source, &Vec3i(pos));
	}

	bool isSnowingAt(BlockSource* source, Vec3i pos) {
		using isSnowingAt = void(__thiscall *)(Weather *, BlockSource *, Vec3i *);
		static isSnowingAt isSnowingAtFunc = reinterpret_cast<isSnowingAt>(FindSignature("48 89 5C 24 ? 57 48 83 EC 20 49 8B D8 48 8B FA E8 ? ? ? ? 84 C0 74 2E 48 8B D3 48 8B CF E8 ? ? ? ? 4C 8B C3 48 8B D7 48 8B C8 E8 ? ? ? ? 0F 2F 05 ? ? ? ? 0F 96"));
		isSnowingAtFunc(this, source, &Vec3i(pos));
	}

	// Removed Vtable because it's filled with unknown shit pointing to nowhere
};