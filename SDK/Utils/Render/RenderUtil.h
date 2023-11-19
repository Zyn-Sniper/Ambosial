#pragma once
//#include "../../include/ImGui/imgui.h"
//#include "../ColorUtil.h"
//#include "../../Horion/DrawUtils.h"
#include "../../../Horion/Module/ModuleManager.h"
#include "../ColorUtil.h"
// #include "ImMath.h"

enum Type {
	INTERNAL = 0,
	EXTERNAL = 1,
};

enum FontType {
	NOTOSANS = 0,
	MOJANG = 1,
};

enum class VertextFormat {
	QUAD = 1,
	TRIANGLE_LIST,
	TRIANGLE_STRIP,
	LINE_LIST,
	LINE_STRIP
};

//MinecraftUIRenderContext* renderContext;

class RenderUtil {
public:
	static void RenderImage(std::string filePath, Vec4 rectPosition, Vec2& idk = Vec2(0.f, 0.f), Vec2& idk2 = Vec2(0.f, 0.f), MC_Color flushColor = MC_Color(255, 255, 255), float alpha = 1) {
		TexturePtr* texturePtr = nullptr;
		texturePtr = new TexturePtr();
		FilePath file(filePath);
		renderCtx->getTexture(texturePtr, file, false);
		static HashedString flushString = HashedString(0xA99285D21E94FC80, "ui_flush");
		//	static uintptr_t flushImageAddr = FindSignature("48 8B C4 48 89 58 ? 48 89 70 ? 55 57 41 54 41 56 41 57 48 8D A8 ? ? ? ? 48 81 EC ? ? ? ? 0F 29 70 ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 85 ? ? ? ? 4D 8B E1");

		if (texturePtr != nullptr) {
			renderCtx->drawImage(texturePtr, Vec2(rectPosition.x, rectPosition.y), Vec2(rectPosition.z, rectPosition.w), idk, idk2);
			renderCtx->flushImages(flushColor, alpha, flushString);
			delete texturePtr;
		}
	}

	static Font* GetFont(FontType fonttype) {
		if (fonttype == 0) {
			return Game.getClientInstance()->minecraftGame->getOldFont(); //Notosans
		} else if (fonttype == 1) {
			return Game.getClientInstance()->minecraftGame->mcFont; // Mojang
		}
	}

	static float getTextLen(std::string* textStr, float textSize, FontType fonttype) {
		TextHolder text(*textStr);

		Font* fontPtr = GetFont(fonttype);

		float ret = renderCtx->getLineLength(fontPtr, &text, textSize, false);

		return ret;
	}

	static void drawText(const Vec2& pos, std::string* textStr, const MC_Color& color, float textSize, float alpha, bool drawShadow, FontType font) {
		TextHolder text(*textStr);
		Font* fontPtr = GetFont(font);
		static uintptr_t caretMeasureData = 0xFFFFFFFF;

		float posF[4]; posF[0] = pos.x; posF[1] = pos.x + 1000; posF[2] = pos.y - 1; posF[3] = pos.y + 1000;

		renderCtx->drawText(fontPtr, posF, &text, color.arr, alpha, 0, &TextMeasureData(&textSize, drawShadow), &caretMeasureData);
	}

	static void drawGradient(Vec2 pos, std::string* textStr, float textSize, bool drawShadow, FontType font) {
		TextHolder text(*textStr);
		std::string test = text.getText();

		int ind = 0;
		for (char c : test) {
			ind++;
			int curIndex = -ind * 25;
			auto SmoothColor = ColorUtil::ClientColor(curIndex);
			int charWidth = getTextLen(&std::string(&c), textSize, font);

			drawText(pos, &std::string(&c), SmoothColor, textSize, 1, drawShadow, font);
			pos.x += charWidth;
		}
	}

	static void RenderRectangle(Vec4 pos, MC_Color col) {
		DrawUtils::setColor(col.r, col.g, col.b, col.a);
		DrawUtils::drawQuad({pos.x, pos.w}, {pos.z, pos.w}, {pos.z, pos.y}, {pos.x, pos.y});
	}

	static void RenderRoundRectangle(Vec4 pos, const MC_Color col) {
		RenderRectangle(Vec4(pos.x, pos.y, pos.z, pos.w), col);
		RenderRectangle(Vec4(pos.x + .5, pos.w, pos.z - .5, pos.w + 1), col);
		RenderRectangle(Vec4(pos.x + 1, pos.w + 1, pos.z - 1, pos.w + 1.5), col);
		RenderRectangle(Vec4(pos.x + 2, pos.w + 1.5, pos.z - 2, pos.w + 2), col);
		RenderRectangle(Vec4(pos.x + .5, pos.y - 1, pos.z - .5, pos.y), col);
		RenderRectangle(Vec4(pos.x + 1, pos.y - 1.5f, pos.z - 1, pos.y - 1), col);
		RenderRectangle(Vec4(pos.x + 2, pos.y - 2, pos.z - 2, pos.y - 1.5), col);
	}

	inline static void Glow(const Vec4& Position) {
		Vec4 gloPos = Vec4(Position.x + 4, Position.y + 2, Position.z - 4, Position.w - 2);
		DrawUtils::drawGlow(gloPos, MC_Color(0, 0, 0, 50 / 6), 0.05, 6, 4);
		// DrawUtils::drawQuad({layerbozo.x, layerbozo.w}, {layerbozo.z, layerbozo.w}, {layerbozo.z, layerbozo.y}, {layerbozo.x, layerbozo.y});  // yesyes
	}

	inline static void GlowColored(const Vec4& Position, MC_Color color) {
		Vec4 gloPos = Vec4(Position.x + 4, Position.y + 2, Position.z - 4, Position.w - 2);
		DrawUtils::drawGlow(gloPos, MC_Color(color.r, color.g, color.b, 50.f / 6.f), 0.05, 6, 4);
		// DrawUtils::drawQuad({layerbozo.x, layerbozo.w}, {layerbozo.z, layerbozo.w}, {layerbozo.z, layerbozo.y}, {layerbozo.x, layerbozo.y});  // yesyes
	}

	inline static float getDeltaTime() {
		return 0.016f;
	};
};
extern RenderUtil* RenderUtils;