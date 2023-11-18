#include "../../ModuleManager.h"
#include "../../../Horion/DrawUtils.h"
#include "../../../SDK/Utils/Movement/MovementUtil.h"
#include "Trail.h"

const char* Trails::getModuleName() {
	return ("Trail");
}

void Trails::onTick(GameMode* gm) {
	trailDelay = 2;
	if (Game.getLocalPlayer() != nullptr) {
		countDelay--;
		if (countDelay <= 0) {
			CircleStatus circle;
			circle.pos = Game.getLocalPlayer()->getRenderPositionComponent()->renderPos;
			circle.pos.y -= Game.getLocalPlayer()->getAABBShapeComponent()->aabb.height;
			circle.duration = trailDuration;
			Circles.push_back(circle);
			countDelay = trailDelay;
		}

		if (!Circles.empty()) {
			for (auto& currentCircle : Circles) {
				if (MoveUtil::isMoving())
					currentCircle.duration--;
				if (currentCircle.duration <= 0) Circles.erase(Circles.begin());
			}
		}
	}
}

void Trails::onPostRender(MinecraftUIRenderContext* renderCtx) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) {
		Circles.clear();
		return;
	}
	if (!Game.canUseMoveKeys()) return;

	if (!Circles.empty()) {
		for (auto& currentCircle : Circles) {
			Vec2 renderPos = DrawUtils::worldToScreen(currentCircle.pos);
			float size = 20 / (float)fmax(1, currentCircle.pos.dist(Game.getClientInstance()->levelRenderer->getOrigin()));
			if (renderPos.x > 0 && renderPos.y > 0) {
				float alpha = currentCircle.duration * 0.003f;
				// clientMessageF("Alpha = %i", currentCircle.duration);
				DrawUtils::drawCircleFilled(renderPos, Vec2(size, size), MC_Color(1.f, 1.f, 1.f, alpha), 1);
			}
		}
	}
}