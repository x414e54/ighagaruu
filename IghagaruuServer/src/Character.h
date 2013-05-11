#ifndef CHARACTER_H
#define CHARACTER_H

#include <stdio.h>
#include <vector>
#include <string>
#include <math.h>
#include <SDL2/SDL_net.h>

class Spell;
class Item;
class Aura;
class GameStats;

class Character {
public:

struct STATS
{
	int health;
	int maxhealth;
	int magika;
	int maxmagika;
	int energy;
	int hate;
	int focus;
	int stamina;
	int strength;
	int dexterity;
	int intelligence;
	int wisdom;
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
	int pluspoints;
	const Item* items[6];
	Spell* spells[11];
	const Item* gear[6];
};

struct AURA
{
	Aura* aura;
	int timeleft;
	int tick;
	Character* applier;
	AURA(Aura* a, int tl, int t, Character* chara) {
		aura=a;
		timeleft=tl;
		tick=t;
		applier=chara;
	}
};

struct INFO
{
	int model;
	int faction;
	int classs;
	std::string name;
	int spellcooldown[11];
	Spell* casting;
	int cooldowns[11];
	Spell* attackqueue[5];
	bool combat;
	bool autoAttack;
	int casttime;
	std::vector <AURA> auras;
};

Character(int cid, std::string cname, float cx, float cy, float cz, float co, int ch, int cm, int cfaction, int cclasss, int plusstam, int plusstr, int plusdex, int plusint, int pluswis, int pluspoints, int level, int xp, int money, TCPsocket sock, GameStats* gamestats);

void Update();
void addXP(int amount);
void addMoney(int amount);
void addLevel(int amount);
void attack();
bool withinRange();
void increaseHate(int amount);
void cast();

STATS stats;
INFO info;
POSITION pos;
INVENTORY inv;
Character* target;
TCPsocket ClientSocket;
GameStats* gamestats;

int getStrength();
int getDexterity();
int getWisdom();
int getStamina();
int getIntelligence();

void addAura(Aura* aura, Character* applier);

void removeAura(int i);

int calculateAttackDmg();

void resetMaxHealth();

void die();

void resetPos();

void addCash(int cash);

int	id;
};
#endif
