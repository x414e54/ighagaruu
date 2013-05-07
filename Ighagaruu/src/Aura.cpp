//--------------------------------------------------------------------------------------
// Aura.cpp
// The aura class, contains structs and information about each aura object. 
//--------------------------------------------------------------------------------------

#include "Aura.h"
#include <stdlib.h>
Aura::Aura() {
			   aura_id=-1;
			   aura_speed=0;
			   aura_icon=0;
			   aura_description="Error";
			   aura_name="Error";
}

void Aura::Set(std::string element, std::string value) {
	if (element=="aura_id") {
		aura_id=atoi(value.c_str());
	} else if (element=="aura_speed") {
		aura_speed=atoi(value.c_str());
	} else if (element=="aura_icon") {
	    aura_icon=atoi(value.c_str());
	} else if (element=="aura_description") {
	    aura_description=value;
	} else if (element=="aura_name") {
	    aura_name=value;
	}
}
