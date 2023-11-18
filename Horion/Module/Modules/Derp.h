#pragma once

#include "Module.h"

class Derp : public IModule {
private:
	int counter = 0;
	bool epicStroke = false;
	bool packetMode = false;
float yaw =0;
float speed=2;
public:
	Derp();
	~Derp();

	// Inherited via IModule
	virtual void onPlayerTick(Player* player) override;
	virtual const char* getModuleName() override;
};
