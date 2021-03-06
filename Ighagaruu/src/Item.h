#pragma once
#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <stdlib.h>
class Item {
public:
Item();
	void Set(std::string element, std::string value);
	int item_id;
	int item_faction;
	int item_class;
	int item_type;
	int item_dmg;
	int item_heal;
	int item_mana;
	int item_cooldown;
	//int item_level;
	int item_rage;
	int item_focus;
	int item_energy;
	int item_giverage;
	int item_givemana;
	int item_giveenergy;
	int item_givefocus;
	int item_minrange;
	int item_maxrange;
	int item_length;
	bool item_globalcooldown;
	int item_projectile;
	int item_explosion;
	int item_useaura;
	bool item_selfcast;
	int item_selfheal;
	int item_selfdmg;
	int item_stamina;
	int item_strength;
	int item_dexterity;
	int item_wisdom;
	int item_charges;
	int item_mindmg;
	int item_maxdmg;
	bool item_useable;
	bool item_useperc;
	int item_slot;
	int item_cost;
	int item_intelect;
	int item_icon;
	std::string item_name;
	std::string item_description;
};

#endif
