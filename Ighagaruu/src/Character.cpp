//--------------------------------------------------------------------------------------
// Character.cpp
// The character class, contains structs and information about each character object.
//--------------------------------------------------------------------------------------

#include "Character.h"
#include <sstream>

Character::Character(int cid) {
	id=cid;
	pos.x=80;
	pos.y=0;
	pos.z=0;
	pos.orientation=0;
	stats.health=0;
	stats.maxhealth=1000;
	stats.magika=0;
	stats.maxmagika=1000;
	stats.energy=0;
	stats.hate=0;
	stats.focus=0;
	inv.money=0;
	inv.xp=0;
	inv.level=1;
	info.faction=0;
	info.classs=0;
	info.model=0;
}

Character::Character(int cid, std::wstring ccharname, int cfaction, int cclasss) {
	charname=ccharname;
	id=cid;
	pos.x=80;
	pos.y=0;
	pos.z=0;
	pos.orientation=0;
	stats.health=0;
	stats.maxhealth=1;
	stats.magika=0;
	stats.maxmagika=1;
	stats.energy=0;
	stats.hate=0;
	stats.focus=0;
	inv.money=0;
	inv.xp=0;
	inv.level=1;
	info.faction=cfaction;
	info.classs=cclasss;
	info.casting=NULL;
	switch(cfaction) {
		case 0: {
			pos.x=80;
			switch(cclasss) {
				case 0: info.model=0; break;
				case 1: info.model=1; break;
				case 2: info.model=2; break;
				case 3: info.model=3; break;
				default: info.model=0; break;
			}
			 break;
		}
		case 1: {
			pos.x=-80;
			switch(cclasss) {
				case 0: info.model=4; break;
				case 1: info.model=5; break;
				case 2: info.model=6; break;
				case 3: info.model=7; break;
				default: info.model=0; break;
			}
			 break;
		}
		default: info.model=0; break;
	}
	for (int i=0; i<6; i++) {
		inv.items[i]=NULL;
	}
	for (int i=0; i<6; i++) {
		inv.gear[i]=NULL;
	}
	for (int i=0; i<11; i++) {
		inv.spells[i]=NULL;
	}
}
void Character::Update(int timeDiff) {
	int energyNew = stats.energy+20*timeDiff;
	int healthNew = stats.health+1*timeDiff;
	int magikaNew = stats.magika+1*timeDiff;
	int focusNew = stats.focus+100*timeDiff;
	if (energyNew >100) {
				energyNew=100;
	}
	stats.energy=energyNew;
	if (focusNew >1000) {
				focusNew=1000;
	}
	stats.focus=focusNew;
	if (healthNew >stats.maxhealth) {
		healthNew=stats.maxhealth;
	}
	stats.health=healthNew;
	if (magikaNew >stats.maxmagika) {
		magikaNew=stats.maxmagika;
	}
	stats.magika=magikaNew;
}

void Character::SetOrientation(float o) {
	pos.orientation=o;
}

std::wstring Character::getName(){
	return charname;
}

void Character::tickCast(int time) {
	//time=time;
	if ((info.casttime-time)<=0) {
		info.casttime=0;
	} else {
		info.casttime=(info.casttime-time);
	}
}

std::wstring Character::getCastText(){
	if (info.casting!=NULL) {
		std::wstringstream str;
		str.precision(1);
		double time =(double)(info.casttime)/1000;
		str << std::fixed <<time;
		std::wstring text =str.str()+L"s";
		return text;
	}
	return L"";
}

void Character::addAura(Aura* aura, Character* app, int timeout) {
	AURA a(aura, app, timeout);
	auras.push_back(a);
}

void Character::removeAura(int auraid, int appid) {
	for (unsigned int i=0; i<auras.size(); i++) {
		int applierid=-1;
		if (auras.at(i).applier!=NULL) {
			applierid=auras.at(i).applier->id;
		}
		if (auras.at(i).aura->aura_id==auraid && applierid==appid) {
			auras.erase(auras.begin()+i);
		}
	}
}
