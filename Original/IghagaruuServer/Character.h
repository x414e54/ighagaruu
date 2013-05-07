 #ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <math.h>
#include <mysql.h>  // main SQLAPI++ header
class Character; 

#ifndef SPELL_H_
#define SPELL_H_
#include "Spell.h"
#endif
#ifndef ITEM_H_
#define ITEM_H_
#include "Item.h"
#endif
#ifndef AURA_H_
#define AURA_H_
#include "Aura.h"
#endif
#ifndef AREATRIGGER_H_
#define AREATRIGGER_H_
#include "Areatrigger.h"
#endif
#ifndef GAMESTATS_H_
#define GAMESTATS_H_
#include "GameStats.h"
#endif
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

Character(int cid, std::string cname, float cx, float cy, float cz, float co, int ch, int cm, int cfaction, int cclasss, int plusstam, int plusstr, int plusdex, int plusint, int pluswis, int pluspoints, int level, int xp, int money, SOCKET sock, GameStats* gamestats);

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
SOCKET ClientSocket;
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
