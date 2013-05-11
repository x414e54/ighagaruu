#include "Item.h"
Item::Item(int itemid,
		   int itemdmg,
		   int itemheal,
		   int itemmana,
		   int itemcooldown,
		   int itemrage,
		   int itemfocus,
		   int itemenergy,
		   int itemgiverage,
		   int itemgivemana,
		   int itemgiveenergy,
		   int itemgivefocus,
		   int itemminrange,
		   int itemmaxrange,
		   int itemlength,
		   bool itemglobalcooldown,
		   int itemprojectile,
		   int itemexplosion,
		   int itemuseaura,
		   bool itemselfcast,
		   int itemselfheal,
		   int itemselfdmg,
		   int itemstamina,
		   int itemintelect,
		   int itemstrength,
		   int itemtype,
		   int itemdescription,
		   int itemmaxdmg,
		   int itemmindmg,
		   bool itemuseable,
		   int itemclass,
		   int itemfaction,
		   int itemcharges,
		   int itemdexterity,
		   int itemwisdom,
		   bool itemuseperc,
		   int itemslot,
		   int itemcost) {
			   item_id=itemid;
			   item_dmg=itemdmg;
			   item_heal=itemheal;
			   item_mana=itemmana;
			   item_cooldown=itemcooldown;
			   item_rage=itemrage;
			   item_focus=itemfocus;
			   item_energy=itemenergy;
			   item_giverage=itemgiverage;
			   item_givemana=itemgivemana;
			   item_giveenergy=itemgiveenergy;
			   item_givefocus=itemgivefocus;
			   item_minrange=itemminrange;
			   item_maxrange=itemmaxrange;
			   item_length=itemlength;
			   item_globalcooldown=itemglobalcooldown;
			   item_projectile=itemprojectile;
			   item_explosion=itemexplosion;
			   item_useaura=itemuseaura;
			   item_selfcast=itemselfcast;
			   item_selfheal=itemselfheal;
			   item_selfdmg=itemselfdmg;
			   item_stamina=itemstamina;
			   item_intelect=itemintelect;
			   item_strength=itemstrength;
			   item_type=itemtype;
			   item_maxdmg=itemmaxdmg;
			   item_mindmg=itemmindmg;
			   item_useable=itemuseable;
			   item_class=itemclass;
			   item_faction=itemfaction;
			   item_charges=itemcharges;
			   item_dexterity=itemdexterity;
			   item_wisdom=itemwisdom;
			   item_useperc=itemuseperc;
			   item_slot=itemslot;
			   item_cost=itemcost;
}
