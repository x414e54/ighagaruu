#pragma once
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include<sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
class Tokenizer {
public:
Tokenizer(std::wstring string, wchar_t delimiter);
Tokenizer(std::wstring string, unsigned int offset, wchar_t delimiter);
~Tokenizer();
std::wstring getNext();
std::wstring getRemaining();
static std::wstring toUppercase(std::wstring mcString);
static std::wstring intToString(int n);
bool hasNext();
private:

	std::wstring currentString;
	wchar_t currentDelimiter;
};

#endif
