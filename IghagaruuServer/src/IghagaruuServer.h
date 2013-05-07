 #ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_net.h>
#include <stdio.h>
#include <vector>
#include <string>


#include <mysql/mysql.h>
#ifndef SPELL_H_
#define SPELL_H_
#include "Spell.h"
#endif
#ifndef ITEM_H_
#define ITEM_H_
#include "Item.h"
#endif
#include "Account.h"
#ifndef CHARACTER_H_
#define CHARACTER_H_
#include "Character.h"
#endif
#ifndef AREATRIGGER_H_
#define AREATRIGGER_H_
#include "AreaTrigger.h"
#endif
#ifndef GAMESTATS_H_
#define GAMESTATS_H_
#include "GameStats.h"
#endif
#include "Tokenizer.h"
DWORD WINAPI ListenThread(LPVOID lpParam);
DWORD WINAPI ServerThread(LPVOID lpParam);
DWORD WINAPI UpdateThread(LPVOID lpParam);
void LoadSpells();
void LoadItems();
void LoadAuras();
void LoadAreaTriggers();
void ResetGame();
#define DEFAULT_PORT "27115"
#define DEFAULT_BUFLEN 512
