#pragma once
#ifndef SPELL_H
#define SPELL_H

#include <string>
#include <stdlib.h>
class Spell {
public:
	Spell();
	void Set(std::string element, std::string value);
	std::wstring getName();
	std::wstring getRange();
	std::wstring getUsages();
	std::wstring getCastTime();
	std::wstring getCooldown();
	std::wstring getDescription(int offset);
	int spell_id;
	std::wstring spell_name;
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
	std::wstring spell_description;
	int spell_icon;
};

#endif
