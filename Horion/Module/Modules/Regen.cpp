#include "Regen.h"

#include "../../../SDK/Utils/ColorUtil.h"
#include "..\..\..\SDK\Attribute.h"
#include "..\..\..\SDK\Utils\Animation\AnimationUtil.h"
#include "..\..\..\SDK\Utils\Render\RenderUtil.h"

bool haspickaxe = false;

//Crazy Regen

using namespace std;
Regen::Regen() : IModule(0, Category::COMBAT, "Regenerates your health") {
	mode.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("HiveSingle", 1));
	addEnum("Mode", &mode);
	addFloat("Range", &range, range, 1, 8);
	addInt("Ticks", &delay, delay, 0, 10);
	//addBool("Rotation", &rotation, rotation);
	addBool("Swing", &swing, swing);
	addBool("Render", &render, render);
	addBool("DigAround", &digAround, digAround);
	//addBool("ShowRotation", &showrotation, showrotation);
}

const char* Regen::getModuleName() {
	return ("Regen");
}

bool Regen::selectPickaxe() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	auto prevSlot = supplies->selectedHotbarSlot;

	for (int n = 0; n < 36; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->isPickaxe()) {
				if (prevSlot != n) {
					supplies->selectedHotbarSlot = n;
				}
				return true;
			}
		}
	}
	return false;
}

bool Regen::hasPickaxe() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	auto prevSlot = supplies->selectedHotbarSlot;

	for (int n = 0; n < 36; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->isPickaxe()) {
				return true;
			}
		}
	}
	return false;
}

bool Regen::isFilled(Vec3i blockPos) {
	bool isFilled = true;
	static std::vector<Vec3i*> checklist;
	if (checklist.empty()) {
		checklist.push_back(new Vec3i(0, -1, 0));
		checklist.push_back(new Vec3i(0, 1, 0));

		checklist.push_back(new Vec3i(0, 0, -1));
		checklist.push_back(new Vec3i(0, 0, 1));

		checklist.push_back(new Vec3i(-1, 0, 0));
		checklist.push_back(new Vec3i(1, 0, 0));
	}

	for (auto current : checklist) {
		Vec3i calc = blockPos.add(*current);
		if (!Game.getLocalPlayer()->getRegion()->getBlock(calc)->blockLegacy->isSolid) {
			isFilled = false;
			break;
		}
	}
	return isFilled;
}

Vec3i Regen::findAroundBlock(Vec3i blockPos) {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;

	ItemStack* stack = inv->getItemStack(1);
	for (int n = 0; n < 36; n++) {
		stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->isPickaxe()) {
				//pickaxe = stack;
				break;
			}
		}
	}
	ItemStack* pickaxe = stack;

	static std::vector<Vec3i*> checklist;
	if (checklist.empty()) {
		checklist.push_back(new Vec3i(0, -1, 0));
		checklist.push_back(new Vec3i(0, 1, 0));

		checklist.push_back(new Vec3i(0, 0, -1));
		checklist.push_back(new Vec3i(0, 0, 1));

		checklist.push_back(new Vec3i(-1, 0, 0));
		checklist.push_back(new Vec3i(1, 0, 0));
	}

	long long MiningSpeed = 10000;
	Vec3i block;
	for (auto current : checklist) {
		ItemStack* stack = pickaxe;
		Vec3i calc = blockPos.add(*current);
		auto currentSpeed = stack->getItem()->getDestroySpeed(*pickaxe, *Game.getLocalPlayer()->getRegion()->getBlock(calc));
		if (currentSpeed < MiningSpeed) {
			MiningSpeed = currentSpeed;
			block = calc;
		}
	}
	return block;
}

void Regen::onEnable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	rot = false;
	firstbreak = false;
	continuemine = false;
	canattack = true;
	isregen = false;
	enabledticks = 0;

	openAnim = -500;
	openTimeOffset = 0;
	openTime = 0;

	//delay = 10;

	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	slot = supplies->selectedHotbarSlot;
}

void Regen::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	auto bhop = moduleMgr->getModule<Bhop>();
	auto fly = moduleMgr->getModule<Fly>();
	if (player == nullptr || moduleMgr->getModule<Scaffold>()->isEnabled() || bhop->doDesync || bhop->getPlayerAuthInputPacketHolder()->size() > 0 /* || fly->isDamaged || fly->getPlayerAuthInputPacketHolder()->size() > 0 */) {
		isregen = false;
		return;
	}
	enabledticks++;

	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	static AbsorptionAttribute attribute = AbsorptionAttribute();
	//auto timerUtil = new TimerUtil();
	static vector<Vec3i> blocks;
	haspickaxe = true;
	if (Game.canUseMoveKeys()) {
		switch (mode.selected) {
		case 0:  // Vanilla
			//No codes
			break;
		case 1:  // FlareonSingle
		{
			//Check Absorpription Health Check
			if (gm->player->getMutableAttribute(&attribute)->currentValue >= 10) {
				isregen = false;
				if (continuemine) gm->stopDestroyBlock(tempblockPos);
				continuemine = false;
				canattack = true;
				enabledticks = 0;
				rot = false;
				destroy = false;
				return;
			} else {
				isregen = true;
			}

			if (enabledticks > delay) {
				isregen = true;
			} else {
				isregen = false;
				canattack = false;
			}

			if (blocks.empty()) {
				for (int x = -range; x <= range; x++) {
					for (int z = -range; z <= range; z++) {
						for (int y = -range; y <= range; y++) {
							if ((*player->getPos()).dist(player->location->pos.add(Vec3(x, y, z))) < range) blocks.push_back(Vec3i(x, y, z));
						}
					}
				}
				// https://www.mathsisfun.com/geometry/pythagoras-3d.html sort = a12 + a22 + √(x2 + y2 + z2) + an2 √(a12 + a22 + √(x2 + y2 + z2) + an2) / 4.f
				sort(blocks.begin(), blocks.end(), [](Vec3i start, Vec3i end) {
					return sqrtf((start.x * start.x) + (start.y * start.y) + (start.z * start.z)) < sqrtf((end.x * end.x) + (end.y * end.y) + (end.z * end.z));
				});
			}

			bool isDestroyed2 = false;
			bool hasSafeRedstone = false;

			for (const Vec3i& offset : blocks) {
				blockPos = gm->player->getPos()->add(offset.toVec3t());

				string name = gm->player->getRegion()->getBlock(blockPos)->blockLegacy->name.getText();
				if (name != "lit_redstone_ore" && name != "redstone_ore" && !digAround) continue;
				if (!isFilled(blockPos) && (name == "lit_redstone_ore" || name == "redstone_ore")) {
					hasSafeRedstone = true;
					break;
				}
			}

			for (const Vec3i& offset : blocks) {
				blockPos = gm->player->getPos()->add(offset.toVec3t());
				destroy = false;

				string name = gm->player->getRegion()->getBlock(blockPos)->blockLegacy->name.getText();
				if (name != "lit_redstone_ore" && name != "redstone_ore" && !digAround) continue;
				bool isSafe = !isFilled(blockPos);
				if (continuemine) {
					if (tempblockPos == blockPos.toVec3t()) {
						if ((name == "lit_redstone_ore" || name == "redstone_ore") && isSafe) {
							destroy = true;
							tempblockPos = blockPos.toVec3t();
						} else {
							if (digAround) {
								destroy = true;
								tempblockPos = blockPos.toVec3t();
							}
						}
					}
				} else {
					if (name == "lit_redstone_ore" || name == "redstone_ore") {
						if (isSafe) {
							destroy = true;
							gm->startDestroyBlock(blockPos, 1, isDestroyed2);
							continuemine = true;
							tempblockPos = blockPos.toVec3t();
						} else if (digAround && !hasSafeRedstone) {
							Vec3i aroundBlock = blockPos.add(0, 1, 0);
							destroy = true;
							gm->startDestroyBlock(aroundBlock, 1, isDestroyed2);
							continuemine = true;
							tempblockPos = aroundBlock.toVec3t();
						} else
							continue;
					}
				}

				if (destroy) {
					slot = supplies->selectedHotbarSlot;
					static bool restored = false;
					if (!selectPickaxe()) {
						if (!restored)
							restored = true;
						else {
							//auto warning = Game.addNotification("Regen", "No Pickaxe Found"); warning->duration = 3;
							//setEnabled(false);
							haspickaxe = false;
							break;
						}
						return;
					}
					restored = false;
					if (isregen) {
						rot = true;
					}
					if (enabledticks > delay + 2) {
						if (isSafe || (name != "lit_redstone_ore" && name != "redstone_ore" && digAround)) gm->destroyBlock(&blockPos, 0);
						hasDestroyed = true;
						enabledticks = 0;
						gm->stopDestroyBlock(blockPos);
						continuemine = false;
						rot = false;
					}
					supplies->selectedHotbarSlot = slot;
					return;
				}

				if (hasDestroyed) {
					hasDestroyed = false;
				}
			}

			if (!destroy) {
				isregen = false;
				if (continuemine) gm->stopDestroyBlock(tempblockPos);
				continuemine = false;
				canattack = true;
				enabledticks = 0;
				rot = false;
			} else {
				canattack = true;
			}
		} break;
		}
	}
}

int cool = 0;

void Regen::onPostRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	cool++;
	if (cool >= 50) cool = 0;

	int percentage = (enabledticks * 100) / delay; if (percentage > 100) percentage = 100;

	float animTime = min(openTime, 1200) / 1200.f;
	animTime = easeOutElastic(animTime);  // Elastic // Bounce// Expo . Quint // Circ

	static AbsorptionAttribute attribute = AbsorptionAttribute();
	float renderx = Game.getClientInstance()->getGuiData()->windowSize.x / 2;
	float rendery = roundf(Game.getGuiData()->heightGame * 0.75f) + 0.5f;

	string rendertext = to_string(int(percentage)) + "%";
	float textLen = RenderUtil::getTextLen(&rendertext, 1.05 * animTime, MOJANG) / 2;
	rendery -= textLen;

	Vec2 renderpos = Vec2(renderx, rendery - 40);

	Vec4 plusRect = Vec4(renderx - 27.5, rendery + 6, 27.5 + renderx, rendery + 16); //55
	Vec4 Rect = Vec4(plusRect.x - 15, plusRect.y, plusRect.z + 15, plusRect.y);
	Vec4 SmoothShit = Vec4(plusRect.x - 15, plusRect.y, plusRect.x + cool * 1.7, plusRect.y);
	if (render) {
		if (haspickaxe) {
			if (player->getMutableAttribute(&attribute)->currentValue < 10 && destroy) {
				// float textLen = DrawUtils::getTextWidth(&rendertext, 1) / 2;
				RenderUtil::RenderRoundRectangle(Rect, MC_Color(19, 19, 19, 150));
				RenderUtil::RenderRoundRectangle(SmoothShit, ColorUtil::ClientColor(1));
				// ImGuiUtils::RenderGlow(Rect);
				Vec4 gloPos = Vec4(SmoothShit.x + 4, SmoothShit.y + 2, SmoothShit.z - 4, SmoothShit.w - 2);
				DrawUtils::drawGlow(gloPos, MC_Color(ColorUtil::ClientColor(1).r, ColorUtil::ClientColor(1).g, ColorUtil::ClientColor(1).b, 50.f / 6), 0.05, 6, 4);
				RenderUtil::drawText(renderpos, &rendertext, ColorUtil::ClientColor(0), 1.05 * animTime, 1, true, MOJANG);
				// if (player->getMutableAttribute(&attribute)->currentValue < 10 && destroy) DrawUtils::drawText(renderpos, &rendertext, ColorUtil::ClientColor(0), 1, 1);
				float size = 20 / (float)fmax(1, blockPos.toVec3t().add(0.5f, 0.5f, 0.5f).dist(Game.getClientInstance()->levelRenderer->getOrigin()));
				Vec2 pos = DrawUtils::worldToScreen(blockPos.toVec3t().add(0.5f, 0.5f, 0.5f));
				if (0 < pos.x && 0 < pos.y) DrawUtils::drawCircleFilled(pos, Vec2(size, size), MC_Color(ColorUtil::ClientColor(1).r, ColorUtil::ClientColor(1).g, ColorUtil::ClientColor(1).b, 0.5), 0.5);
			} else {
				openAnim = -500;
				openTimeOffset = 0;
				openTime = 0;
			}
		} else {
			openAnim = -500;
			openTimeOffset = 0;
			openTime = 0;
			//string nopickaxe = "No Pickaxe Found!";
			//float textLen = DrawUtils::getTextWidth(&nopickaxe, 1) / 2;
			//renderpos.x -= textLen;
			//DrawUtils::drawText(renderpos, &nopickaxe, MC_Color(255, 0, 0), 1, 1);
		}
	}
}

void Regen::onPlayerTick(Player* player) {
	if (player == nullptr) return;

	if (openTimeOffset == 0)
		openTimeOffset = TimerUtil::getCurrentMs();
	if (openAnim < 150) openAnim += (28 - openAnim) * 0.09f;
	openTime = TimerUtil::getCurrentMs() - openTimeOffset;

	if (isregen) {
		Vec2 regenRot = player->getPos()->CalcAngle(Vec3(blockPos.x, blockPos.y, blockPos.z));
		//Rotation code :)
	}
}

void Regen::onSendPacket(Packet* packet) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
		auto* movePacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		Vec2 regenRot = player->getPos()->CalcAngle(Vec3(blockPos.x, blockPos.y, blockPos.z));
		if (mode.selected == 2) return;
		if (isregen) {
			movePacket->yaw = regenRot.y;
			movePacket->yawUnused = regenRot.y;
			movePacket->pitch = regenRot.x;
		}
	}
}

void Regen::onDisable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	destroy = false;
	isregen = false;
	rot = false;
	canattack = true;
}
