//--------------------------------------------------------------------------------------
// IAFLoader.cpp
// This class loads up the game auras from the media folder to use in the game.
// This removes the need to continuatlly contact the game server for aura information.
//--------------------------------------------------------------------------------------

#include "dxut.h"
#ifndef AURA_H_
#define AURA_H_
#include "Aura.h"
#endif
#include "IAFLoader.h"



void IAFLoader::LoadIAF(std::vector <Aura*> *auras) {
	loadauras=auras;
	error=false;
	pos=0;
	fopen_s(&file, "media\\auras.IAF", "r" );
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

void IAFLoader::Start() {
	switch (buffer[0]) {
		case '<': CreateAura(); break;
		case ' ': GetNext(); Start(); break;
		case 10: GetNext(); Start(); break;
		default: break;
	}
}

void IAFLoader::CreateAura() {
	currentAura = new Aura();
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
			Match('a');
			Match('u');
			Match('r');
			Match('a');
			Match('>');
			StartElement();
			break;
				 }
	}

}

void IAFLoader::StartElement() {
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
void IAFLoader::Element() {
	GetNext();
	switch (buffer[0]) {
		case '>': Value(); break;
		case ' ': Element(); break;
		case '/': EndElement(); break;
		case 10: Element(); break;
		default: currentElement=currentElement+buffer[0]; Element(); break;
	}
}

void IAFLoader::EndElement() {
	GetNext();
	switch (buffer[0]) {
		case '>': {
			if (endElement=="aura")
			{
				if (currentAura!=NULL && currentAura->aura_id!=-1) {
					loadauras->push_back(currentAura);
					currentAura=NULL;
				}
				GetNext();
				Start();
			} else {
				currentAura->Set(currentElement, currentValue);
				GetNext();
				StartElement(); 
			}
			  break;
		 }
		default: endElement=endElement+buffer[0]; EndElement(); break;
	}
}

void IAFLoader::Value() {
	GetNext();
	switch (buffer[0]) {
		case '<': Element(); break;
		default: currentValue=currentValue+buffer[0]; Value(); break;
	}
}

void IAFLoader::GetNext() {
	if(pos<result) {
		pos++;
		buffer++;
	}
}

void IAFLoader::Match(char m) {
	if (buffer[0]==m) {
		GetNext();
	} else {
		error=true;
	}
}