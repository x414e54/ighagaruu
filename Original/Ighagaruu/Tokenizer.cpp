#include"DXUT.h"
#include"Tokenizer.h"
Tokenizer::Tokenizer(const wchar_t* string, wchar_t delimiter)
{
	currentString=string;
	currentDelimiter=delimiter;
}
Tokenizer::~Tokenizer(){
}
std::wstring Tokenizer::getNext(){
	if (hasNext()) {
	std::wstring functionString;
	functionString=currentString;
	size_t index = functionString.find_first_of(currentDelimiter,0);
    std::wstring token=functionString.substr(0, index);
	std::wstring remaining=functionString.substr(index+1);
	std::wstring nextString=L"hello";
	nextString=token;
	currentString=remaining;
	return nextString;
	} else {
	return L"";
	}
}
std::wstring Tokenizer::getRemaining()
{
	return currentString;
}
bool Tokenizer::hasNext()
{
	std::wstring functionString;
	functionString=currentString;
	size_t index= functionString.find_first_of(currentDelimiter,0);
	int ind=(int)index;
	if (ind>=0) {
	return true;
	} else {
		return false;
	}
}

std::wstring Tokenizer::toUppercase(std::wstring mcString) {
 for (unsigned int i=0; i<mcString.length(); i++)
 {
  mcString[i]=toupper(mcString[i]);
 }
 return mcString;
}

std::wstring Tokenizer::intToString(int n) {
	std::wstringstream str;
	str << n;
 return str.str();
}

