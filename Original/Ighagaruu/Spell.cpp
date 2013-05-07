//--------------------------------------------------------------------------------------
// Spell.cpp
// The spell class, contains structs and information about each spell object.
//--------------------------------------------------------------------------------------

#include "dxut.h"
#ifndef SPELL_H_
#define SPELL_H_
#include "Spell.h"
#endif
#include <sstream>
Spell::Spell(){
	spell_id=-1;
	spell_name="Error";
	spell_faction=0;
	spell_class=0;
	spell_type=0;
	spell_dmg=0;
	spell_heal=0;
	spell_mana=0;
	spell_cooldown=0;
	spell_level=0;
	spell_rage=0;
	spell_focus=0;
	spell_energy=0;
	spell_giverage=0;
	spell_givemana=0;
	spell_giveenergy=0;
	spell_givefocus=0;
	spell_minrange=0;
	spell_maxrange=0;
	spell_length=0;
	spell_globalcooldown=0;
	spell_projectile=0;
	spell_explosion=0;
	spell_useaura=0;
	spell_selfcast=0;
	spell_selfheal=0;
	spell_selfdmg=0;
	spell_drainlife=false;
	spell_drainmana=false;
	spell_drainrage=false;
	spell_drainfocus=false;
	spell_drainenergy=false;
	spell_useperc=false;
	spell_cost=0;
	spell_attackqueue=0;
	spell_description="Error";
	spell_icon=0;
}
void Spell::Set(std::string element, std::string value) {
	if (element=="spell_id") {
		spell_id=atoi(value.c_str());
	} else if (element=="spell_name") {
		spell_name=value;
	} else if (element=="spell_faction") {
		spell_faction=atoi(value.c_str());
	} else if (element=="spell_class") {
		spell_class=atoi(value.c_str());
	} else if (element=="spell_type") {
		spell_type=atoi(value.c_str());
	} else if (element=="spell_dmg") {
		spell_dmg=atoi(value.c_str());
	} else if (element=="spell_heal") {
		spell_heal=atoi(value.c_str());
	} else if (element=="spell_mana") {
		spell_mana=atoi(value.c_str());
	} else if (element=="spell_cooldown") {
		spell_cooldown=atoi(value.c_str());
	} else if (element=="spell_level") {
		spell_level=atoi(value.c_str());
	} else if (element=="spell_rage") {
		spell_rage=atoi(value.c_str());
	} else if (element=="spell_focus") {
		spell_focus=atoi(value.c_str());
	} else if (element=="spell_energy") {
		spell_energy=atoi(value.c_str());
	} else if (element=="spell_giverage") {
		spell_giverage=atoi(value.c_str());
	} else if (element=="spell_givemana") {
		spell_givemana=atoi(value.c_str());
	} else if (element=="spell_giveenergy") {
		spell_giveenergy=atoi(value.c_str());
	} else if (element=="spell_givefocus") {
		spell_givefocus=atoi(value.c_str());
	} else if (element=="spell_minrange") {
		spell_minrange=atoi(value.c_str());
	} else if (element=="spell_maxrange") {
		spell_maxrange=atoi(value.c_str());
	} else if (element=="spell_length") {
		spell_length=atoi(value.c_str());
	} else if (element=="spell_globalcooldown") {
		spell_globalcooldown=(atoi(value.c_str())? true : false);
	} else if (element=="spell_projectile") {
		spell_projectile=atoi(value.c_str());
	} else if (element=="spell_explosion") {
		spell_explosion=atoi(value.c_str());
	} else if (element=="spell_useaura") {
		spell_useaura=atoi(value.c_str());
	} else if (element=="spell_selfcast") {
		spell_selfcast=(atoi(value.c_str())? true : false);
	} else if (element=="spell_selfheal") {
		spell_selfheal=atoi(value.c_str());
	} else if (element=="spell_selfdmg") {
		spell_selfdmg=atoi(value.c_str());
	} else if (element=="spell_drainlife") {
		spell_drainlife=(atoi(value.c_str())? true : false);
	} else if (element=="spell_drainmana") {
		spell_drainmana=(atoi(value.c_str())? true : false);
	} else if (element=="spell_drainrage") {
		spell_drainrage=(atoi(value.c_str())? true : false);
	} else if (element=="spell_drainfocus") {
		spell_drainfocus=(atoi(value.c_str())? true : false);
	} else if (element=="spell_drainenergy") {
		spell_drainenergy=(atoi(value.c_str())? true : false);
	} else if (element=="spell_useperc") {
		spell_useperc=(atoi(value.c_str())? true : false);
	} else if (element=="spell_cost") {
		spell_cost=atoi(value.c_str());
	} else if (element=="spell_attackqueue") {
		spell_attackqueue=(atoi(value.c_str())? true : false);
	} else if (element=="spell_description") {
		spell_description=value;
	} else if (element=="spell_icon") {
		spell_icon=atoi(value.c_str());
	}
}

LPWSTR Spell::getName(){
	LPWSTR widesname=L"Error";
	if (spell_name!="") {
	LPCSTR shortsname=spell_name.c_str();
	widesname=new WCHAR[strlen(shortsname)+1];
	MultiByteToWideChar(0,0,shortsname,-1,widesname,(int)strlen(shortsname)+1);
	}
	return widesname;
}

LPWSTR Spell::getDescription(int offset){
	LPWSTR widesname=L"Error";
	if (spell_description!="") {
	std::string offsetdesc="";
	int j=0;
	for (unsigned int i=(offset*50); i<spell_description.length() && j<50; i++) {
		offsetdesc+=spell_description.c_str()[i];
		j++;
	}
	LPCSTR shortsname=offsetdesc.c_str();
	widesname=new WCHAR[strlen(shortsname)+1];
	MultiByteToWideChar(0,0,shortsname,-1,widesname,(int)strlen(shortsname)+1);
	}
	return widesname;
}

LPWSTR Spell::getRange(){
	LPWSTR widetext=L"";
	std::stringstream min;
	min <<spell_minrange;
	std::stringstream max;
	max <<spell_maxrange;
	std::string text ="";
	if (spell_minrange>0 && spell_maxrange>0) {
		text=min.str() + "-" + max.str() + " units";
	} else if (spell_minrange>0 && spell_maxrange==0 ){
		text=min.str() + "-inf" + " units";
	} else if (spell_minrange==0 && spell_maxrange>0 ){
		text=max.str() + " units";
	} else {
		return widetext;
	}
	LPCSTR shorttext=text.c_str();
	widetext=new WCHAR[strlen(shorttext)+1];
	MultiByteToWideChar(0,0,shorttext,-1,widetext,(int)strlen(shorttext)+1);
	return widetext;
}
LPWSTR Spell::getUsages(){
	LPWSTR widetext=L"";
	std::stringstream mana;
	mana <<spell_mana;
	std::stringstream energy;
	energy <<spell_energy;
	std::stringstream focus;
	focus <<spell_focus;
	std::stringstream hate;
	hate <<spell_rage;
	std::string text ="";
	text = mana.str() + "M " + energy.str() + "E " + focus.str() + "F " + hate.str() + "H ";
	LPCSTR shorttext=text.c_str();
	widetext=new WCHAR[strlen(shorttext)+1];
	MultiByteToWideChar(0,0,shorttext,-1,widetext,(int)strlen(shorttext)+1);
	return widetext;
}
LPWSTR Spell::getCastTime(){
	LPWSTR castTime=L"";
	if (spell_length>0) {
		castTime=L"sometime";
	} else {
		if (spell_attackqueue) {
			castTime=L"Next Melee";
		} else {
			castTime=L"Instant";
		}
	}
	return castTime;
}

LPWSTR Spell::getCooldown(){
	LPWSTR widetext=L"";
	std::stringstream str;
	int time =(int)(spell_cooldown)/1000;
	std::string s2 = "";
	if (time<60) {
		s2=" seconds";
	} else {
		time=time/60;
		s2=" mins";
	}
	str << time;
	std::string text =str.str()+s2;
	LPCSTR shorttext=text.c_str();
	widetext=new WCHAR[strlen(shorttext)+1];
	MultiByteToWideChar(0,0,shorttext,-1,widetext,(int)strlen(shorttext)+1);
	return widetext;
}
