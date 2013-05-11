#ifndef AURA_H
#define AURA_H

class Character;

class Aura {
public:
	Aura(int auraid,
		int auraintelligence,
		int auradexterity,
		int aurawisdom,
		int aurastamina,
		int aurastrength,
		int auratimeout,
		int auratick,
		int auradmg,
		int auraheal,
		int auraaoeinfect,
		int auraaoedmg,
		int auraaoeheal,
		int auraaoehealenemy,
		int auraaoedmgenemy,
		int auraaoerange,
		int auragivestealth,
		int auragiveinvisiblity,
		int auraseestealth,
		int auraseeinvisibility,
		int auraremoveonattack,
		int auraspeed,
		int auraflag,
		int auraicon,
		bool auraremoveondeath);
	int aura_id;
	int aura_intelligence;
	int aura_dexterity;
	int aura_wisdom;
	int aura_stamina;
	int aura_strength;
	int aura_timeout;
	int aura_tick;
	int aura_dmg;
	int aura_heal;
	int aura_aoeinfect;
	int aura_aoedmg;
	int aura_aoeheal;
	int aura_aoehealenemy;
	int aura_aoedmgenemy;
	int aura_aoerange;
	int aura_givestealth;
	int aura_giveinvisiblity;
	int aura_seestealth;
	int aura_seeinvisibility;
	int aura_removeonattack;
	int aura_speed;
	int aura_flag;
	int aura_icon;
	bool aura_removeondeath;
	void tick(Character* char1);
};
#endif
