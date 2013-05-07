#include <vector>
#ifndef AURA_H_
#define AURA_H_
#include "Aura.h"
#endif
class IAFLoader {
public:
void LoadIAF (std::vector <Aura*> *Auras);
void Element();
void CreateAura();
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
std::vector <Aura*> *loadauras;
Aura* currentAura;
size_t result;
size_t pos;
bool error;
};