#include "Fly.h"
#include "../../../SDK/Utils/TimerUtil.h"
#include "../../../SDK/Utils/Movement/MovementUtil.h"

float jumpY = 0;

Fly::Fly() : IModule('F', Category::MOVEMENT, "Fly to the sky") {
	mode.addEntry(EnumEntry("Creative", 0))
	.addEntry(EnumEntry("CubeGlide", 1))
	.addEntry(EnumEntry("AirStuck", 2))
	.addEntry(EnumEntry("Jetpack", 3))
	.addEntry(EnumEntry("Jetpack2", 4))
	.addEntry(EnumEntry("Motion", 5))
	.addEntry(EnumEntry("Hive", 6))
	.addEntry(EnumEntry("Geyser", 7))
	.addEntry(EnumEntry("GroundSpoof", 8))
	.addEntry(EnumEntry("HiveDamage", 9));
	addEnum("Mode", &mode);
	addFloat("Horizontal Speed", &this->horizontalSpeed, this->horizontalSpeed, 0.1f, 10.f);
	addFloat("Vertical Speed", &this->verticalSpeed, this->verticalSpeed, 0.1f, 10.f);
}

Fly::~Fly() {
}

const char *Fly::getModuleName() {
	return ("Fly");
}

void Fly::onEnable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr)
		return;
	nowpos = player->getRenderPositionComponent()->renderPos.y;

	lastFlyMs = TimerUtil::getCurrentMs();
	isDamaged = false;
	if (mode.selected == 9) {
		for (int x = -2; x <= 2; x++) {
			for (int z = -2; z <= 2; z++) {
				for (int y = -2; y <= 2; y++) {
					Vec3 blockBelow = player->location->pos.add(Vec3(x, y, z));
					if (player->getRegion()->getBlock(blockBelow.add(0, -1, 0))->toLegacy()->isSolid) continue;

					BlockSource *region = Game.getLocalPlayer()->getRegion();
					Block *block = region->getBlock(Vec3i(blockBelow));
					BlockLegacy *blockLegacy = (block->blockLegacy);
					if (!blockLegacy->isSolid) {
						Vec3i blok(blockBelow);

						// Find neighbour
						static std::vector<Vec3i *> checklist;
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
							bool Y = ((region->getBlock(calc)->blockLegacy))->isSolid;
							if (!((region->getBlock(calc)->blockLegacy))->isSolid) {
								// Found a solid block to click
								foundCandidate = true;
								blok = calc;
								break;
							}
							i++;
						}
						if (foundCandidate) {
							Vec3 BlockPos = blockBelow;
							player->level->blockSide = i;
							player->level->block = Vec3i(BlockPos);
							player->level->rayHitType = 0;
							player->level->rayHitVec = BlockPos;
							Game.getGameMode()->buildBlock(&blok, i, false);
							isDamaged = true;
							jumpY = player->getPos()->y;
							return;
						}
					}
				}
			}
		}
	}
}

void Fly::onTick(GameMode *gm) {
	LocalPlayer *player = Game.getLocalPlayer();
	if (player == nullptr)
		return;
	gameTick++;

	switch (mode.selected) {
	case 0:
		//gm->player->setAbilities()
		break;
	case 1: {
		float calcYaw = (gm->player->getMovementProxy()->getRot().x + 90) * (PI / 180);

		gameTick++;

		Vec3 pos = *Game.getLocalPlayer()->getPos();
		pos.y += 1.3f;
		C_MovePlayerPacket a(Game.getLocalPlayer(), pos);
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);
		pos.y -= 1.3f;
		C_MovePlayerPacket a2(Game.getLocalPlayer(), pos);
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&a2);

		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * horizontalSpeed;
		moveVec.z = sin(calcYaw) * horizontalSpeed;

		gm->player->lerpMotion(moveVec);

		if (gameTick >= 5) {
			gameTick = 0;
			float yaw = gm->player->getActorHeadRotationComponent()->rot.y * (PI / 180);
			float length = 4.f;

			float x = -sin(yaw) * length;
			float z = cos(yaw) * length;

			gm->player->setPos(pos.add(Vec3(x, 0.5f, z)));
		}

		break;
	}
	case 2:
		gm->player->location->velocity = Vec3(0, 0, 0);
		break;

	case 3: {
		float calcYaw = (gm->player->getActorHeadRotationComponent()->rot.y + 90) * (PI / 180);
		float calcPitch = (gm->player->getActorHeadRotationComponent()->rot.x) * -(PI / 180);

		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * horizontalSpeed;
		moveVec.y = sin(calcPitch) * horizontalSpeed;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * horizontalSpeed;

		gm->player->lerpMotion(moveVec);

		break;
	}
	case 4: {
		if (gameTick >= 5) {
			float calcYaw = (gm->player->getActorHeadRotationComponent()->rot.y + 90) * (PI / 180);
			float calcPitch = (gm->player->getActorHeadRotationComponent()->rot.x) * -(PI / 180);

			Vec3 pos = *Game.getLocalPlayer()->getPos();
			C_MovePlayerPacket a(Game.getLocalPlayer(), pos);
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);
			pos.y += 0.35f;
			a = C_MovePlayerPacket(Game.getLocalPlayer(), pos);
			Game.getClientInstance()->loopbackPacketSender->sendToServer(&a);

			gm->player->location->velocity.y = 0.465f;
			Vec3 moveVec;
			moveVec.x = cos(calcYaw) * cos(calcPitch) * horizontalSpeed;
			moveVec.z = sin(calcYaw) * cos(calcPitch) * horizontalSpeed;

			gm->player->location->velocity.x = moveVec.x;
			gm->player->location->velocity.z = moveVec.z;

			float teleportX = cos(calcYaw) * cos(calcPitch) * 0.00000005f;
			float teleportZ = sin(calcYaw) * cos(calcPitch) * 0.00000005f;

			pos = *gm->player->getPos();
			gm->player->setPos(Vec3(pos.x + teleportX, pos.y - 0.15f, pos.z + teleportZ));

			gm->player->location->velocity.y -= 0.15f;
			gameTick = 0;
		}

		gm->player->location->velocity = Vec3(0, 0, 0);

		break;
	}
	case 5:
		gm->player->location->velocity = Vec3(0, 0, 0);
		break;
	case 6:
		gm->player->location->velocity = Vec3(0.f, gm->player->location->velocity.y, 0.f);
		Game.getClientInstance()->minecraft->setTimerSpeed(1.f);
		break;
	case 7:
		float motion;

		GameSettingsInput *inputf = Game.getClientInstance()->getGameSettingsInput();
		bool jumping = GameData::isKeyDown(*inputf->spaceBarKey);
		bool sneaking = GameData::isKeyDown(*inputf->sneakKey);

		if (gameTick = 15 && !jumping && !sneaking) {
			motion = -0.04;
			gameTick = 0;
		}

		//gm->player->location->velocity = Vec3(0.f, motion, 0.f);
		break;
	}

	if (mode.selected == 9) {
		if (isDamaged) {
			if (player->getPos()->y < jumpY) {
				player->jumpFromGround();
				player->setPos(Vec3(player->getPos()->x, jumpY, player->getPos()->z));
			}
			Game.getClientInstance()->minecraft->setTimerSpeed(32.f);
			player->setSprinting(true);
			Game.getClientInstance()->getMoveTurnInput()->isSneakDown = false;
		} else {
			Game.getClientInstance()->minecraft->setTimerSpeed(5.f);
			player->location->velocity.x = 0.f;
			player->location->velocity.z = 0.f;
		}
	}
}

void Fly::onDisable() {
	if (Game.getLocalPlayer() == nullptr)
		return;

	dodesync = false;
	isDamaged = false;
	nowpos = 0;

	switch (mode.selected) {
	case 0:
		//if (Game.getLocalPlayer()->getActorGameTypeComponent()->gameType != 1)
			//Game.getLocalPlayer()->getAbilitiesComponent()->canFly = false;
			//Game.getLocalPlayer()->getAbilitiesComponent()->isFlying = false;
		break;
	case 1:
		Game.getLocalPlayer()->location->velocity = Vec3(0, 0, 0);
		break;
	case 6:
		Game.getLocalPlayer()->location->velocity = Vec3(0.f, Game.getLocalPlayer()->location->velocity.y, 0.f);
		Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
		break;
	case 7:
		Game.getLocalPlayer()->location->velocity = Vec3(0, 0, 0);
		break;
	}

	Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
}

void Fly::onMove(MoveInputHandler *input) {
	LocalPlayer *localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	if (mode.selected == 8) {  // spoof
		if (!localPlayer->getMovementProxy()->isOnGround()) {
			// MoveUtil::setSpeed(0.257);
			dodesync = true;
			MoveUtil::setSpeed(horizontalSpeed);
		}
		localPlayer->location->velocity.y = 0;
		if (input->isJumping) {
			localPlayer->location->velocity.y += verticalSpeed;
			localPlayer->getMovementProxy()->setFallDistance(0.f);
		}
		if (input->isSneakDown) {
			localPlayer->location->velocity.y -= verticalSpeed;
			localPlayer->getMovementProxy()->setFallDistance(0.f);
		}
	}

	bool keyPressed = false;
	GameSettingsInput *inputf = Game.getClientInstance()->getGameSettingsInput();
	bool jumping = GameData::isKeyDown(*inputf->spaceBarKey);
	bool sneaking = GameData::isKeyDown(*inputf->sneakKey);

	float calcYaw = (localPlayer->getActorHeadRotationComponent()->rot.y + 90) * (PI / 180);
	float c = cos(calcYaw);
	float s = sin(calcYaw);

	Vec2 moveVec2D = {input->forwardMovement, -input->sideMovement};
	bool flag = moveVec2D.magnitude() > 0.f;

	moveVec2D = {moveVec2D.x * c - moveVec2D.y * s, moveVec2D.x * s + moveVec2D.y * c};
	Vec3 moveVec;

	switch (mode.selected) {
	case 5: {
		Vec3 *localPlayerPos = localPlayer->getPos();

		float yaw = localPlayer->getActorHeadRotationComponent()->rot.y;
		Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
		bool pressed = moveVec2d.magnitude() > 0.01f;

		if (input->isJumping) {
			localPlayer->location->velocity.y += verticalSpeed;
		}

		if (input->isSneakDown) {
			localPlayer->location->velocity.y -= verticalSpeed;
		}

		if (input->right) {
			yaw += 90.f;

			if (input->forward)
				yaw -= 45.f;
			else if (input->backward)
				yaw += 45.f;
		}

		if (input->left) {
			yaw -= 90.f;

			if (input->forward)
				yaw += 45.f;
			else if (input->backward)
				yaw -= 45.f;
		}

		if (input->backward && !input->left && !input->right)
			yaw += 180.f;

		if (pressed) {
			float calcYaw = (yaw + 90.f) * (PI / 180.f);
			Vec3 moveVec;
			moveVec.x = cos(calcYaw) * horizontalSpeed;
			moveVec.y = localPlayer->location->velocity.y;
			moveVec.z = sin(calcYaw) * horizontalSpeed;
			localPlayer->lerpMotion(moveVec);
		}

		break;
	}
	case 6: {
		// Moonlight is an AntiCheat by disepi (Zephyr Developer)
		// This Fly/Glide worked on the Hive in the first half year of 2021
		// Idea from Weather Client (dead by now), TurakanFly from BadMan worked similar with less height loss

		if (!localPlayer->getMovementProxy()->isOnGround()) {
			//localPlayer->location->velocity.y = 0.f;
		}

		GameSettingsInput *input = Game.getClientInstance()->getGameSettingsInput();

		if (input == nullptr)
			return;

		float yaw = localPlayer->getActorHeadRotationComponent()->rot.y;

		if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->backKey))
			return;
		else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 45.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->forwardKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 45.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 135.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->backKey) && GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 135.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->forwardKey)) {
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->backKey)) {
			yaw += 180.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->rightKey) && !GameData::isKeyDown(*input->leftKey)) {
			yaw += 90.f;
			keyPressed = true;
		} else if (GameData::isKeyDown(*input->leftKey) && !GameData::isKeyDown(*input->rightKey)) {
			yaw -= 90.f;
			keyPressed = true;
		} else {
			localPlayer->location->velocity.x = 0.f;
			localPlayer->location->velocity.z = 0.f;
			keyPressed = false;
		}

		if (yaw >= 180)
			yaw -= 360.f;

		float calcYaw = (yaw + 90) * (PI / 180);
		Vec3 moveVec;
		moveVec.x = cos(calcYaw) * horizontalSpeed;
		moveVec.y = localPlayer->location->velocity.y;
		moveVec.z = sin(calcYaw) * horizontalSpeed;

		if (keyPressed) {
			localPlayer->lerpMotion(moveVec);
			keyPressed = false;
		}

		/*if (gameTick > 0) {
			if (!localPlayer->getMovementProxy()->isOnGround()) {
				Vec3 pos = *localPlayer->getPos();
				pos.x += moveVec.x;
				pos.z += moveVec.z;

				localPlayer->setPos(pos);
				keyPressed = false;
			}

			gameTick = 0;
		}*/
		break;
	}
	case 7:
		float motion = 0.0f;

		if (Game.canUseMoveKeys()) {
			if (jumping)
				motion += verticalSpeed;
			if (sneaking)
				motion -= verticalSpeed;
		}

		if (flag || jumping || sneaking) {
			moveVec.x = moveVec2D.x * horizontalSpeed;
			moveVec.y = motion;
			moveVec.z = moveVec2D.y * horizontalSpeed;

			localPlayer->lerpMotion(moveVec);
		}
		break;
	}
}

void Fly::onSendPacket(Packet *packet) {
	GameSettingsInput *input = Game.getClientInstance()->getGameSettingsInput();
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	if (mode.selected == 8) {
		if (packet->isInstanceOf<PlayerAuthInputPacket>()) {
			PlayerAuthInputPacket *authInput = reinterpret_cast<PlayerAuthInputPacket *>(packet);
			authInput->pos.y = nowpos;
		}
	}

	Vec3 pos;
	pos.x = player->getPos()->x;
	pos.y = player->getPos()->y;
	pos.z = player->getPos()->z;
	static int tick = 1;
	tick++;
}
