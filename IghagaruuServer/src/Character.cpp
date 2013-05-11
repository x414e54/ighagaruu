#include "Character.h"
#include "Aura.h"
#include "Spell.h"
#include "Item.h"
#include "GameStats.h"

#include <mysql/mysql.h>
#define DEFAULT_BUFLEN 512
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
Character::Character(int cid, std::string cname, float cx, float cy, float cz, float co, int ch, int cm, int cfaction, int cclasss, int plusstam, int plusstr, int plusdex, int plusint, int pluswis, int pluspoints, int level, int xp, int money, TCPsocket sock, GameStats* gs) {
	gamestats=gs;
	info.autoAttack=false;
	info.casting=NULL;
	target=NULL;
	id=cid;
	info.name=cname;
	pos.x=cx;
	pos.y=cy;
	pos.z=cz;
	pos.orientation=co;
	stats.health=ch;
	stats.magika=cm;
	stats.energy=0;
	stats.hate=0;
	stats.focus=0;
	stats.stamina=1+plusstam;
	stats.strength=1+plusstr;
	stats.dexterity=1+plusdex;
	stats.intelligence=1+plusint;
	stats.wisdom=1+pluswis;
	inv.money=money;
	inv.xp=xp;
	inv.level=level;
	inv.pluspoints=pluspoints;
	ClientSocket=sock;
	info.faction=cfaction;
	info.classs=cclasss;

	switch(cfaction) {
		case 0: {
			switch(cclasss) {
				case 0: info.model=0; stats.stamina+=(2*inv.level); stats.strength+=(inv.level); break; //tank
				case 1: info.model=1; stats.stamina+=(inv.level); stats.dexterity+=(inv.level); stats.strength+=(2*inv.level);  break; //melee
				case 2: info.model=2; stats.stamina+=(int)(1.5*inv.level); stats.intelligence+=(inv.level); stats.wisdom+=(2*inv.level);  break; //healer
				case 3: info.model=3; stats.stamina+=(inv.level); stats.intelligence+=(2*inv.level);  stats.wisdom+=(inv.level); break; //range-dartsetc.
				default: info.model=0; break;
			} break;
		}
		case 1: {
			switch(cclasss) {
				case 0: info.model=4; stats.stamina+=(2*inv.level); stats.strength+=(inv.level); break; //tank
				case 1: info.model=5; stats.stamina+=(inv.level); stats.dexterity+=(inv.level); stats.strength+=(2*inv.level);  break; //melee
				case 2: info.model=6; stats.stamina+=(int)(1.5*inv.level); stats.intelligence+=(inv.level); stats.wisdom+=(2*inv.level);  break; //healer
				case 3: info.model=7; stats.stamina+=(inv.level); stats.intelligence+=(2*inv.level);  stats.wisdom+=(inv.level); break; //range-dartsetc.
				default: info.model=0; break;
			} break;
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
	for (int i=0; i<5; i++) {
		info.attackqueue[i]=NULL;
	}

	resetMaxHealth();
	if (stats.health<=0) { die();}
}

void Character::resetMaxHealth() {
	int weaponstamina=0;
	int weaponintellect=0;
	for (int i=0; i<6; i++) {
		if (inv.gear[i]!=NULL) {
			weaponstamina=weaponstamina+inv.gear[i]->item_stamina;
			weaponintellect=weaponintellect+inv.gear[i]->item_intelect;
		}
	}
	int aurastamina=0;
	int auraintellect=0;
	for (unsigned int i=0; i<info.auras.size(); i++) {
		aurastamina+=info.auras.at(i).aura->aura_stamina;
		auraintellect+=info.auras.at(i).aura->aura_intelligence;
	}
	stats.maxhealth=(stats.stamina+weaponstamina+aurastamina)*10;
	stats.maxmagika=(stats.intelligence+weaponintellect+auraintellect)*10;
	if (stats.health>stats.maxhealth) {stats.health=stats.maxhealth;}
	if (stats.magika>stats.maxmagika) {stats.magika=stats.maxmagika;}
}

void Character::Update() {
	int energyNew = stats.energy+20;
	int healthNew = stats.health+1;
	int magikaNew = stats.magika+1;
	int focusNew = stats.focus+100;
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
	if (info.autoAttack) {
		attack();
	}
	for (unsigned int i=0; i<info.auras.size(); i++) {
		if (info.auras.at(i).aura->aura_timeout>0) {
				info.auras.at(i).timeleft--;
		}
		info.auras.at(i).tick--;
		if (info.auras.at(i).timeleft>=0) {
			if (info.auras.at(i).tick==0) {
				info.auras.at(i).tick=info.auras.at(i).aura->aura_tick;
				info.auras.at(i).aura->tick(this);
			}
		} else {
				removeAura(i);
		}
	}
}

void Character::addXP(int amount)
{
	if (inv.level<10)
	{
		//send()
        int characterXp=amount;
        while (characterXp>0) {
            if (inv.level<10) {
                int maxxp = 100*(inv.level*inv.level);
                int newxp = inv.xp+characterXp;
                if (newxp>=maxxp) {
                    addLevel(1);
                    characterXp=newxp-maxxp;
                } else {
                    characterXp=newxp;
                    inv.xp=characterXp;
                    characterXp=0;
                }
            } else {
                characterXp=0;
            }
        }
	}
}

void Character::addMoney(int amount)
{
	inv.money=inv.money+amount;
}

void Character::addLevel(int characterLevel) {
        if (inv.level<10) {
            int newlevel=inv.level+characterLevel;
            if (newlevel<=10) {
                inv.level=newlevel;
				inv.pluspoints+=4;
            } else {
                inv.level=10;
            }
			//send()
        }
    }


void Character::attack() {
	if (target!=NULL) {
		if (info.attackqueue[0]==NULL) 
		{
			if (target->stats.health>0)  // target is alive
			{
				if (withinRange()) // in range
				{
					int attackdmg = 10;
					if ((target->stats.health)-attackdmg<=0)
					{
						target->stats.health=0;
						target->die();
						target=NULL;
						info.autoAttack=false;
						info.casting=NULL;
						addXP(0);
						addMoney(0);
					} else 
					{
						target->stats.health=(target->stats.health)-attackdmg;
						target->increaseHate(10);
					}
				} else // too far
				{
				}
			} else	// target is dead
			{
				info.autoAttack=false;
			}
		} else
		{
			info.attackqueue[0]->Cast(this,target,true, false);
			for (int i=0; i<4; i++) {
				info.attackqueue[i]=info.attackqueue[i+1];
			}
		}
	} else 
	{
		info.autoAttack=false;
	}
}

bool Character::withinRange()
{
	if (target!=NULL) {
		float x=pos.x-target->pos.x;
		float y=pos.y-target->pos.y;
		float d=sqrt((x*x)+(y*y));
		if (d<=5) return true;
	}
	return false;
}

void Character::increaseHate(int amount)
{
	int hateNew = stats.hate+amount;
	if (hateNew<=100) {stats.hate=hateNew;} else {stats.hate=100;}
}

void Character::cast() {
	if (info.casting!=NULL) {

		if (target!=NULL) {
			if (info.casting!=NULL) 
			{
				info.casttime=info.casttime-50;
				if (info.casttime<=0)
				{
					info.casting->Cast(this, target, false, true);
					info.casting=NULL;
				}
			}
		}
	}
}

int Character::calculateAttackDmg() {
	int basicmin=0;
	int basicmax=0;
	//if (gear[6]!=NULL) {
//	}
	return 1;
}

void Character::removeAura(int i) {
	AURA aura=info.auras.at(i);
	info.auras.erase(info.auras.begin()+i);
	char sendbuff[512];
	sendbuff[0]=13;
	sendbuff[1]=1;
	int bufint=2;
	INTARR ai(aura.aura->aura_id);
	for (int i=0; i<4; i++) {
		sendbuff[bufint]=ai.b[i];
		bufint++;
	}
	int appid=-1;
	if (aura.applier!=NULL) {
		appid=aura.applier->id;
	}
	INTARR ap(appid);
	for (int i=0; i<4; i++) {
		sendbuff[bufint]=ap.b[i];
		bufint++;
	}
	SDLNet_TCP_Send(ClientSocket, sendbuff, DEFAULT_BUFLEN);
	resetMaxHealth();
}

void Character::addAura(Aura* aura, Character* applier) {
	AURA a(aura,aura->aura_timeout,aura->aura_tick, applier);
	info.auras.push_back(a);
	char sendbuff[512];
	sendbuff[0]=13;
	sendbuff[1]=0;
	int bufint=2;
	INTARR ai(aura->aura_id);
	for (int i=0; i<4; i++) {
		sendbuff[bufint]=ai.b[i];
		bufint++;
	}
	INTARR at(aura->aura_timeout);
	for (int i=0; i<4; i++) {
		sendbuff[bufint]=at.b[i];
		bufint++;
	}
	int appid=-1;
	if (applier!=NULL) {
		appid=applier->id;
	}
	INTARR ap(appid);
	for (int i=0; i<4; i++) {
		sendbuff[bufint]=ap.b[i];
		bufint++;
	}
	SDLNet_TCP_Send(ClientSocket, sendbuff, DEFAULT_BUFLEN);
	resetMaxHealth();
}

void Character::die() {
	float startx;
	float starty=0.0;
	if (info.faction==0) {
		startx=80.0;
	} else {
		startx=-80.0;
	}
	target=NULL;
	info.autoAttack=false;
	info.casting=NULL;
	for (unsigned int i=0; i<info.auras.size(); i++) {
		if (info.auras.at(i).aura->aura_flag) {
			gamestats->ResetFlag();
		}
		if (info.auras.at(i).aura->aura_removeondeath) {
			removeAura(i);
		}
	}
	stats.focus=0;
	stats.hate=0;
	pos.x=startx;
	pos.y=0.0;
	pos.z=0.0;
	resetPos();
	resetMaxHealth();
	stats.health=stats.maxhealth;
	stats.magika=stats.maxmagika;
	char sendbuff[512];
	sendbuff[0]=17;
	sendbuff[1]=0;
	SDLNet_TCP_Send(ClientSocket, sendbuff, DEFAULT_BUFLEN);
}

void Character::resetPos() {
		char sendbuff[512];
		sendbuff[0]=0;  // return type 0 = stats

		///
		sendbuff[1]=0;  // poisition stats

		int bufint=2;
		FLOATARR o(pos.orientation);
		for (int i=0; i<8; i++) {
			sendbuff[bufint]=o.b[i];
			bufint++;
		}
		FLOATARR x(pos.x);
		for (int i=0; i<8; i++) {
			sendbuff[bufint]=x.b[i];
			bufint++;
		}
		FLOATARR y(pos.y);
		for (int i=0; i<8; i++) {
			sendbuff[bufint]=y.b[i];
			bufint++;
		}
		FLOATARR z(pos.z);
		for (int i=0; i<8; i++) {
			sendbuff[bufint]=z.b[i];
			bufint++;
		}
		SDLNet_TCP_Send(ClientSocket, sendbuff, DEFAULT_BUFLEN);
}

void Character::addCash(int csh) {
	MYSQL *conn;
	char *server = "localhost";
	char *user = "root";
	char *password = "";
	char *database = "ighagaruu";
	conn = mysql_init(NULL);
	mysql_real_connect(conn, server,user, password, database, 0, NULL, 0);

	inv.money+=csh;
	char query[512];
	snprintf(query,512,"UPDATE characters SET character_money='%i' WHERE character_id='%i'",inv.money, id);

	mysql_query(conn, query);
	mysql_close(conn); 
}
