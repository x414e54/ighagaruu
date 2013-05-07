//--------------------------------------------------------------------------------------
// ISFLoader.cpp
// This class loads up the game spells from the media folder to use in the game.
// This removes the need to continuatlly contact the game server for spell information.
//--------------------------------------------------------------------------------------

#include "dxut.h"
#ifndef SPELL_H_
#define SPELL_H_
#include "Spell.h"
#endif
#include "ISFLoader.h"



void ISFLoader::LoadISF(std::vector <Spell*> *spells) {
	loadspells=spells;
	error=false;
	pos=0;
	fopen_s(&file, "media\\spells.ISF", "r" );
	if( file!= NULL )
	{
		fseek (file , 0 , SEEK_END);
		lSize = ftell (file);
		rewind (file);

		// allocate memory to contain the whole file:
		buffer = (char*) malloc (sizeof(char)*lSize);
		if (buffer == NULL) {exit (2);}

		// copy the file into the buffer:
		result = fread (buffer,1,lSize,file);
		//if (result != lSize) {exit (3);}

		Match('<');
		Match('i');
		Match('g');
		Match('h');
		Match('a');
		Match('g');
		Match('a');
		Match('r');
		Match('u');
		Match('u');
		Match('>');
		if (!error) {
			Start();
		}
		// terminate
		fclose (file);
		//free (buffer);
	}
	else{
		//
	}

}

void ISFLoader::Start() {
	switch (buffer[0]) {
		case '<': CreateSpell(); break;
		case ' ': GetNext(); Start(); break;
		case 10: GetNext(); Start(); break;
		default: break;
	}
}

void ISFLoader::CreateSpell() {
	currentSpell = new Spell();
	GetNext(); 
	switch (buffer[0]) {
		case '/': {
			Match('i');
			Match('g');
			Match('h');
			Match('a');
			Match('g');
			Match('a');
			Match('r');
			Match('u');
			Match('u');
			Match('>');
			break;
				  }
		default: { 
			Match('s');
			Match('p');
			Match('e');
			Match('l');
			Match('l');
			Match('s');
			Match('>');
			StartElement();
			break;
				 }
	}

}

void ISFLoader::StartElement() {
	currentElement="";
	endElement="";
	currentValue="";
switch (buffer[0]) {
		case '<': Element(); break;
		case ' ': GetNext(); StartElement(); break;
		case 10: GetNext(); StartElement(); break;
		default: break;
	}
}
void ISFLoader::Element() {
	GetNext();
	switch (buffer[0]) {
		case '>': Value(); break;
		case ' ': Element(); break;
		case '/': EndElement(); break;
		case 10: Element(); break;
		default: currentElement=currentElement+buffer[0]; Element(); break;
	}
}

void ISFLoader::EndElement() {
	GetNext();
	switch (buffer[0]) {
		case '>': {
			if (endElement=="spells")
			{
				if (currentSpell!=NULL && currentSpell->spell_id!=-1) {
					loadspells->push_back(currentSpell);
					currentSpell=NULL;
				}
				GetNext();
				Start();
			} else {
				currentSpell->Set(currentElement, currentValue);
				GetNext();
				StartElement(); 
			}
			  break;
		 }
		default: endElement=endElement+buffer[0]; EndElement(); break;
	}
}

void ISFLoader::Value() {
	GetNext();
	switch (buffer[0]) {
		case '<': Element(); break;
		default: currentValue=currentValue+buffer[0]; Value(); break;
	}
}

void ISFLoader::GetNext() {
	if(pos<result) {
		pos++;
		buffer++;
	}
}

void ISFLoader::Match(char m) {
	if (buffer[0]==m) {
		GetNext();
	} else {
		error=true;
	}
}