class GameStats;
#ifndef CHARACTER_H_
#define CHARACTER_H_
#include "Character.h"
#endif
#ifndef AURA_H_
#define AURA_H_
#include "Aura.h"
#endif
#ifndef AREATRIGGER_H_
#define AREATRIGGER_H_
#include "AreaTrigger.h"
#endif
class GameStats {
public:
	int blueteam;
	int redteam;
	std::vector <Character*> *characters;
	Character* flagcarrier;
	AreaTrigger* flagtrigger;
	GameStats(std::vector <Character*> *characters);
	void ResetFlag();
	void ResetGame();
	void EndRound(bool winner);
	void BlueScore();
	void RedScore();
	void PickUpFlag(Character* chara);
};