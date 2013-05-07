#pragma once
#ifndef AURA_H
#define AURA_H

#include <string>

class Aura {
public:
	Aura();
	void Set(std::string element, std::string value);
	int aura_id;
	int aura_speed;
	int aura_icon;
	std::string aura_description;
	std::string aura_name;
};

#endif
