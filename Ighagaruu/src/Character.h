#pragma once
#ifndef CHARACTER_H
#define CHARACTER_H

#include "Item.h"
#include "Spell.h"
#include "Aura.h"
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
Character(int cid, std::wstring charname, int cfaction, int cclasss);
void Update(int timeDiff);
void tickCast(int time);
void SetOrientation(float orientation);
void addAura(Aura* aura, Character* app, int timeout);
void removeAura(int a, int b);
std::wstring getName();
std::wstring getCastText();
INFO info;
STATS stats;
POSITION pos;
INVENTORY inv;
std::vector <AURA> auras;
int id;
std::wstring charname;
private:
};

#endif
