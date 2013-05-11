#include "GameStats.h"
#include <SDL2/SDL_net.h>
#include <mysql/mysql.h>
#include "Character.h"
#include "Aura.h"
#include "AreaTrigger.h"
#define DEFAULT_BUFLEN 512

GameStats::GameStats(std::vector <Character*> *chs) {
	characters=chs;
	ResetGame();
	}

void GameStats::ResetFlag() {
		flagcarrier=NULL;
		flagtrigger->areatrigger_enabled=true;
		char sendbuff[512];
		sendbuff[0]=17;
		for (unsigned int i=0; i<characters->size(); i++) {
				sendbuff[1]=7;
				SDLNet_TCP_Send(characters->at(i)->ClientSocket, sendbuff, DEFAULT_BUFLEN);
		}
	}

void GameStats::PickUpFlag(Character* chara) {
		flagcarrier=chara;
		char sendbuff[512];
		sendbuff[0]=17;
		for (unsigned int i=0; i<characters->size(); i++) {
			if (characters->at(i)->info.faction==chara->info.faction) {
				sendbuff[1]=5;
				SDLNet_TCP_Send(characters->at(i)->ClientSocket, sendbuff, DEFAULT_BUFLEN);
			} else {
				sendbuff[1]=6;
				SDLNet_TCP_Send(characters->at(i)->ClientSocket, sendbuff, DEFAULT_BUFLEN);
			}
		}
	}

void GameStats::ResetGame() {
		blueteam=0;
		redteam=0;
		flagcarrier=NULL;
	}
void GameStats::RedScore() {
		redteam++;
		flagcarrier=NULL;
		flagtrigger->areatrigger_enabled=true;
		char sendbuff[512];
		sendbuff[0]=17;
		for (unsigned int i=0; i<characters->size(); i++) {
			if (characters->at(i)->info.faction==0) {
				sendbuff[1]=1;
				SDLNet_TCP_Send(characters->at(i)->ClientSocket, sendbuff, DEFAULT_BUFLEN);
			} else {
				sendbuff[1]=2;
				SDLNet_TCP_Send(characters->at(i)->ClientSocket, sendbuff, DEFAULT_BUFLEN);
			}
		}
		if (redteam>=5) {
			EndRound(true);
		}
	}
void GameStats::BlueScore() {
		blueteam++;
		flagcarrier=NULL;
		flagtrigger->areatrigger_enabled=true;
		char sendbuff[512];
		sendbuff[0]=17;
		for (unsigned int i=0; i<characters->size(); i++) {
			if (characters->at(i)->info.faction==1) {
				sendbuff[1]=1;
				SDLNet_TCP_Send(characters->at(i)->ClientSocket, sendbuff, DEFAULT_BUFLEN);
			} else {
				sendbuff[1]=2;
				SDLNet_TCP_Send(characters->at(i)->ClientSocket, sendbuff, DEFAULT_BUFLEN);
			}
		}
		if (blueteam>=5) {
			EndRound(false);
		}
	}
void GameStats::EndRound(bool winner) {
		int winningfaction;
		char sendbuff[512];
		sendbuff[0]=17;
		if (winner) {
			winningfaction=0;
		} else {
			winningfaction=1;
		}
		for (unsigned int i=0; i<characters->size(); i++) {
			if (characters->at(i)->info.faction==winningfaction) {
				sendbuff[1]=3;
				SDLNet_TCP_Send(characters->at(i)->ClientSocket, sendbuff, DEFAULT_BUFLEN);
				characters->at(i)->addCash(10);
			} else {
				sendbuff[1]=4;
				SDLNet_TCP_Send(characters->at(i)->ClientSocket, sendbuff, DEFAULT_BUFLEN);
			}
		}
		ResetGame();
	}
