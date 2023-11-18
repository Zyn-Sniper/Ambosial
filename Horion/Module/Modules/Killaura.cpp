#include "Killaura.h"

#include "Regen.h"

using namespace std;
Killaura::Killaura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically.") {
	rotations.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Normal", 1))
		.addEntry(EnumEntry("Smooth", 2));
	addEnum("Rotations", &rotations);
	visualizeMode.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Circle", 1))
		.addEntry(EnumEntry("Box", 2))
		.addEntry(EnumEntry("Test", 3));
	addEnum("Visualize", &visualizeMode);
	addBool("MultiAura", &isMulti, isMulti);
	addFloat("Range", &range, range, 2.f, 20.f);
	addFloat("Smoothing", &smoothing, smoothing, 2.f, 100.f);
	addInt("CPS", &delay, delay, 1, 20);
	addBool("hurttime", &hurttime, hurttime);
	addBool("AutoWeapon", &autoweapon, autoweapon);
	addBool("Strafe", &strafe, strafe);
	addBool("Randomize", &randomize, randomize);
}

Killaura::~Killaura() {
}

const char* Killaura::getModuleName() {
	return ("Killaura");
}

static std::vector<Entity*> targetList;
void findEntity(Entity* currentEntity, bool isRegularEntity) {
	static auto killauraMod = moduleMgr->getModule<Killaura>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
		return;

	if (!Game.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!Game.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 66)  // falling block
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());

	if (dist < killauraMod->range) {
		targetList.push_back(currentEntity);
	}
}

struct CompareTargetEnArray {
	bool operator()(Entity* lhs, Entity* rhs) {
		LocalPlayer* localPlayer = Game.getLocalPlayer();
		return (lhs->getRenderPositionComponent()->renderPos).dist(localPlayer->getRenderPositionComponent()->renderPos) < (rhs->getRenderPositionComponent()->renderPos).dist(localPlayer->getRenderPositionComponent()->renderPos);
	}
};

void Killaura::onEnable() {
	if (Game.getLocalPlayer() == nullptr)
		setEnabled(false);
	targetList.clear();
}

void Killaura::findWeapon() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

void Killaura::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) {
		targetList.clear();
		return;
	}
	if (moduleMgr->getModule<Regen>()->isEnabled() && moduleMgr->getModule<Regen>()->isregen) return;
	targetListEmpty = targetList.empty();
	//Loop through all our players and retrieve their information
	targetList.clear();

	Game.forEachEntity(findEntity);
	std::sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
	if (!targetList.empty()) {
		if (autoweapon) findWeapon();

		if (strafe) {
			//angle = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		}
		TimerUtil* timerUtil = new TimerUtil;
		if (timerUtil->hasTimedElapsed((1000 / delay), true)) {
			player->level->rayHitType = 1;
			// Attack all entitys in targetList
			if (isMulti) {
				for (auto& i : targetList) {
					Game.getLocalPlayer()->swing();
					Game.getGameMode()->attack(i);
				}
			} else {
				Game.getLocalPlayer()->swing();
				Game.getGameMode()->attack(targetList[0]);
			}
		}
	}
}

void Killaura::onPlayerTick(Player* plr) {
	if (moduleMgr->getModule<Regen>()->isEnabled() && moduleMgr->getModule<Regen>()->isregen) return;
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if (!targetList.empty()) {
		Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		if (strafe) {
			player->getMovementProxy()->setYHeadRot(angle.y);
			player->getMobBodyRotationComponent()->bodyRot = angle.y;
		}
		if (randomize) {
			float xRandom = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 10));
			float yRandom = static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 10));
			if (10 >= 0.f) {
				yRandom += 1.75f;
				xRandom -= 3.5f;
			}
			angle.x += yRandom;
			angle.y += xRandom;
		}
		switch (rotations.selected) {
		case 1: {
			/*player->pitch = angle.x;
			player->yawUnused1 = angle.y;
			player->bodyYaw = angle.y;*/
			animYaw = angle.y;
			animPitch = angle.x;
			player->getActorRotationComponent()->rot.x = animPitch;
			player->getMovementProxy()->setYHeadRot(animYaw);
			player->getMobBodyRotationComponent()->bodyRot = animYaw;
		} break;
		case 2: {
			if (fabsf(angle.y - animYaw) < 180) {
				animYaw = easingEaseOutCubic(1, animYaw, angle.y - animYaw, smoothing);
			} else {
				float angle2;
				float animYaw2;
				if (0 <= angle.y)
					angle2 = 180 - angle.y;
				else
					angle2 = -180 - angle.y;
				if (0 <= animYaw)
					animYaw2 = 180 - animYaw;
				else
					animYaw2 = -180 - animYaw;
				animYaw2 = easingEaseOutCubic(1, animYaw2, angle2 - animYaw2, smoothing);
				if (0 <= animYaw2)
					animYaw = 180 - animYaw2;
				else
					animYaw = -180 - animYaw2;
			}
			animPitch = easingEaseOutCubic(1, animPitch, angle.x - animPitch, smoothing);
			player->getActorRotationComponent()->rot.x = animPitch;
			player->getMovementProxy()->setYHeadRot(animYaw);
			player->getMobBodyRotationComponent()->bodyRot = animYaw;
			/*player->pitch = animPitch;
			player->yawUnused1 = animYaw;
			player->bodyYaw = animYaw;*/
		}
		}
	}
}

void Killaura::onLevelRender() {
	if (moduleMgr->getModule<Regen>()->isEnabled() && moduleMgr->getModule<Regen>()->isregen) return;
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	static float n = 0;
	static float anim = 0;
	if (!targetList.empty() && visualizeMode.selected == 1) {
		anim++;
		float range = 0.8f;
		Vec3 permutations[56];
		for (int i = 0; i < 56; i++) {
			permutations[i] = {sinf((i * 10.f) / (180 / PI)) * range, 0.f, cosf((i * 10.f) / (180 / PI)) * range};
		}

		const float coolAnim = 0.9f + 0.9f * sin((anim / 60) * PI);
		Vec3 pos = targetList[0]->getRenderPositionComponent()->renderPos;

		auto yPos = pos.y;
		yPos -= 1.6f;
		yPos += coolAnim;

		vector<Vec3> posList;
		posList.reserve(56);
		for (auto& perm : permutations) {
			Vec3 curPos(pos.x, yPos, pos.z);
			posList.push_back(curPos.add(perm));
		}
		DrawUtils::setColor(1.f, 1.f, 1.f, 1.f);
		DrawUtils::drawLinestrip3d(posList);
	}
}

void Killaura::onPreRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if (!targetList.empty() && visualizeMode.selected == 2) {
		for (auto& i : targetList) {
			Vec3 position = i->getRenderPositionComponent()->renderPos;
			float color = i->getMovementProxy()->getHealth() / 20;
			DrawUtils::setColor(1 - color, color, 0, 0.15f);
			DrawUtils::drawBox(position.add(-0.5, -1.7, -0.5), position.add(0.5, 0.3, 0.5), 0.3f, true);
			//DrawUtils::draw2DBox(position.add(-0.5, -1.7, -0.5), position.add(0.5, 0.3, 0.5), 0.7f, false, true);
		}
	}
	if (!targetList.empty() && visualizeMode.selected == 3) {
		for (auto& i : targetList) {
			Vec3 position = i->getRenderPositionComponent()->renderPos;
			float color = i->getMovementProxy()->getHealth() / 20;
			DrawUtils::setColor(1 - color, color, 0, 0.15f);
			float size = 20 / (float)fmax(1, position.add(0.5f, 0.5f, 0.5f).dist(Game.getClientInstance()->levelRenderer->getOrigin()));
			Vec2 pos = DrawUtils::worldToScreen(position.add(0.5f, 0.5f, 0.5f));
			if (0 < pos.x && 0 < pos.y) DrawUtils::drawCircleFilled(pos, Vec2(size, size), MC_Color(1 - color, color, 0.f), 0.15);
			//DrawUtils::drawBox(position.add(-0.5, -1.7, -0.5), position.add(0.5, 0.3, 0.5), 0.3f, true);
			// DrawUtils::draw2DBox(position.add(-0.5, -1.7, -0.5), position.add(0.5, 0.3, 0.5), 0.7f, false, true);
		}
	}
}

void Killaura::onSendPacket(Packet* packet) {
	if (moduleMgr->getModule<Regen>()->isEnabled() && moduleMgr->getModule<Regen>()->isregen) return;
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (moduleMgr->getModule<Regen>()->isEnabled() && moduleMgr->getModule<Regen>()->isregen) return;
	if (packet->isInstanceOf<C_MovePlayerPacket>()) {
		if (!targetList.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			movePacket->pitch = animPitch;
			movePacket->headYaw = animYaw;
			movePacket->yaw = animYaw;
		}
	}
	if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
		if (!targetList.empty()) {
			auto* movePacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			movePacket->pitch = animPitch;
			movePacket->yawUnused = animYaw;
			movePacket->yaw = animYaw;
		}
	}
}
