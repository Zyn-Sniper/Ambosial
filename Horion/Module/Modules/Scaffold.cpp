#include "Scaffold.h"

#include "../../../SDK/Utils/ColorUtil.h"
#include "../../../SDK/Utils/Logger.h"
#include "../../../SDK/Utils/Render/RenderUtil.h"
#include "../../DrawUtils.h"
#include "../../Module/ModuleManager.h"
#include "../../../SDK/Utils/Animation/AnimationUtil.h"

int blockSide = 0;

using namespace std;
Scaffold::Scaffold() : IModule(VK_NUMPAD1, Category::WORLD, "Automatically build blocks beneath you") {
	rotations.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("Legit", 1))
		.addEntry(EnumEntry("Down", 2));

	//selectType.addEntry(EnumEntry("Switch", 0));
	//selectType.addEntry("Spoof", 1);
	//selectType.addEntry("Fake", 2);

	addEnum("Rotations", &rotations);
	addBool("Spoof", &spoof, spoof);
	addBool("Zipline+", &down, down);
	addBool("Highlight", &highlight, highlight);
	addBool("Aura Bypass", &hive, hive);
	//addBool("Rotations", &rotations, rotations);
	addBool("Y Lock", &Ylock, Ylock);
	addInt("Extend", &extend, extend, 0, 8);
}

void Scaffold::onEnable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	float yPos = player->getRenderPositionComponent()->renderPos.y;  // Block 1 block below the player
	yPos -= player->getAABBShapeComponent()->aabb.height;
	YCoord = yPos;
	if (down) YCoord += 2;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	beforeSlot = supplies->selectedHotbarSlot;

	openAnim = -500;
	openTimeOffset = 0;
	openTime = 0;
}

bool Scaffold::tryScaffold(Vec3 blockBelow) {
	Vec3 vel = Game.getLocalPlayer()->location->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1
	blockBelow = blockBelow.floor();

	DrawUtils::setColor(1.f, 1.f, 1.f, 0.05f);                                           // white when placing all the time
	if (highlight) {
		float size = 20 / (float)fmax(1, blockBelow.add(0.5f, 0.5f, 0.5f).dist(Game.getClientInstance()->levelRenderer->getOrigin()));
		Vec2 pos = DrawUtils::worldToScreen(blockBelow.add(0.5f, 0.5f, 0.5f));
		if (0 < pos.x && 0 < pos.y) DrawUtils::drawCircleFilled(pos, Vec2(size, size), ColorUtil::ClientColor(1), 0.35);
		//DrawUtils::drawBox(blockBelow, Vec3(blockBelow).add(1), 0.4f, true);  // Draw a box around the block about to be placed
	}

	if (Game.getLocalPlayer()->getRegion()->getBlock(Vec3i(blockBelow))->blockLegacy->blockId == 0) {
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
			if (Game.getLocalPlayer()->getRegion()->getBlock(calc)->blockLegacy->blockId != 0) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		BlockPos = Vec3i(blockBelow);
		if (foundCandidate) {
			if (rotations.selected == 1) {
				auto player = Game.getLocalPlayer();
				//BlockPos = blockBelow; no work
				player->level->blockSide = i;
				blockSide = i;
				player->level->block = BlockPos;
				player->level->rayHitType = 0;
				player->level->rayHitVec = BlockPos.toVec3t();
			}
			Game.getGameMode()->buildBlock(&blok, i, true);
			return true;
		}
	}
	return false;
}

bool Scaffold::tryClutchScaffold(Vec3 blockBelow) {
	Vec3 vel = Game.getLocalPlayer()->location->velocity;
	vel = vel.normalize();  // Only use values from 0 - 1
	blockBelow = blockBelow.floor();

	DrawUtils::setColor(0.f, 0.f, 1.f, 1.f);                                       // blue when predicting
	if (highlight) {
		float size = 20 / (float)fmax(1, blockBelow.add(0.5f, 0.5f, 0.5f).dist(Game.getClientInstance()->levelRenderer->getOrigin()));
		Vec2 pos = DrawUtils::worldToScreen(blockBelow.add(0.5f, 0.5f, 0.5f));
		if (0 < pos.x && 0 < pos.y) DrawUtils::drawCircleFilled(pos, Vec2(size, size), ColorUtil::ClientColor(1), 0.35);
		// DrawUtils::drawBox(blockBelow, Vec3(blockBelow).add(1), 0.4f, true);  // Draw a box around the block about to be placed
	}

	static std::vector<Vec3i> checkBlocks;
	if (checkBlocks.empty()) {  // Only re sort if its empty
		for (int y = -4; y <= 4; y++) {
			for (int x = -4; x <= 4; x++) {
				for (int z = -4; z <= 4; z++) {
					checkBlocks.push_back(Vec3i(x, y, z));
				}
			}
		}
		// https://www.mathsisfun.com/geometry/pythagoras-3d.html c2 = x2 + y2 + z2 funny
		std::sort(checkBlocks.begin(), checkBlocks.end(), [](Vec3i first, Vec3i last) {
			return sqrtf((float)(first.x * first.x) + (float)(first.y * first.y) + (float)(first.z * first.z)) < sqrtf((float)(last.x * last.x) + (float)(last.y * last.y) + (float)(last.z * last.z));
		});
	}

	for (const Vec3i& blockOffset : checkBlocks) {
		Vec3i currentBlock = Vec3i(blockBelow).add(blockOffset);

		// Normal tryScaffold after it sorts
		if (Game.getLocalPlayer()->getRegion()->getBlock(Vec3i(currentBlock))->blockLegacy->blockId == 0) {
			Vec3i blok(currentBlock);

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
				//bool Y = ((region->getBlock(calc)->blockLegacy))->material->isReplaceable;
				if (Game.getLocalPlayer()->getRegion()->getBlock(calc)->blockLegacy->blockId != 0) {
					// Found a solid block to click
					foundCandidate = true;
					blok = calc;
					break;
				}
				i++;
			}
			BlockPos = Vec3i(currentBlock);
			if (foundCandidate) {
				if (rotations.selected == 1) {
					auto player = Game.getLocalPlayer();
					//BlockPos = blockBelow;no work
					player->level->blockSide = i;
					blockSide = i;
					player->level->block = BlockPos;
					player->level->rayHitType = 0;
					player->level->rayHitVec = BlockPos.toVec3t();
				}
				Game.getGameMode()->buildBlock(&blok, i, true);
				return true;
			}
		}
	}
	return false;
}

bool Scaffold::UselessBlocks(ItemStack* itemStack) {
	std::vector<std::string> uselessSubstrings = {"_button", "chest", "boom", "vine", "pressure_plate", "fence", "_wall", "_stairs", "_table", "furnace", "trapdoor", "command_block", "torch", "carpet", "coral_fan"};
	std::vector<std::string> uselessNames = {"cake", "ladder", "tnt", "lever", "loom", "scaffolding", "web", "sand", "gravel", "dragon_egg", "anvil", "sapling", "grass", "string"};
	std::string itemName = itemStack->getItem()->name.getText();
	for (auto substring : uselessSubstrings) {
		if (itemName.find(substring) != std::string::npos) {
			return false;
		}
	}
	for (auto name : uselessNames) {
		if (itemName == name) {
			return false;
		}
	}
	return true;
}

bool Scaffold::findBlock() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	auto prevSlot = supplies->selectedHotbarSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* stack = inv->getItemStack(i);
		if (stack->item != nullptr) {
			if (stack->getItem()->isBlock() && UselessBlocks(stack)) {
				if (prevSlot != i)
					supplies->selectedHotbarSlot = i;
				return true;
			}
		}
	}
	return false;
}

void Scaffold::onPostRender(MinecraftUIRenderContext* ctx) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr || !Game.canUseMoveKeys()) {
		return;
	}

	ItemStack* itemStack;

	// DrawItem
	if (!findBlock()) {
		openAnim = -500;
		openTimeOffset = 0;
		openTime = 0;
	}

	if (findBlock()) {
		PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
		//ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
		Inventory* inv = supplies->inventory;
		int totalCount = 0;

		for (int s = 0; s < 9; s++) {
			ItemStack* stack = inv->getItemStack(s);
			if (stack->item != nullptr && stack->getItem()->isBlock()) {
				totalCount += stack->count;
			}
		}

		float animTime = min(openTime, 1000) / 1000.f;
		animTime = easeOutExpo(animTime);  // Elastic // Bounce// Expo . Quint // Circ

		float renderx = Game.getClientInstance()->getGuiData()->windowSize.x / 2;
		float rendery = roundf(Game.getGuiData()->heightGame * 0.75f) + 0.5f;
		Vec2 textPosZephyr = Vec2(renderx + 4, (rendery - 33.f) * animTime);
		Vec4 plusRect = Vec4(renderx, (rendery + 6) * animTime, 55 + renderx, (rendery + 16) * animTime);

		std::string Count = std::to_string(totalCount);
		std::string text = "  Blocks " + (Count);
		float textLen = RenderUtil::getTextLen(&text, 1.3, NOTOSANS) / 2;
		textPosZephyr.x -= textLen;
		plusRect.x -= textLen;
		Vec4 rectangleRect = Vec4(plusRect.x + 6, plusRect.y - 39, plusRect.z - 22, plusRect.w - 39);
		Vec4 wow = Vec4(50, 50, 50, 25);
		RenderUtil::drawText(textPosZephyr, &text, MC_Color(255, 255, 255), 1.3, animTime, false, NOTOSANS);  // MC
		Vec4 gloPos = Vec4(rectangleRect.x + 4, rectangleRect.y + 2, rectangleRect.z - 4, rectangleRect.w - 2);
		RenderUtil::RenderRoundRectangle(rectangleRect, MC_Color(0, 0, 0, 70));
		DrawUtils::drawGlow(gloPos, MC_Color(0, 0, 0, 50 / 6), 0.05, 6, 4);
	}

	auto selectedItem = player->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr || !selectedItem->getItem()->isBlock())) {
		return;
	}

	float speed = player->location->velocity.magnitudexz();
	Vec3 velocity = player->location->velocity.normalize();
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
	if (GameData::isKeyDown(*input->spaceBarKey) && 0.05f > speed) {
		Vec3i pos = *player->getPos();
		float yPos = player->getRenderPositionComponent()->renderPos.y;  // Block 1 block below the player
		yPos -= player->getAABBShapeComponent()->aabb.width;
		//yPos -= 2.f;
		YCoord = yPos;
		player->location->velocity.y = 0.35f;
		isTower = true;
		if (player->getRegion()->getBlock(Vec3i(pos.x, yPos, pos.z))->blockLegacy->blockId != 0) {
		}
	} else {
		if (isTower) player->location->velocity.y = 0.f;
		isTower = false;
	}
	handleScaffoldUp(player, speed, velocity);

	/*if (down) {
		handleScaffoldDown(player, speed, velocity);
	} else {
		handleScaffoldUp(player, speed, velocity);
	}*/
}

void Scaffold::handleScaffoldDown(Player* player, float speed, const Vec3& velocity) {
	Vec3 blockBelow = getBlockBelow(player, 2.5f);
	Vec3 blockBelowBelow = getBlockBelow(player, 3.f);

	if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
		if (speed > 0.05f) {
			attemptScaffoldWhileMoving(player, speed, velocity, blockBelow, blockBelowBelow);
		}
	}
}

void Scaffold::handleScaffoldUp(Player* player, float speed, const Vec3& velocity) {
	Vec3 blockBelowReal = getBlockBelow(player, 2.f);
	Vec3 blockBelow = blockBelowReal;

	if (Ylock) {
		adjustYCoordinate(blockBelow, blockBelowReal);
	}

	if (!down) extendBlock(player, velocity, blockBelow);

	if (player->getRegion()->getBlock(Vec3i(blockBelow.floor()))->blockLegacy->blockId == 0) {
		handleReplaceableBlock(player, speed, velocity, blockBelow);
	} else {
		handleNonReplaceableBlock(player, speed, velocity, blockBelow);
	}
}

Vec3 Scaffold::getBlockBelow(Player* player, float yOffset) {
	Vec3 blockBelow = player->getRenderPositionComponent()->renderPos;
	blockBelow.y -= player->getAABBShapeComponent()->aabb.width + yOffset;
	if (down) blockBelow.y += 2;
	//blockBelow.y -= yOffset;
	return blockBelow;
}

void Scaffold::adjustYCoordinate(Vec3& blockBelow, const Vec3& blockBelowReal) {
	if (isTower || down) return;
	blockBelow.y = YCoord;
	if (blockBelowReal.y < YCoord) {
		YCoord = blockBelowReal.y;
	}
}

void Scaffold::extendBlock(Player* player, const Vec3& velocity, Vec3& blockBelow) {
	blockBelow.x += velocity.x * extend;
	blockBelow.z += velocity.z * extend;
}

void Scaffold::attemptScaffoldWhileMoving(Player* player, float speed, const Vec3& velocity, Vec3& blockBelow, Vec3& blockBelowBelow) {
	blockBelow.z -= velocity.z * 0.4f;
	blockBelowBelow.z -= velocity.z * 0.4f;

	if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow)) {
		blockBelow.x -= velocity.x * 0.4f;
		blockBelowBelow.x -= velocity.x * 0.4f;

		if (!tryScaffold(blockBelow) && !tryScaffold(blockBelowBelow) && player->isSprinting()) {
			blockBelow.z += velocity.z;
			blockBelow.x += velocity.x;
			blockBelowBelow.z += velocity.z;
			blockBelowBelow.x += velocity.x;

			tryScaffold(blockBelow);
			tryScaffold(blockBelowBelow);
		}
	}
}

void Scaffold::handleReplaceableBlock(Player* player, float speed, const Vec3& velocity, Vec3& blockBelow) {
	tryClutchScaffold(blockBelow);

	if (hive && !isTower) {
		Vec3 nextBlock = blockBelow;
		for (int n = 0; n < extend; n++) {
			nextBlock = getNextBlock(player, velocity, nextBlock);
			tryClutchScaffold(nextBlock);
		}
	}
}

void Scaffold::handleNonReplaceableBlock(Player* player, float speed, const Vec3& velocity, Vec3& blockBelow) {
	if (!hive) {
		if (!tryScaffold(blockBelow)) {
			if (speed > 0.05f) {
				blockBelow.z -= velocity.z * 0.4f;

				if (!tryScaffold(blockBelow)) {
					blockBelow.x -= velocity.x * 0.4f;

					if (!tryScaffold(blockBelow) && player->isSprinting()) {
						blockBelow.z += velocity.z;
						blockBelow.x += velocity.x;

						tryScaffold(blockBelow);
					}
				}
			}
		}
	}
}

Vec3 Scaffold::getNextBlock(Player* player, const Vec3& velocity, const Vec3& blockBelow) {
	Vec3 nextBlock = blockBelow;
	if (abs(velocity.x) > abs(velocity.z)) {
		nextBlock.x += (velocity.x > 0 ? 1 : (velocity.x < 0 ? -1 : 0));
	} else {
		nextBlock.z += (velocity.z > 0 ? 1 : (velocity.z < 0 ? -1 : 0));
	}
	return nextBlock;
}

void Scaffold::onSendPacket(Packet* packet) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	float speed = player->location->velocity.magnitudexz();
	Vec3 blockBelow = player->getRenderPositionComponent()->renderPos;  // Block 1 block below the player
	blockBelow.y -= player->getAABBShapeComponent()->aabb.width;
	//blockBelow.y -= 2.f;

	if (rotations.selected == 0 && packet->isInstanceOf<PlayerAuthInputPacket>() || rotations.selected == 0 && packet->isInstanceOf<C_MovePlayerPacket>()) {
		Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(BlockPos.toVec3t());
		auto* inputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
		inputPacket->pitch = angle.x;
		inputPacket->yawUnused = angle.y;
		inputPacket->yaw = angle.y;
		movePacket->pitch = angle.x;
		movePacket->yaw = angle.y;
		movePacket->headYaw = angle.y;
	}
}

void Scaffold::onPlayerTick(Player* player) {
	if (player == nullptr) return;
	if (spoof) {
		Game.getClientInstance()->minecraft->setRenderTimerSpeed(0.f);
	}
	if (openTimeOffset == 0)
		openTimeOffset = TimerUtil::getCurrentMs();
	if (openAnim < 150) openAnim += (28 - openAnim) * 0.09f;
	openTime = TimerUtil::getCurrentMs() - openTimeOffset;
	if (rotations.selected == 0) {
		Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(BlockPos.toVec3t());
		player->getActorRotationComponent()->rot.x = angle.x;
		player->getMovementProxy()->setYHeadRot(angle.y);
		player->getMobBodyRotationComponent()->bodyRot = angle.y;
	}
	if (rotations.selected == 1) {
		player->level->blockSide = blockSide;
		player->level->block = BlockPos;
		player->level->rayHitType = 0;
		player->level->rayHitVec = BlockPos.toVec3t();
		player->getActorRotationComponent()->rot.x = 70;
		//player->pitch = 70;
	}
	if (rotations.selected == 2) {
		player->level->rayHitType = 0;
		player->getActorRotationComponent()->rot.x = 90;
		//player->pitch = 90;
	}
	float speed = player->location->velocity.magnitudexz();
	Vec3 blockBelow = player->getRenderPositionComponent()->renderPos;  // Block 1 block below the player
	blockBelow.y -= player->getAABBShapeComponent()->aabb.width;
	//blockBelow.y -= 2.f;

	/* if (speed > 0.05f) {
		Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(blockBelow);
		//player->pitch = 80;
	}*/
}

void Scaffold::onDisable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
	Game.getClientInstance()->minecraft->setRenderTimerSpeed(20.f);
	supplies->selectedHotbarSlot = beforeSlot;

	openTimeOffset = 0;
	openTime = 0;
}
