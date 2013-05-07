#include"Tokenizer.h"
Tokenizer::Tokenizer(const char* string, char delimiter)
{
	currentString=string;
	currentDelimiter=delimiter;
}
Tokenizer::~Tokenizer(){
}
std::string Tokenizer::getNext(){
	if (hasNext()) {
	std::string functionString;
	functionString=currentString;
	size_t index = functionString.find_first_of(currentDelimiter,0);
    std::string token=functionString.substr(0, index);
	std::string remaining=functionString.substr(index+1);
	std::string nextString="hello";
	nextString=token;
	currentString=remaining;
	return nextString;
	} else {
	return "";
	}
}
std::string Tokenizer::getRemaining()
{
	return currentString;
}
bool Tokenizer::hasNext()
{
	std::string functionString;
	functionString=currentString;
	size_t index= functionString.find_first_of(currentDelimiter,0);
	int ind=(int)index;
	if (ind>=0) {
	return true;
	} else {
		return false;
	}
}

std::string Tokenizer::toUppercase(std::string mcString) {
 for (unsigned int i=0; i<mcString.length(); i++)
 {
  mcString[i]=toupper(mcString[i]);
 }
 return mcString;
}

std::string Tokenizer::intToString(int n) {
	std::stringstream str;
	str << n;
 return str.str();
}

