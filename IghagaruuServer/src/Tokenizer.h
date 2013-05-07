#include<sstream>
class Tokenizer {
public:
Tokenizer(const char* string, char delimiter);
Tokenizer(const char* string, char delimiter);
~Tokenizer();
std::string getNext();
std::string getRemaining();
static std::string toUppercase(std::string mcString);
static std::string intToString(int n);
bool hasNext();
private:

	std::string currentString;
	char currentDelimiter;
};
