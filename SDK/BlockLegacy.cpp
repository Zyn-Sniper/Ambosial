#include "BlockLegacy.h"

#include "../Memory/GameData.h"
#include "../SDK/Utils/Logger.h"
#include "../SDK/Utils/Utils.h"

bool BlockLegacy::getCollisionShape(AABB* collShapeOut, Block* block, BlockSource* blockSource, const Vec3i* pos, Entity* actor) {
	return Utils::CallVFunc<5, bool, AABB*, Block*, BlockSource*, const Vec3i*, Entity*>(this, collShapeOut, block, blockSource, pos, 0);
}

void BlockLegacy::liquidGetFlow(Vec3* flowOut, BlockSource* reg, const Vec3i* pos) {
	using liquid_getFlow_t = int(__fastcall*)(BlockLegacy*, Vec3*, BlockSource*, const Vec3i*);
	static liquid_getFlow_t liquid_getDepth = Utils::FuncFromSigOffset<liquid_getFlow_t>(FindSignature("e8 a3 be 4c ce 48"), 1);

	liquid_getDepth(this, flowOut, reg, pos);
}

bool BlockLegacy::hasWater(BlockSource* reg, const Vec3i& pos) {
	if (this->material->isLiquid) {
		return !this->material->isSuperHot;
	}

	auto liquidBlock = reg->getLiquidBlock(pos)->toLegacy();
	return this != liquidBlock && liquidBlock->material->isLiquid && !liquidBlock->material->isSuperHot;
}
