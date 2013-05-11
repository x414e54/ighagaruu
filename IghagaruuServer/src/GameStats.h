#ifndef GAMESTATS_H
#define GAMESTATS_H

#include <vector>

class Character;
class AreaTrigger;

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
#endif
