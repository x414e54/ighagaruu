#ifndef SPELL_H_
#define SPELL_H_
#include "Spell.h"
#endif

Spell::Spell(int spellid,
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
			  std::vector <Aura*> *as
			  )
{
	auras=as;
	spell_id=spellid;
	spell_faction=spellfaction;
	spell_class=spellclass;
	spell_type=spelltype;
	spell_dmg=spelldmg;
	spell_heal=spellheal;
	spell_mana=spellmana;
	spell_cooldown=spellcooldown;
	spell_level=spelllevel;
	spell_rage=spellrage;
	spell_focus=spellfocus;
	spell_energy=spellenergy;
	spell_giverage=spellgiverage;
	spell_givemana=spellgivemana;
	spell_giveenergy=spellgiveenergy;
	spell_givefocus=spellgivefocus;
	spell_minrange=spellminrange;
	spell_maxrange=spellmaxrange;
	spell_length=spelllength;
	spell_globalcooldown=spellglobalcooldown;
	spell_projectile=spellprojectile;
	spell_explosion=spellexplosion;
	spell_useaura=spelluseaura;
	spell_selfcast=spellselfcast;
	spell_selfheal=spellselfheal;
	spell_selfdmg=spellselfdmg;
	spell_drainlife=spelldrainlife;
	spell_drainmana=spelldrainmana;
	spell_drainrage=spelldrainrage;
	spell_drainfocus=spelldrainfocus;
	spell_drainenergy=spelldrainenergy;
	spell_useperc=spelluseperc;
	spell_cost=spellcost;
	spell_attackqueue=spellattackqueue;
}


void Spell::Cast(Character* char1, Character* char2, bool queued, bool casted) {
	char sendbuff[512];
	int bufint;
	if (char2->stats.health>0)  // target is alive
	{
		int withinrange=withinRange(char1, char2);
		if (withinrange==1) // in range
		{
			if ((char1->stats.magika)-spell_mana>=0)  // enough magika
			{
				if ((char1->stats.energy)-spell_energy>=0)  // enough e
				{
					if ((char1->stats.hate)-spell_rage>=0)  // enough r
					{
						if ((char1->stats.focus)-spell_focus>=0)  // enough f
						{
							if ((spell_length==0 || casted) && (!spell_attackqueue || queued))  // instant
							{
								sendbuff[0]=6; 
								sendbuff[1]=1; 
								send( char1->ClientSocket, sendbuff, 512, 0 );
								char1->stats.magika=char1->stats.magika-spell_mana;
								char1->stats.energy=char1->stats.energy-spell_energy;
								char1->stats.hate=char1->stats.hate-spell_rage;
								char1->stats.focus=char1->stats.focus-spell_focus;

								sendbuff[0]=6; 
								sendbuff[1]=3; 
								bufint=2;
								INTARR c(char1->id);
								for (int i=0; i<4; i++) {
									sendbuff[bufint]=c.b[i];
									bufint++;		
								}
								INTARR s(spell_id);
								for (int i=0; i<4; i++) {
									sendbuff[bufint]=s.b[i];
									bufint++;		
								}
								INTARR a(spell_dmg);
								for (int i=0; i<4; i++) {
									sendbuff[bufint]=a.b[i];
									bufint++;		
								}
								send( char2->ClientSocket, sendbuff, 512, 0 );
								sendbuff[0]=6; 
								sendbuff[1]=2; 
								bufint=2;
								INTARR c2(char2->id);
								for (int i=0; i<4; i++) {
									sendbuff[bufint]=c2.b[i];
									bufint++;		
								}
								INTARR s2(spell_id);
								for (int i=0; i<4; i++) {
									sendbuff[bufint]=s2.b[i];
									bufint++;		
								}
								INTARR a2(spell_dmg);
								for (int i=0; i<4; i++) {
									sendbuff[bufint]=a2.b[i];
									bufint++;		
								}
								send( char1->ClientSocket, sendbuff, 512, 0 );

								if ((char2->stats.health)-spell_dmg<=0)
								{
									char2->stats.health=0;
									char2->die();
									char1->info.autoAttack=false;
									char1->info.casting=NULL;
									char1->addXP(0);
									char1->addMoney(0);
								} else 
								{
									char2->stats.health=(char2->stats.health)-spell_dmg;
									if (spell_useaura>0) {
										bool alreadygot=false;
										for (unsigned int i=0; i<char2->info.auras.size(); i++) {
											if(char2->info.auras.at(i).aura->aura_id==spell_useaura) {
												alreadygot=true;
												break;
											}	
										}
										if (!alreadygot) {
											for (unsigned int i=0; i<auras->size(); i++) {
											if(auras->at(i)->aura_id==spell_useaura) {
													char2->addAura(auras->at(i), char1);
													break;
												}	

											}
										}
									}
								}
							} else
							{
								if (spell_attackqueue) //queued attack
								{
									int i=0;
									while(char1->info.attackqueue[i]!=NULL && i<5) {
										i++;
									}
									char1->info.attackqueue[i]=this;
								} else					// casted attack
								{
									char1->info.casting=this;
									char1->info.casttime=spell_length;
									sendbuff[0]=6; 
									sendbuff[1]=0; 
									bufint=2;
									INTARR t(spell_id);
									for (int i=0; i<4; i++) {
										sendbuff[bufint]=t.b[i];
										bufint++;		
									}
									send( char1->ClientSocket, sendbuff, 512, 0 );
								}
							}
						} else
						{
								sendbuff[0]=6; //focus
								sendbuff[1]=4; 
								send( char1->ClientSocket, sendbuff, 512, 0 );
						}
					} else
					{
								sendbuff[0]=6; //rage
								sendbuff[1]=5;
								send( char1->ClientSocket, sendbuff, 512, 0 );
					}
				} else
				{
								sendbuff[0]=6; //energy
								sendbuff[1]=6;
								send( char1->ClientSocket, sendbuff, 512, 0 );
				}
			} else
			{
								sendbuff[0]=6; //not enough magika
								sendbuff[1]=7;
								send( char1->ClientSocket, sendbuff, 512, 0 );
			}
		} else if (withinrange==0) // too close
		{
								sendbuff[0]=6; 
								sendbuff[1]=8;
								send( char1->ClientSocket, sendbuff, 512, 0 );
		} else if (withinrange==2) // too far
		{
								sendbuff[0]=6; 
								sendbuff[1]=9;
								send( char1->ClientSocket, sendbuff, 512, 0 );
		}
	} else	// target is dead
	{
								sendbuff[0]=6; 
								sendbuff[1]=10;
								send( char1->ClientSocket, sendbuff, 512, 0 );
	}

}

int Spell::withinRange(Character* char1, Character* char2)
{
	float x=char1->pos.x-char2->pos.x;
	float y=char1->pos.y-char2->pos.y;
	float d=sqrt((x*x)+(y*y));
	if (d<spell_minrange) return 0;
	if (d>spell_maxrange) return 2;
	return 1;
}

