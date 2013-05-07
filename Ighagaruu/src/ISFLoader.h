#pragma once
#ifndef ISFLOADER_H
#define ISFLOADER_H

#include <vector>
#include "Spell.h"

class ISFLoader {
public:
void LoadISF (std::vector <Spell*> *spells);
void Element();
void CreateSpell();
void Start();
void StartElement();
void EndElement();
void Value();
void GetNext();
void Match(char m);
long lSize;
char * buffer;
std::string currentElement;
std::string endElement;
std::string currentValue;
std::vector <Spell*> *loadspells;
Spell* currentSpell;
size_t result;
size_t pos;
bool error;
};

#endif
