#include "Aura.h"
Aura::Aura(int auraid,
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
		   bool auraremoveondeath) {
			   aura_id=auraid;
			   aura_intelligence=auraintelligence;
			   aura_dexterity=auradexterity;
			   aura_wisdom=aurawisdom;
			   aura_stamina=aurastamina;
			   aura_strength=aurastrength;
			   aura_timeout=auratimeout;
			   aura_tick=auratick;
			   aura_dmg=auradmg;
			   aura_heal=auraheal;
			   aura_aoeinfect=auraaoeinfect;
			   aura_aoedmg=auraaoedmg;
			   aura_aoeheal=auraaoeheal;
			   aura_aoehealenemy=auraaoehealenemy;
			   aura_aoedmgenemy=auraaoedmgenemy;
			   aura_aoerange=auraaoerange;
			   aura_givestealth=auragivestealth;
			   aura_giveinvisiblity=auragiveinvisiblity;
			   aura_seestealth=auraseestealth;
			   aura_seeinvisibility=auraseeinvisibility;
			   aura_removeonattack=auraremoveonattack;
			   aura_speed=auraspeed;
			   aura_flag=auraflag;
			   aura_icon=auraicon;
			   aura_removeondeath=auraremoveondeath;
}

void Aura::tick(Character* char1){
	if(aura_tick>0) {

	}
}
