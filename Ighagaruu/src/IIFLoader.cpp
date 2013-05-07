//--------------------------------------------------------------------------------------
// IIFLoader.cpp
// This class loads up the game items from the media folder to use in the game.
// This removes the need to continuatlly contact the game server for item information.
//--------------------------------------------------------------------------------------

#include "IIFLoader.h"

#include <stdio.h>

void IIFLoader::LoadIIF(std::vector <Item*> *items) {
	loaditems=items;
	pos=0;
	error=false;
	FILE* file = fopen("media\\items.IIF", "r" );
	if( file!= NULL )
	{
		fseek (file , 0 , SEEK_END);
		lSize = ftell (file);
		rewind (file);

		// allocate memory to contain the whole file:
		buffer = new char[lSize];
		if (buffer == NULL) { return; }

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

void IIFLoader::Start() {
	switch (buffer[0]) {
		case '<': CreateItem(); break;
		case ' ': GetNext(); Start(); break;
		case 10: GetNext(); Start(); break;
		default: break;
	}
}

void IIFLoader::CreateItem() {
	currentItem = new Item();
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
			Match('i');
			Match('t');
			Match('e');
			Match('m');
			Match('>');
			StartElement();
			break;
				 }
	}

}

void IIFLoader::StartElement() {
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
void IIFLoader::Element() {
	GetNext();
	switch (buffer[0]) {
		case '>': Value(); break;
		case ' ': Element(); break;
		case '/': EndElement(); break;
		case 10: Element(); break;
		default: currentElement=currentElement+buffer[0]; Element(); break;
	}
}

void IIFLoader::EndElement() {
	GetNext();
	switch (buffer[0]) {
		case '>': {
			if (endElement=="item")
			{
				if (currentItem!=NULL && currentItem->item_id!=-1) {
					loaditems->push_back(currentItem);
					currentItem=NULL;
				}
				GetNext();
				Start();
			} else {
				currentItem->Set(currentElement, currentValue);
				GetNext();
				StartElement(); 
			}
			  break;
		 }
		default: endElement=endElement+buffer[0]; EndElement(); break;
	}
}

void IIFLoader::Value() {
	GetNext();
	switch (buffer[0]) {
		case '<': Element(); break;
		default: currentValue=currentValue+buffer[0]; Value(); break;
	}
}

void IIFLoader::GetNext() {
	if(pos<result) {
		pos++;
		buffer++;
	}
}

void IIFLoader::Match(char m) {
	if (buffer[0]==m) {
		GetNext();
	} else {
		error=true;
	}
}
