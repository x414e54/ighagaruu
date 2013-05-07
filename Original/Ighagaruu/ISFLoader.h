#include <vector>
#ifndef SPELL_H_
#define SPELL_H_
#include "Spell.h"
#endif
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
FILE* file;
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