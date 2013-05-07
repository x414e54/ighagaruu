#include <vector>
#ifndef ITEM_H_
#define ITEM_H_
#include "Item.h"
#endif
class IIFLoader {
public:
void LoadIIF (std::vector <Item*> *Items);
void Element();
void CreateItem();
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
std::vector <Item*> *loaditems;
Item* currentItem;
size_t result;
size_t pos;
bool error;
};