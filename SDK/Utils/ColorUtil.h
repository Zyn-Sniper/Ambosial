#pragma once
#include "../../Horion/Module/ModuleManager.h"
#include "../../Horion/DrawUtils.h"
#include "Time/TimeUtil.h"

class ColorUtil {
private:
	unsigned __int64 lastMS = getCurrentMs();
	inline static unsigned __int64 getCurrentMs() {
		FILETIME f;
		GetSystemTimeAsFileTime(&f);
		(unsigned long long)f.dwHighDateTime;
		unsigned __int64 nano = ((unsigned __int64)f.dwHighDateTime << 32LL) + (unsigned __int64)f.dwLowDateTime;
		return (nano - 116444736000000000LL) / 10000;
	}

public:
	inline static MC_Color ColorUtil::Test(int red, int green, int blue, int red2, int green2, int blue2, long index) {
		float hue = ((TimerUtil::getCurrentMs()) % (int)(((int)10) * 1000)) / (float)(((int)10) * 1000);
		float r, g, b = 0;
		Utils::ColorConvertHSVtoRGB(hue, 1.F, 1, r, g, b);
		double offset = ((TimerUtil::getCurrentMs() - index) / 2) / (double)120;
		double aids123 = ((TimerUtil::getCurrentMs() - index) % 1000 / 1000.000);
		int aids1234 = ((TimerUtil::getCurrentMs() - index) % 2000 / 2000.000) * 2;
		aids123 = aids1234 % 2 == 0 ? aids123 : 1 - aids123;
		double inverse_percent = 1 - aids123;
		int redPart = (int)(r * red * inverse_percent + r * red2 * aids123);
		int greenPart = (int)(g * green * inverse_percent + g * green2 * aids123);
		int bluePart = (int)(b * blue * inverse_percent + b * blue2 * aids123);

		return MC_Color(redPart, greenPart, bluePart);
	}

	inline static MC_Color ColorUtil::waveColor(int red, int green, int blue, int red2, int green2, int blue2, long index) {
		double offset = ((TimeUtil::getCurrentMs() - index) / 8) / (double)120;
		double aids123 = ((TimeUtil::getCurrentMs() - index) % 1000 / 1000.000);
		int aids1234 = ((TimeUtil::getCurrentMs() - index) % 2000 / 2000.000) * 2;
		aids123 = aids1234 % 2 == 0 ? aids123 : 1 - aids123;
		double inverse_percent = 1 - aids123;
		int redPart = (int)(red * inverse_percent + red2 * aids123);
		int greenPart = (int)(green * inverse_percent + green2 * aids123);
		int bluePart = (int)(blue * inverse_percent + blue2 * aids123);

		return MC_Color(redPart, greenPart, bluePart);
	};

	inline static MC_Color ClientColor(int index) {
		return waveColor(moduleMgr->getModule<Theme>()->r, moduleMgr->getModule<Theme>()->g, moduleMgr->getModule<Theme>()->b, moduleMgr->getModule<Theme>()->r2, moduleMgr->getModule<Theme>()->g2, moduleMgr->getModule<Theme>()->b2, index * 3.f);
	}
};