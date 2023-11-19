#include "Insults.h"

using namespace std;

string Zephyr[11] = {
	"I have a good Zephyr config, don't blame me.",
	"Packet Patses < Zephyr!",
	"Imagine using a Packet Paste!",
	"Imagine being stuck on 1.18.12!",
	"Packet Paste Detected!",
	"Hey, did you know? Zephyr has the best Hive regen for MC:BE!",
	"Why use Clumsy when you can make a PingSpoofer!",
	"Hello, Packet Paster! Oh, goodbye...",
	"Zoooooom! Do you see the power of Zephyr?",
	"the fact people think that zephyr is discontinued is funny!",
	"Did you really expect to win that, Zephyr never loses!"};

string UwUSpeak[13] = {
	"Thanks for the fwee woot~",
	"Heyyy OwU!",
	"hehe~~",
	"Thanks for letting me touch you! Hehe!",
	"You're so gentle UwU!",
	"OwO!",
	"Hey! Thanks for letting me kill you~",
	"aahhhhh~",
	"You're so cute!",
	"mmmmmmmm~",
	"You're such a sussy baka",
	"OwO! You're so easy!",
	"I got stuck in the washing machine~"};

string Toxic[10] = {
	"GG EZ STAY MAD",
	"TRY HARDER",
	"ARE YOUR HANDS EVEN ON THE KEYBOARD?",
	"BOZO",
	"SNIPED, L",
	"GO PLAY ON YOUR WII BUDDY",
	"THE UNINSTALL BUTTON IS RESERVED JUST FOR YOU",
	"TOUCH GRASS",
	"HONESTLY EASIEST KILL OF MY ENTIRE LIFE",
	"GONNA BLAME IT ON LAG BUDDY?"};

void Insults::onPlayerTick(Player* plr) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;

	int random = 0;
	srand(time(NULL));
	if (killed) {
		ign = Utils::sanitize(ign);
		if (notification) {
			Game.addInfoBox("", "You killed " + ign)->closeTimer = 3.f;
		}
		C_TextPacket textPacket;
		Level* level = player->level;
		Vec3* pos = player->getPos();
		if (sound) {
			level->playSound("random.orb", *pos, 1, 1);
			level->playSound("firework.blast", *pos, 1, 1);
		}
		switch (mode.selected) {
		case 0:  // Zephyr
			random = rand() % 11;
			if (playernameign) {
				textPacket.message = TextHolder(Zephyr[random] + " @" + ign);
			} else {
				textPacket.message = TextHolder(Zephyr[random]);
			}
			break;
		case 1:  //
			random = rand() % 20;
			if (playernameign) {
				textPacket.message = TextHolder(UwUSpeak[random] + " @" + ign);
			} else {
				textPacket.message = TextHolder(UwUSpeak[random]);
			}
			break;
		case 2:  // Toxic
			random = rand() % 17;
			if (playernameign) {
				textPacket.message = TextHolder(" @" + ign + Toxic[random]);
			} else {
				textPacket.message = TextHolder(Toxic[random]);
			}
			break;
		}
		textPacket.sourceName.setText(player->getNameTag()->getText());
		textPacket.xboxUserId = to_string(player->getUserId());
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&textPacket);
		killed = false;
	}
}
