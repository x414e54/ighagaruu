#include <string>

class Spell;

#ifndef CHARACTER_H_
#define CHARACTER_H_
#include "Character.h"
#endif
#ifndef AURA_H_
#define AURA_H_
#include "Aura.h"
#endif
class Spell {
public:
	
struct FLOATARR {
	char b[8];
	float f;
	char* ptr;
	FLOATARR(float inf){
		f=inf;
		ptr=(char*)&f;
		b[0]=(char)*ptr;
		b[1]=(char)*(ptr+1);
		b[2]=(char)*(ptr+2);
		b[3]=(char)*(ptr+3);
		b[4]=(char)*(ptr+4);
		b[5]=(char)*(ptr+5);
		b[6]=(char)*(ptr+6);
		b[8]=(char)*(ptr+7);
	}
};
struct INTARR {
	char b[4];
	int i;
	char* ptr;
	INTARR(int in){
		i=in;
		ptr=(char*)&i;
		b[0]=(char)*ptr;
		b[1]=(char)*(ptr+1);
		b[2]=(char)*(ptr+2);
		b[3]=(char)*(ptr+3);
	}
};
struct ARRFLOAT {
	char b[8];
	float f;
	float* ptr;
	ARRFLOAT(char b1, char b2, char b3, char b4, char b5, char b6, char b7, char b8 ){
		b[0]=b1;
		b[1]=b2;
		b[2]=b3;
		b[3]=b4;
		b[4]=b5;
		b[5]=b6;
		b[6]=b7;
		b[7]=b8;
		ptr=(float*)&b;
		f=*ptr;
	}
};
struct ARRINT {
	char b[4];
	int i;
	int* ptr;
	ARRINT(char b1, char b2, char b3, char b4){
		b[0]=b1;
		b[1]=b2;
		b[2]=b3;
		b[3]=b4;
		ptr=(int*)&b;
		i=*ptr;
	}
};
	Spell(int spellid,
		int spellfaction,
		int spellclass,
		int spelltype,
		int spelldmg,
		int spellheal,
		int spellmana,
		int spellcooldown,
		int spelllevel,
		int spellrage,
		int spellfocus,
		int spellenergy,
		int spellgiverage,
		int spellgivemana,
		int spellgiveenergy,
		int spellgivefocus,
		int spellminrange,
		int spellmaxrange,
		int spelllength,
		bool spellglobalcooldown,
		int spellprojectile,
		int spellexplosion,
		int spelluseaura,
		bool spellselfcast,
		int spellselfheal,
		int spellselfdmg,
		bool spelldrainlife,
		bool spelldrainmana,
		bool spelldrainrage,
		bool spelldrainfocus,
		bool spelldrainenergy,
		bool spelluseperc,
		int spellcost,
		bool spellattackqueue,
		std::vector <Aura*> *auras
		);
	void Cast(Character* char1, Character* char2, bool queued, bool casted);
	int withinRange(Character* char1, Character* char2);
	int spell_id;
	int spell_faction;
	int spell_class;
	int spell_type;
	int spell_dmg;
	int spell_heal;
	int spell_mana;
	int spell_cooldown;
	int spell_level;
	int spell_rage;
	int spell_focus;
	int spell_energy;
	int spell_giverage;
	int spell_givemana;
	int spell_giveenergy;
	int spell_givefocus;
	int spell_minrange;
	int spell_maxrange;
	int spell_length;
	bool spell_globalcooldown;
	int spell_projectile;
	int spell_explosion;
	int spell_useaura;
	bool spell_selfcast;
	int spell_selfheal;
	int spell_selfdmg;
	bool spell_drainlife;
	bool spell_drainmana;
	bool spell_drainrage;
	bool spell_drainfocus;
	bool spell_drainenergy;
	bool spell_useperc;
	int spell_cost;
	bool spell_attackqueue;
	std::vector <Aura*> *auras;
};
