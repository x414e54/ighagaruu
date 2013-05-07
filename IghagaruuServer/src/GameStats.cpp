#ifndef GAMESTATS_H_
#define GAMESTATS_H_
#include "GameStats.h"
#endif

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
				send(characters->at(i)->ClientSocket, sendbuff, 512, 0 );
		}
	}

void GameStats::PickUpFlag(Character* chara) {
		flagcarrier=chara;
		char sendbuff[512];
		sendbuff[0]=17;
		for (unsigned int i=0; i<characters->size(); i++) {
			if (characters->at(i)->info.faction==chara->info.faction) {
				sendbuff[1]=5;
				send(characters->at(i)->ClientSocket, sendbuff, 512, 0 );
			} else {
				sendbuff[1]=6;
				send(characters->at(i)->ClientSocket, sendbuff, 512, 0 );
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
				send(characters->at(i)->ClientSocket, sendbuff, 512, 0 );
			} else {
				sendbuff[1]=2;
				send(characters->at(i)->ClientSocket, sendbuff, 512, 0 );
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
				send(characters->at(i)->ClientSocket, sendbuff, 512, 0 );
			} else {
				sendbuff[1]=2;
				send(characters->at(i)->ClientSocket, sendbuff, 512, 0 );
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
				send(characters->at(i)->ClientSocket, sendbuff, 512, 0 );
				characters->at(i)->addCash(10);
			} else {
				sendbuff[1]=4;
				send(characters->at(i)->ClientSocket, sendbuff, 512, 0 );
			}
		}
		ResetGame();
	}