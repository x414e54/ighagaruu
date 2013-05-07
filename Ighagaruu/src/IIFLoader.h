#pragma once
#ifndef IIFLOADER_H
#define IIFLOADER_H

#include <vector>
#include "Item.h"

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

#endif
