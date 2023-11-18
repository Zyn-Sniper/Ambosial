#include "ChinaHat.h"
#include "../../../Horion/DrawUtils.h"
#include "../../../SDK/Utils/ColorUtil.h"

void ChinaHat::onLevelRender() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	static float n = 0;
	static float anim = 0;
	if (Game.canUseMoveKeys()) {
		DrawUtils::setColor(ColorUtil::ClientColor(1).r, ColorUtil::ClientColor(1).b, ColorUtil::ClientColor(1).g, 1);

		float step = 0.7f;
		Vec3 permutations[360];
		for (int i = 0; i < 360; i++) {
			permutations[i] = {step * sinf(i), 0.f, step * cosf(i)};
		}

		Vec3* start = player->getPosOld();
		Vec3* end = player->getPos();

		auto te = DrawUtils::getLerpTime();
		// vec3_t pos = start->lerp(end, te);
		Vec3 pos = *end;

		auto yPos = pos.y + 0.2f;

		std::vector<Vec3> posList;
		posList.reserve(360);  // was 56
		for (auto& perm : permutations) {
			Vec3 curPos(pos.x, yPos, pos.z);
			posList.push_back(curPos.add(perm));
		}
		std::vector<Vec3> renderLines;
		for (auto& circle : posList) {
			renderLines.push_back(player->getPos()->add(0, 0.5, 0));
			renderLines.push_back(circle);
			DrawUtils::drawLinestrip3d(renderLines);
			renderLines.clear();
		}
		DrawUtils::drawLinestrip3d(posList);
	}
}