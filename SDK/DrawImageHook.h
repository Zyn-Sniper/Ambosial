#pragma once
#include "Game/MinecraftUIRenderContext/MinecraftUIRenderContext.h"
#include "../Horion/DrawUtils.h"
#include "../SDK/Utils/ColorUtil.h"
#include "ClientInstance.h"

struct TextureData {
	size_t numTextures;
	__int64** texturePtr;
} data;

void DrawImageDetour(
	MinecraftUIRenderContext* _this,
	TexturePtr* texturePtr,
	Vec2& imagePos,
	Vec2& imageDimension,
	Vec2& UvPos,
	Vec2& UvSize) {

	//if (strcmp(texturePtr->filePath.getText(), xorstr_("textures/ui/selected_hotbar_slot")) == 0) {
		//static float lerpedPos = imagePos.x;
		//lerpedPos = Math::animate(imagePos.x, lerpedPos, RenderUtil::getDeltaTime() * Game::Core::HotbarSpeed);
		//imagePos.x = lerpedPos;
		//RenderUtil::fillRectangle(Vector4<float>(imagePos.x + 1, imagePos.y + 1, imagePos.x + imageDimension.x - 1, imagePos.y + imageDimension.y - 1), UIColor(0, 0, 0), 0.5f);
		//RenderUtil::fillRectangle(Vector4<float>(imagePos.x + 1, imagePos.y + 1, imagePos.x + imageDimension.x - 1, imagePos.y + 1.5f), ColorUtil::getClientColor(5, 1, 1, 1), 1.f);
		//return;
	//}

	if (strcmp(texturePtr->filePath.getText(), ("textures/ui/title")) == 0) {
		static float pos = 0;

		std::string str("Polar");

		float strSize = 6;
		float width = DrawUtils::getTextWidth(&str, strSize);
		auto wid = Game.getClientInstance()->getGuiData()->windowSize;

		Vec2 titlePos = Vec2(wid.x / 2.f - DrawUtils::getTextWidth(&str, strSize) / 2.f, wid.y / 9.5f);
		//titleSize = Vector2<float>(width, 48);


		DrawUtils::drawText(Vec2(titlePos.x - 1, titlePos.y + 1), &str, MC_Color(0, 0, 0), strSize);
		DrawUtils::drawText(titlePos.add(-1, 0), &str, ColorUtil::getRainbowColor(2, 1, 1, 1), strSize);
		DrawUtils::flush();

		//RenderUtil::drawString(titlePos, &str, UIColor(0, 0, 0), 5.99, anim, false);
		//RenderUtil::drawGradientString(titlePos, &str, strSize, anim, false, 3, 1, 1, 55);
		//RenderUtil::flushString();

		return;
	}

	/* This is for when we want to make our own hotbar */

	//const char* prefix = "textures/ui/hotbar_";
	//int prefixLen = strlen(prefix);
	//if (strncmp(texturePtr->filePath.getText(), prefix, prefixLen) == 0) {
		// ImRenderUtil::fillRectangleCustom(Vector4<float>(imagePos.x, imagePos.y, imagePos.x + imageDimension.x, imagePos.y + imageDimension.y), UIColor(0, 0, 0), 0.5f, Vector4<float>(20, 20, 20, 20));
		// RenderUtil::renderImage(Util::getClientPath() + xorstr_("Assets\\roundhotbar.png"), Vector4<float>(imagePos.x, imagePos.y, imagePos.x + imageDimension.x, imagePos.y + imageDimension.y));
		//RenderUtil::fillRectangle(Vector4<float>(imagePos.x, imagePos.y, imagePos.x + imageDimension.x, imagePos.y + imageDimension.y), UIColor(0, 0, 0), 0.5f);
		//return;
	//}

	//if (strcmp(texturePtr->filePath.getText(), xorstr_("textures/ui/hotbar_end_cap")) == 0) {
		//return;
	//}

	//if (strcmp(texturePtr->filePath.getText(), xorstr_("textures/ui/hotbar_start_cap")) == 0) {
		//return;
	//};

	Utils::CallVFunc<7, __int64, const TexturePtr*, Vec2 const&, Vec2 const&, Vec2 const&, Vec2 const&>(_this, texturePtr, imagePos, imageDimension, UvPos, UvSize);

	//Utils::CallVFunc<void, MinecraftUIRenderContext*, TextureData*, Vector2<float>&, Vector2<float>&, Vector2<float>&, Vector2<float>&>(
		//__o__DrawImage,
		//_this,
		//texturePtr,
		//imagePos,
		//imageDimension,
		//UvPos,
		//UvSize);
}