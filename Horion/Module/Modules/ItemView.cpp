#include "ItemView.h"

void* FluxSwing = (void*)FindSignature("0f 84 ? ? ? ? 48 8b 46 ? 48 85 c0 74 ? 48 8b 08 48 85 c9 74 ? 44 0f b6 69");

ItemView::ItemView() : IModule(0, Category::VISUAL, "Animate swords like java") {
	mode.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Bedrock", 1))
		.addEntry(EnumEntry("Java", 2));
	addEnum("Mode", &mode);
	addInt("SwingDuration", &swingDuration, swingDuration, 0, 40);
}

ItemView::~ItemView() {
}

const char* ItemView::getModuleName() {
	return "ItemView";
}

void ItemView::onTick(GameMode* gm) {
	auto killaura = moduleMgr->getModule<Killaura>();
	auto player = Game.getLocalPlayer();
	if (player == nullptr || !Game.canUseMoveKeys()) {
		shouldBlock = false;
		Utils::patchBytes((unsigned char*)((uintptr_t)FluxSwing), (unsigned char*)"\x0F\x84\xBD\x04\x00\x00", 6);
		return;
	}
	ItemStack* stack = player->getSupplies()->inventory->getItemStack(player->getSupplies()->selectedHotbarSlot);
	shouldBlock = stack != nullptr && stack->item != nullptr && stack->getItem()->isWeapon() && Game.isRightClickDown() || stack != nullptr && stack->item != nullptr && stack->getItem()->isWeapon() && !killaura->targetListEmpty && killaura->isEnabled() || (moduleMgr->getModule<Scaffold>()->spoof && moduleMgr->getModule<Scaffold>()->isEnabled());

	if (shouldBlock) {
		Utils::nopBytes((unsigned char*)FluxSwing, 6);
	} else {
		Utils::patchBytes((unsigned char*)((uintptr_t)FluxSwing), (unsigned char*)"\x0F\x84\xBD\x04\x00\x00", 6);
	}
}

void ItemView::onDisable() {
	auto player = Game.getLocalPlayer();
	Utils::patchBytes((unsigned char*)((uintptr_t)FluxSwing), (unsigned char*)"\x0F\x84\xBD\x04\x00\x00", 6);
}
