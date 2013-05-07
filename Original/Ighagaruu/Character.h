#ifndef ITEM_H_
#define ITEM_H_
#include "ITEM.h"
#endif
#ifndef SPELL_H_
#define SPELL_H_
#include "Spell.h"
#endif
#ifndef AURA_H_
#define AURA_H_
#include "Aura.h"
#endif
#include <vector>
class Character {
public:
	struct STATS
{
	int health;
	int magika;
	int maxhealth;
	int maxmagika;
	int energy;
	int hate;
	int focus;
};

struct POSITION 
{
	float x;
	float y;
	float z;
	float orientation;
};
struct INVENTORY
{
	int xp;
	int level;
	int money;
	Spell* spells[11];
	int spellcooldown[11];
	Item* items[6];
	Item* gear[6];
};
struct INFO
{
	int model;
	int faction;
	int classs;
	int spellcooldown[11];
	Spell* casting;
	int cooldowns[11];
	bool combat;
	bool autoAttack;
	int casttime;
};
struct AURA
{
	Aura* aura;
	int timeleft;
	Character* applier;
	AURA(Aura* a, Character* chara, int timeout) {
		aura=a;
		timeleft=timeout;
		applier=chara;
	}
};
Character(int cid);
Character(int cid, std::string charname, int cfaction, int cclasss);
void Update(int timeDiff);
void tickCast(int time);
void SetOrientation(float or);
void addAura(Aura* aura, Character* app, int timeout);
void removeAura(int a, int b);
LPWSTR getName();
LPWSTR getCastText();
INFO info;
STATS stats;
POSITION pos;
INVENTORY inv;
std::vector <AURA> auras;
int id;
std::string charname;
private:
};
