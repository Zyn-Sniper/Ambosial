#include "Bhop.h"

#include "../../../SDK/Utils/HMath.h"
#include "..\ModuleManager.h"

Bhop::Bhop() : IModule(0, Category::MOVEMENT, "Hop around like a bunny!") {
	mode.addEntry(EnumEntry("Vanilla", 0))
		.addEntry(EnumEntry("Friction", 1))
		.addEntry(EnumEntry("Hive", 2));
	addEnum("Mode", &mode);
	addFloat("Speed", &speed, speed, 0.1f, 0.8f);
	addFloat("Height", &height, height, 0.01f, 0.80f);
	addFloat("Fricition", &friction, friction, 0.70f, 0.95f);
	addFloat("Drag", &drag, drag, 1.f, 40.f);
	addBool("NoStep", &hive, hive);
	//addBool("Strafe", &strafe, strafe);
	addBool("Desync", &desync, desync);
	//addBool("Reset", &resetDesyncSetting, resetDesyncSetting);
	addInt("DesyncMS", &desyncMs, desyncMs, 520, 1000);
	addFloat("DesyncTimer", &desyncTimer, desyncTimer, 20.f, 50.f);
	addBool("FullStop", &fullstop, fullstop);
}

Bhop::~Bhop() {
}

const char* Bhop::getModuleName() {
	return ("BunnyHop");
}

void Bhop::onEnable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	//PlayerAuthInputPacketHolder.clear();
	cashedStepHeignt = player->getMaxAutoStepComponent()->stepHeight;
	smoothMoveVec = Vec2(0.f, 0.f);
	OldPos = *player->getPos();
	desyncLastMS = TimerUtil::getCurrentMs();
}

bool velocity = false;
void Bhop::onMove(MoveInputHandler* input) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	/*if (resetDesyncSetting) {
		desyncTimer = 30.f;
		desyncMs = 550.f;
		resetDesyncSetting = false;
	}*/

	/*if (player->isSneaking())
		return;*/
	player->setSprinting(true);

	Vec2 moveVec2d = {input->forwardMovement, -input->sideMovement};
	bool pressed = moveVec2d.magnitude() > 0.01f;
	float yaw = 0.f;
	yaw = player->getMovementProxy()->getRot().y;
	float calcYaw = (yaw + 90) * (PI / 180);
	Vec3 moveVec;
	float c = cos(calcYaw);
	float s = sin(calcYaw);
	moveVec2d = {moveVec2d.x * c - moveVec2d.y * s, moveVec2d.x * s + moveVec2d.y * c};
	smoothMoveVec.x = easingEaseOutCubic(1, smoothMoveVec.x, moveVec2d.x - smoothMoveVec.x, drag);
	smoothMoveVec.y = easingEaseOutCubic(1, smoothMoveVec.y, moveVec2d.y - smoothMoveVec.y, drag);

	//DesyncBoost
	if (desync) {
		if (input->isJumping && !moduleMgr->getModule<Scaffold>()->isTower) {
			//Game.getClientInstance()->getMoveTurnInput()->isJumping = false;
			doDesync = true;
			Game.getClientInstance()->minecraft->setTimerSpeed(desyncTimer);
		} else {
			doDesync = false;
			Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
		}
	} else {
		doDesync = false;
		//Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
	}

	if (hive) player->getMaxAutoStepComponent()->stepHeight = 0.f;
	if (mode.selected == 1 || mode.selected == 2) {
		static bool useVelocity = false;
		if (0.4000000059604645 >= 0.385) {
			if (player->getMovementProxy()->isOnGround() && pressed) {
				//player->jumpFromGround();
				//player->setPos()
			}
			useVelocity = false;
		} else
			useVelocity = true;

		speedFriction *= friction;
		if (player->location->velocity.magnitudexz() < 0.05) {
			speedFriction *= 0.5;
		}
		if (pressed) {
			if (player->getMovementProxy()->isOnGround()) {
				if (useVelocity && !input->isJumping) player->location->velocity.y = 0.4000000059604645;
				//speedFriction = RandomFloat(0.4190652072429657, 0.44381298780441284);
				smoothMoveVec = moveVec2d;
				//player->velocity = Vec3(0.f, player->velocity.y, 0.f);
				player->jumpFromGround();
				speedFriction = speed;
			} else {
				moveVec.x = smoothMoveVec.x * speedFriction;
				moveVec.y = player->location->velocity.y;
				moveVec.z = smoothMoveVec.y * speedFriction;
				player->lerpMotion(moveVec);
			}
			if (mode.selected == 2) {
				if (player->location->velocity.y > 0)
					velocity = true;
				else {
					if (velocity) {
						player->location->velocity.y = -0.2f;
						velocity = false;
					}
				}
			}
		}
	} else if (mode.selected == 0) {
		if (player->getMovementProxy()->isOnGround() && pressed) {
			player->jumpFromGround();
			player->location->velocity.y = height;
		}
		moveVec.x = moveVec2d.x * speed;
		moveVec.y = player->location->velocity.y;
		moveVec.z = moveVec2d.y * speed;
		if (pressed) player->lerpMotion(moveVec);
	}
}
