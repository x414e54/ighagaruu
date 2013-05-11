#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
class Account {
public:

Account(int auserid, std::string ausername, int agamelevel);

std::string username;
int	userid;
int	gamelevel;
private:

};
#endif
