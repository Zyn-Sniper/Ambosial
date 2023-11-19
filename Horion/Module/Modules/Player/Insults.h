#pragma once
#include "../../ModuleManager.h"
#include "../Module.h"

class Insults : public IModule {
private:
	bool sound = false;
	bool notification = false;
	bool playernameign = false;

public:
	Insults() : IModule(0x0, Category::PLAYER, "Changes the games Internal timer") {
		addEnum("Mode", &mode);
		mode.addEntry(EnumEntry("Ambosial", 0))
			.addEntry(EnumEntry("UwU", 1))
			.addEntry(EnumEntry("Toxic", 2));
		addBool("Sound", &sound, sound);
		addBool("PlayerName", &playernameign, playernameign);
		addBool("Notifications", &notification, notification);
	};
	std::string ign = "a";
	bool killed = false;

	SettingEnum mode = this;

	std::string getModSettings() {
		return mode.GetSelectedEntry().GetName();
	}

	virtual void onPlayerTick(Player* plr);
	const char* getModuleName() {
		return "Insults";
	}
	void onEnable() {
		killed = false;
	}
	//Insults();
};