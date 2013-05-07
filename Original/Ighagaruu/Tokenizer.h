#include<sstream>
#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
class Tokenizer {
public:
Tokenizer(const wchar_t* string, wchar_t delimiter);
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
