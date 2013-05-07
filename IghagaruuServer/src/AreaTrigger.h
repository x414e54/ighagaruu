class AreaTrigger;

#ifndef CHARACTER_H_
#define CHARACTER_H_
#include "Character.h"
#endif

#ifndef GAMESTATS_H_
#define GAMESTATS_H_
#include "GameStats.h"
#endif

struct VECTOR {
	float x;
	float y;
	float z;
	VECTOR() {
		x=0;
		y=0;
		z=0;
	}
	VECTOR(float x1, float y1, float z1) {
		x=x1;
		y=y1;
		z=z1;
	}
};
struct BOUNDINGBOX {
	VECTOR u;
	VECTOR l;
	BOUNDINGBOX() {
		VECTOR u1(0,0,0);
		VECTOR l1(0,0,0);
		u=u1;
		l=l1;
	}
	BOUNDINGBOX(VECTOR u1, VECTOR l1) {
		u=u1;
		l=l1;
	}
};
class AreaTrigger {
public:

	GameStats* gamestats;
	std::vector <Aura*> *auras;
	std::vector <AreaTrigger*> *triggers;
	BOUNDINGBOX box;
	AreaTrigger(GameStats* gamestats, std::vector <Aura*> *auras, std::vector <AreaTrigger*> *triggers);
	void Action(Character* chara);
	bool WithinTrigger(Character* chara);
	void Cooldown();

	bool areatrigger_enabled;
	int areatrigger_id;
	int areatrigger_faction;
	bool areatrigger_teleport;
	float areatrigger_telex;
	float areatrigger_teley;
	float areatrigger_telez;
	int areatrigger_giveaura;
	int areatrigger_needaura;
	bool areatrigger_disableontouch;
	bool areatrigger_addpoints;
	int areatrigger_cooldown;
	bool areatrigger_oncooldown;
	int areatrigger_enabletrigger;
	bool areatrigger_flag;
};