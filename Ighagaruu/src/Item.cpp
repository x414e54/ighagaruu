//--------------------------------------------------------------------------------------
// Item.cpp
// The item class, contains structs and information about each item object.
//--------------------------------------------------------------------------------------

#include "Item.h"

Item::Item() {
	item_id=-1;
	item_name="Error";
	item_dmg=0;
	item_heal=0;
	item_mana=0;
	item_cooldown=0;
	item_rage=0;
	item_focus=0;
	item_energy=0;
	item_giverage=0;
	item_givemana=0;
	item_giveenergy=0;
	item_givefocus=0;
	item_minrange=0;
	item_maxrange=0;
	item_length=0;
	item_globalcooldown=0;
	item_projectile=0;
	item_explosion=0;
	item_useaura=0;
	item_selfcast=0;
	item_selfheal=0;
	item_selfdmg=0;
	item_stamina=0;
	item_intelect=0;
	item_strength=0;
	item_type=0;
	item_maxdmg=0;
	item_mindmg=0;
	item_useable=0;
	item_class=0;
	item_faction=0;
	item_charges=0;
	item_dexterity=0;
	item_wisdom=0;
	item_useperc=false;
	item_slot=0;
	item_cost=0;
	item_description="Error";
	item_icon=0;
}

void Item::Set(std::string element, std::string value) {
	if (element=="item_id") {
		item_id=atoi(value.c_str());
	} else if (element=="item_name") {
		item_name=value;
	} else if (element=="item_dmg") {
		item_dmg=atoi(value.c_str());
	} else if (element=="item_heal") {
		item_heal=atoi(value.c_str());
	} else if (element=="item_mana") {
		item_mana=atoi(value.c_str());
	} else if (element=="item_cooldown") {
		item_cooldown=atoi(value.c_str());
	} else if (element=="item_rage") {
		item_rage=atoi(value.c_str());
	} else if (element=="item_focus") {
		item_focus=atoi(value.c_str());
	} else if (element=="item_energy") {
		item_energy=atoi(value.c_str());
	} else if (element=="item_giverage") {
		item_giverage=atoi(value.c_str());
	} else if (element=="item_givemana") {
		item_givemana=atoi(value.c_str());
	} else if (element=="item_giveenergy") {
		item_giveenergy=atoi(value.c_str());
	} else if (element=="item_givefocus") {
		item_givefocus=atoi(value.c_str());
	} else if (element=="item_minrange") {
		item_minrange=atoi(value.c_str());
	} else if (element=="item_maxrange") {
		item_maxrange=atoi(value.c_str());
	} else if (element=="item_length") {
		item_length=atoi(value.c_str());
	} else if (element=="item_globalcooldown") {
		item_globalcooldown=(atoi(value.c_str())? true : false);
	} else if (element=="item_projectile") {
		item_projectile=atoi(value.c_str());
	} else if (element=="item_explosion") {
		item_explosion=atoi(value.c_str());
	} else if (element=="item_useaura") {
		item_useaura=atoi(value.c_str());
	} else if (element=="item_selfcast") {
		item_selfcast=(atoi(value.c_str())? true : false);
	} else if (element=="item_selfheal") {
		item_selfheal=atoi(value.c_str());
	} else if (element=="item_selfdmg") {
		item_selfdmg=atoi(value.c_str());
	} else if (element=="item_stamina") {
		item_stamina=atoi(value.c_str());
	} else if (element=="item_intelect") {
		item_intelect=atoi(value.c_str());
	} else if (element=="item_strength") {
		item_strength=atoi(value.c_str());
	} else if (element=="item_type") {
		item_type=atoi(value.c_str());
	} else if (element=="item_maxdmg") {
		item_maxdmg=atoi(value.c_str());
	} else if (element=="item_mindmg") {
		item_mindmg=atoi(value.c_str());
	} else if (element=="item_useable") {
		item_useable=(atoi(value.c_str())? true : false);
	} else if (element=="item_class") {
		item_class=atoi(value.c_str());
	} else if (element=="item_faction") {
		item_faction=atoi(value.c_str());
	} else if (element=="item_charges") {
		item_charges=atoi(value.c_str());
	} else if (element=="item_dexterity") {
		item_dexterity=atoi(value.c_str());
	} else if (element=="item_wisdom") {
		item_wisdom=atoi(value.c_str());
	} else if (element=="item_useperc") {
		item_useperc=(atoi(value.c_str())? true : false);
	} else if (element=="item_slot") {
		item_slot=atoi(value.c_str());
	} else if (element=="item_cost") {
		item_cost=atoi(value.c_str());
	} else if (element=="item_description") {
		item_description=value;
	} else if (element=="item_icon") {
		item_icon=atoi(value.c_str());
	}
}
