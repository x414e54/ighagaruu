#ifndef AREATRIGGER_H_
#define AREATRIGGER_H_
#include "AreaTrigger.h"
#endif

AreaTrigger::AreaTrigger(GameStats* gs, std::vector <Aura*> *as, std::vector <AreaTrigger*> *at){
	gamestats=gs;
	auras=as;
	triggers=at;
	areatrigger_enabled=false;
	areatrigger_id=-1;
	areatrigger_faction=0;
	areatrigger_teleport=false;
	areatrigger_telex=0;
	areatrigger_teley=0;
	areatrigger_telez=0;
	areatrigger_giveaura=0;
	areatrigger_needaura=0;
	areatrigger_addpoints=false;
	areatrigger_disableontouch=false;
	areatrigger_cooldown=1000;
	areatrigger_oncooldown=false;
	areatrigger_enabletrigger=0;
	areatrigger_flag=false;
}

void AreaTrigger::Action(Character* chara) {
	if (areatrigger_enabled && (areatrigger_faction==-1 || areatrigger_faction==chara->info.faction)) {
		bool canTrigger=true;
		if (areatrigger_needaura>0) {
			canTrigger=false;
		}
		for (unsigned int i=0; i<chara->info.auras.size(); i++) {
			if (chara->info.auras.at(i).aura->aura_id==areatrigger_needaura) {
				canTrigger=true;
				chara->removeAura(i);
				break;
			}
		}
		if (canTrigger) {
			if(areatrigger_giveaura>0) {
				bool alreadygot=false;
				for (unsigned int i=0; i<chara->info.auras.size(); i++) {
					if(chara->info.auras.at(i).aura->aura_id==areatrigger_giveaura) {
						alreadygot=true;
						break;
					}

				}
				if (!alreadygot) {
					for (unsigned int i=0; i<auras->size(); i++) {
						if(auras->at(i)->aura_id==areatrigger_giveaura) {
							chara->addAura(auras->at(i), NULL);
							break;
						}

					}
				}
			}
			if(areatrigger_addpoints) { 
				if(areatrigger_faction==0) {
					gamestats->RedScore();
				} else if(areatrigger_faction==1) {
					gamestats->BlueScore();
				}
			}
			if(areatrigger_teleport) {
				chara->pos.x=areatrigger_telex;
				chara->pos.y=areatrigger_teley;
				chara->pos.z=areatrigger_telez;
			}
			if(areatrigger_disableontouch) {
				areatrigger_enabled=false;
			} else {
				areatrigger_oncooldown=true;
				areatrigger_enabled=false;
			}
			if(areatrigger_flag) {
				gamestats->flagtrigger=this;
				gamestats->PickUpFlag(chara);
			}
			if(areatrigger_enabletrigger>0) {
				for (unsigned int i=0; i<triggers->size(); i++) {
					if(triggers->at(i)->areatrigger_id==areatrigger_enabletrigger) {
						triggers->at(i)->areatrigger_enabled=true;
						if(triggers->at(i)->areatrigger_flag=true) {
							gamestats->flagcarrier=NULL;
						}
						break;
					}
				}
			}
		}
	}
}

bool AreaTrigger::WithinTrigger(Character* chara) {
	if(chara->pos.x<=box.u.x && chara->pos.y<=box.u.y && chara->pos.z<=box.u.z && chara->pos.x>=box.l.x && chara->pos.y>=box.l.y && chara->pos.z>=box.l.z) {
		return true;
	} else {
		return false;
	}
}

void AreaTrigger::Cooldown() {
	if (areatrigger_oncooldown) {
		areatrigger_cooldown=areatrigger_cooldown-1000;
		if (areatrigger_cooldown<=0) {
			areatrigger_enabled=true;
			areatrigger_cooldown=1000;
			areatrigger_oncooldown=false;
		}
	}
}