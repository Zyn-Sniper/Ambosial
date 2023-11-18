#include "AntiRegen.h"

#include "../../../SDK/Utils/ColorUtil.h"
#include "..\..\..\SDK\Attribute.h"

using namespace std;
AntiRegen::AntiRegen() : IModule(0, Category::MISC, "Try stop enemy's regen / Try ban your enemy") {
	addFloat("Range", &range, range, 1, 8);
}

const char* AntiRegen::getModuleName() {
	return ("AntiRegen");
}

bool AntiRegen::findBlock() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	auto prevSlot = supplies->selectedHotbarSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* stack = inv->getItemStack(i);
		if (stack->item != nullptr) {
			if (stack->getItem()->isBlock() && string(stack->getItem()->name.getText()).find("boombox") == string::npos) {
				if (prevSlot != i)
					supplies->selectedHotbarSlot = i;
				return true;
			}
		}
	}
	return false;
}

bool AntiRegen::tryScaffold(Vec3 blockBelow) {
	Vec3 vel = Game.getLocalPlayer()->location->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1
	blockBelow = blockBelow.floor();

	BlockSource* region = Game.getLocalPlayer()->getRegion();
	Block* block = region->getBlock(Vec3i(blockBelow));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->blockId == 0) {
		Vec3i blok(blockBelow);

		// Find neighbour
		static std::vector<Vec3i*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new Vec3i(0, -1, 0));
			checklist.push_back(new Vec3i(0, 1, 0));

			checklist.push_back(new Vec3i(0, 0, -1));
			checklist.push_back(new Vec3i(0, 0, 1));

			checklist.push_back(new Vec3i(-1, 0, 0));
			checklist.push_back(new Vec3i(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			Vec3i calc = blok.sub(*current);
			bool Y = ((region->getBlock(calc)->blockLegacy))->blockId == 0;
			if (!((region->getBlock(calc)->blockLegacy))->blockId == 0) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			auto player = Game.getLocalPlayer();
			Vec3 BlockPos = blockBelow;
			player->level->blockSide = i;
			player->level->block = Vec3i(BlockPos);
			player->level->rayHitType = 0;
			player->level->rayHitVec = BlockPos;
			Game.getGameMode()->buildBlock(&blok, i, false);
			return true;
		}
	}
	return false;
}

void AntiRegen::onEnable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	beforeSlot = supplies->selectedHotbarSlot;
}

void AntiRegen::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	findBlock();

	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	static vector<Vec3i> blocks;
	if (blocks.empty()) {
		for (int x = -range; x <= range; x++) {
			for (int z = -range; z <= range; z++) {
				for (int y = -range; y <= range; y++) {
					if ((*player->getPos()).dist(player->location->pos.add(Vec3(x, y, z))) < range) blocks.push_back(Vec3i(x, y, z));
				}
			}
		}
		// https://www.mathsisfun.com/geometry/pythagoras-3d.html sort = a12 + a22 + ��(x2 + y2 + z2) + an2 ��(a12 + a22 + ��(x2 + y2 + z2) + an2) / 4.f
		sort(blocks.begin(), blocks.end(), [](Vec3i start, Vec3i end) {
			return sqrtf((start.x * start.x) + (start.y * start.y) + (start.z * start.z)) < sqrtf((end.x * end.x) + (end.y * end.y) + (end.z * end.z));
		});
	}

	bool isDestroyed2 = false;

	for (const Vec3i& offset : blocks) {
		blockPos = gm->player->getPos()->add(offset.toVec3t());
		Vec3 blockPos2 = gm->player->getPos()->add(offset.toVec3t());

		string name = gm->player->getRegion()->getBlock(blockPos)->toLegacy()->name.getText();
		if (name == "lit_redstone_ore" || name == "redstone_ore") {
			if (tryScaffold(blockPos.toVec3t().add(0, 1, 0))) return;
			if (tryScaffold(blockPos.toVec3t().add(0, -1, 0))) return;
			if (tryScaffold(blockPos.toVec3t().add(1, 0, 0))) return;
			if (tryScaffold(blockPos.toVec3t().add(-1, 0, 0))) return;
			if (tryScaffold(blockPos.toVec3t().add(0, 0, 1))) return;
			if (tryScaffold(blockPos.toVec3t().add(0, 0, -1))) return;
		}
	}
}

void AntiRegen::onDisable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	supplies->selectedHotbarSlot = beforeSlot;
}
