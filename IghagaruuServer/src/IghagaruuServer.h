#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <vector>
#include <string>

#include <mysql/mysql.h>
#include "Spell.h"
#include "Item.h"
#include "Account.h"
#include "Character.h"
#include "AreaTrigger.h"
#include "GameStats.h"
#include "Tokenizer.h"

SDL_Thread* listenThread=NULL;
SDL_Thread* serverThread=NULL;
int ListenThread(void *data);
int ServerThread(void *data);
int UpdateThread(void *data);
void LoadSpells();
void LoadItems();
void LoadAuras();
void LoadAreaTriggers();
void ResetGame();
#define DEFAULT_PORT 27115
#define DEFAULT_BUFLEN 512
