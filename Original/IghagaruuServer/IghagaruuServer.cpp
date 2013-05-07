#include "IghagaruuServer.h"
//------------------------------------------------------------------------
// IghagaruuServer.cpp
// This is the main server file, containing the game loops and loop for individual
// client connections.
//------------------------------------------------------------------------

// Structs for useful parts of the program.
struct FLOATARR {
	char b[8];
	float f;
	char* ptr;
	FLOATARR(float inf){
		f=inf;
		ptr=(char*)&f;
		b[0]=(char)*ptr;
		b[1]=(char)*(ptr+1);
		b[2]=(char)*(ptr+2);
		b[3]=(char)*(ptr+3);
		b[4]=(char)*(ptr+4);
		b[5]=(char)*(ptr+5);
		b[6]=(char)*(ptr+6);
		b[8]=(char)*(ptr+7);
	}
};
struct INTARR {
	char b[4];
	int i;
	char* ptr;
	INTARR(int in){
		i=in;
		ptr=(char*)&i;
		b[0]=(char)*ptr;
		b[1]=(char)*(ptr+1);
		b[2]=(char)*(ptr+2);
		b[3]=(char)*(ptr+3);
	}
};
struct ARRFLOAT {
	char b[8];
	float f;
	float* ptr;
	ARRFLOAT(char b1, char b2, char b3, char b4, char b5, char b6, char b7, char b8 ){
		b[0]=b1;
		b[1]=b2;
		b[2]=b3;
		b[3]=b4;
		b[4]=b5;
		b[5]=b6;
		b[6]=b7;
		b[7]=b8;
		ptr=(float*)&b;
		f=*ptr;
	}
};
struct ARRINT {
	char b[4];
	int i;
	int* ptr;
	ARRINT(char b1, char b2, char b3, char b4){
		b[0]=b1;
		b[1]=b2;
		b[2]=b3;
		b[3]=b4;
		ptr=(int*)&b;
		i=*ptr;
	}
};
std::vector <Character*> characters;
std::vector <Item*> items;
std::vector <Aura*> auras;
std::vector <Spell*> spells;
std::vector <AreaTrigger*> areatriggers;
float yadjust[8];

char *server = "localhost";
char *user = "root";
char *password = "";
char *database = "ighagaruu";

GameStats* gamestats;
std::string toUppercase(std::string mcString) {
 for (unsigned int i=0; i<mcString.length(); i++)
 {
  mcString[i]=toupper(mcString[i]);
 }
 return mcString;
}

// Main function
int main(int argc, char* argv[])
{

//////////////

//////////////////////
    WSADATA wsaData;
    SOCKET ListenSocket = INVALID_SOCKET,
           ClientSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    hints;
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
	
	gamestats = new GameStats(&characters);

	LoadSpells();
	//LoadItems();
	LoadAuras();
	LoadAreaTriggers();
	yadjust[0]=5.5f;
	yadjust[1]=5.5f;
	yadjust[2]=2.5f;
	yadjust[3]=5.5f;

	yadjust[4]=4.5f;
	yadjust[5]=4.5f;
	yadjust[6]=3.0f;
	yadjust[7]=4.5f;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
	unsigned long b=1;
	ioctlsocket(ListenSocket,FIONBIO,&b);
	DWORD nThreadID2;
	HANDLE thread = CreateThread(0, 0, ListenThread, (LPVOID)ListenSocket, 0, &nThreadID2);

	WaitForSingleObject(thread,INFINITE);
    // No longer need server socket
    closesocket(ListenSocket);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        return 1;
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();


	return 0;
}

// This is the listen thread, it constantly loops and waits for clients to connect
DWORD WINAPI ListenThread(LPVOID lpParam)
{
	SOCKET ListenSocket = (SOCKET)lpParam,
          ClientSocket = INVALID_SOCKET;
	while (true) {
    // Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket != INVALID_SOCKET) {
				DWORD nThreadID;
				CreateThread(0, 0, ServerThread, (LPVOID)ClientSocket, 0, &nThreadID);
		}
		Sleep(1);

	}
	return 0;
}

// Server thread is spawned for seach individual client connection.
DWORD WINAPI ServerThread(LPVOID lpParam)
{
	bool closeconn=false;
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	Account* acc =NULL;
	Character* chara=NULL;

	SOCKET ClientSocket = (SOCKET)lpParam;

	int iResult, iSendResult;
	int recvbuflen = DEFAULT_BUFLEN;

	do {
		char recvbuf[DEFAULT_BUFLEN];
		iResult = recv(ClientSocket, recvbuf, 512, 0);
		if (iResult > 0)
		{
			char firstByte = recvbuf[0];
			char sendbuff[512];
			int sendlength=0;
			bool needSend =true;
			sendbuff[0]=-1; 
			switch (firstByte)
			{
			case 20:
				{  //accountlogin
					int buffint=1;
					std::string username="";
					int i=0;
					while (recvbuf[buffint]!=32) {
						username=username+recvbuf[buffint];
						buffint++;
						i++;
					}
					std::string passwordu="";
					i=0;
					buffint++;
					while (recvbuf[buffint]!=32) {
						passwordu=passwordu+recvbuf[buffint];
						buffint++;
						i++;
					}

					conn = mysql_init(NULL);
					mysql_real_connect(conn, server,user, password, database, 0, NULL, 0);

					char query[512];
					sprintf_s(query,512,"SELECT userid, username, gamelevel, suspended FROM users WHERE username='%s' AND password='%s'",username.c_str(),passwordu.c_str());
					mysql_query(conn, query);

					res = mysql_store_result(conn);
					if (res->row_count==1) {

						row = mysql_fetch_row(res);
						if (atoi(row[4])!=1) {

							acc = new Account(atoi(row[0]),row[1],atoi(row[2]));

							mysql_free_result(res);

							char query[512];

							sprintf_s(query,512,"UPDATE users SET online='1', lastonline='NOW()' WHERE userid='%i'",acc->userid);

							mysql_query(conn, query);

							sendbuff[0]=20;
							sendbuff[1]=2;

						} else {

							sendbuff[0]=20;
							sendbuff[1]=1;
							closeconn=true;
						}
					} else {
						sendbuff[0]=20;
						sendbuff[1]=0;
						closeconn=true;
					}
					mysql_close(conn); 
					break;
				}
			case 21: //list chars
				{
					if (acc!=NULL) {
						sendbuff[0]=21;
						conn = mysql_init(NULL);
						mysql_real_connect(conn, server,user, password, database, 0, NULL, 0);

						char query[512];
						sprintf_s(query,512,"SELECT * FROM characters WHERE userid='%i'",acc->userid);

						mysql_query(conn, query);

						res = mysql_store_result(conn);
						int bufint=1;
						while(row = mysql_fetch_row(res)){
							INTARR c(atoi(row[0]));
							for (int i=0; i<4; i++) {
								sendbuff[bufint]=c.b[i];
								bufint++;
							}									//char* name=row[1];
							//for (unsigned int i=0; i<strlen(name); i++) {
							//	sendbuff[bufint]=name[i];
							//	bufint++;
							//}
							//bufint++;
							//sendbuff[bufint]=32;
						}
						INTARR e(-1);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=e.b[i];
							bufint++;
						}

						mysql_free_result(res);

						mysql_query(conn, query);

					} else {
						sendbuff[0]=23;
						sendbuff[1]=0;
						closeconn=true;
					}
					mysql_close(conn); 
					break;
				}
			case 22:
				{
					if (acc!=NULL) {
						ARRINT c(recvbuf[1],recvbuf[2],recvbuf[3],recvbuf[4]);
						conn = mysql_init(NULL);
						mysql_real_connect(conn, server,user, password, database, 0, NULL, 0);

						char query[512];
						sprintf_s(query,512,"SELECT character_id, character_name, character_faction, character_class FROM characters WHERE userid='%i' AND character_id='%i'",acc->userid,c.i);

						mysql_query(conn, query);

						res = mysql_store_result(conn);
						sendbuff[0]=22;
						int bufint=1;
						if (res->row_count==1) {
							row = mysql_fetch_row(res);
							INTARR c(atoi(row[0]));
							for (int i=0; i<4; i++) {
								sendbuff[bufint]=c.b[i];
								bufint++;
							}		
							char* name=row[1];
							for (unsigned int i=0; i<strlen(name); i++) {
								sendbuff[bufint]=name[i];
								bufint++;
							}
							sendbuff[bufint]=' ';
							bufint++;
							INTARR f(atoi(row[2]));
							for (int i=0; i<4; i++) {
								sendbuff[bufint]=f.b[i];
								bufint++;
							}	
							INTARR cl(atoi(row[3]));
							for (int i=0; i<4; i++) {
								sendbuff[bufint]=cl.b[i];
								bufint++;
							}	
							mysql_free_result(res);
						}

					} else {
						sendbuff[0]=23;
						sendbuff[1]=0;
						closeconn=true;
					}
					mysql_close(conn); 
					break;
				}
			case 24:
				{
					if (acc!=NULL) {
						bool notlogged=true;
						ARRINT c(recvbuf[1],recvbuf[2],recvbuf[3],recvbuf[4]);
						for (unsigned int i=0; i<characters.size(); i++) {
							if(characters.at(i)->id==c.i) {
								notlogged=false;
								closesocket(characters.at(i)->ClientSocket);
								characters.at(i)->ClientSocket=ClientSocket;
								chara=characters.at(i);
								chara->resetMaxHealth();
								sendbuff[0]=24;
								break;
							}
						}
						if (notlogged) {

						conn = mysql_init(NULL);
						mysql_real_connect(conn, server,user, password, database, 0, NULL, 0);

						char query[512];
						sprintf_s(query,512,"SELECT * FROM characters WHERE userid='%i' AND character_id='%i'",acc->userid,c.i);

						mysql_query(conn, query);

						res = mysql_store_result(conn);
						if (res->row_count==1) {
							chara=new Character(atoi(row[0]),row[1],(float)atof(row[3]),(float)atof(row[4]),(float)atof(row[5]),(float)atof(row[2]),atoi(row[6]),atoi(row[7]),atoi(row[10]),atoi(row[11]), atoi(row[12]), atoi(row[13]), atoi(row[14]), atoi(row[15]), atoi(row[16]), atoi(row[17]), atoi(row[18]), atoi(row[19]), atoi(row[20]), ClientSocket, gamestats);
							DWORD nThreadID;
							CreateThread(0, 0, UpdateThread, (LPVOID)chara, 0, &nThreadID);

							sendbuff[0]=24;
							//
							char query[512];
							sprintf_s(query,512,"SELECT * FROM character_spells WHERE character_id='%i'",chara->id);
	
							mysql_query(conn, query);

							res = mysql_store_result(conn);
							int k=0;
							while(row = mysql_fetch_row(res)){
								if (k<11) {
									bool found=false;
									for (unsigned int i=0; i< spells.size() && !found; i++) {
										if (spells.at(i)->spell_id==atoi(row[1])) {
											chara->inv.spells[k]=spells.at(i);
											found=true;
										}
									}
									k++;
								} else {
									break;
								}
							}

							///
							sprintf_s(query,512,"SELECT * FROM item i, character_items c WHERE c.character_id='%i' AND c.item_id=i.item_id",chara->id);
	
							mysql_query(conn, query);

							res = mysql_store_result(conn);
							while(row = mysql_fetch_row(res)){
								const Item* item = new Item(atoi(row[0]),atoi(row[2]),atoi(row[3]),0,atoi(row[5]),atoi(row[6]),atoi(row[7]),atoi(row[8]),atoi(row[9]),atoi(row[10]),atoi(row[11]),atoi(row[12]),atoi(row[13]),atoi(row[14]),atoi(row[15]),(atoi(row[16])? true : false),atoi(row[17]),atoi(row[18]),atoi(row[19]),(atoi(row[20])? true : false),atoi(row[21]),atoi(row[22]),atoi(row[23]),atoi(row[24]),atoi(row[25]),atoi(row[26]),atoi(row[28]),atoi(row[29]),atoi(row[30]),(atoi(row[31])? true : false),atoi(row[32]),atoi(row[33]),atoi(row[34]),atoi(row[35]),atoi(row[36]),(atoi(row[37])? true : false),atoi(row[38]),atoi(row[39]));
								int k=atoi(row[43]);
								if (k<6) {
									if (item->item_slot==k) {
										chara->inv.gear[k]=item;
									}
								} else if (k>=6 && k<12) {
									chara->inv.items[k]=item;
								}
							}
							chara->resetMaxHealth();
							characters.push_back(chara);
							mysql_close(conn); 
							}
						}
					} else {
						sendbuff[0]=23;
						sendbuff[1]=0;
						closeconn=true;
					}
					break;
				}
			default:
				{
					sendbuff[0]=23;
					sendbuff[1]=0;
					closeconn=true;
					break;
				}
			}
			if (needSend && sendbuff[0]!=-1) {
				iSendResult = send( ClientSocket, sendbuff, 512, 0 );
			}
		}
		if(closeconn) {
			return 0;
		}
		Sleep(1);
	} while (chara==NULL);
	// Receive until the peer shuts down the connection
	do {
		char recvbuf[DEFAULT_BUFLEN];
		int nLeft = recvbuflen;
		int iPos = 0;
		iResult = recv(ClientSocket, recvbuf, 512, 0);
		if (iResult > 0) {
			char firstByte = recvbuf[0];
			//Tokenizer* toke = new Tokenizer(recvbuf, ':');
			//std::string firstToken=toke->getNext();
			// Echo the buffer back to the sender
			//std::string test=Tokenizer::intToString(chara->stats.energy);
			char sendbuff[512];
			int sendlength=0;
			bool needSend =true;
			sendbuff[0]=-1; 
			switch (firstByte) {
	case 0:   {  //stats
		sendbuff[0]=0;  // return type 0 = stats

		///
		sendbuff[1]=0;  // poisition stats

		//char test=(char)chara->pos.orientation;
		int bufint=2;
		FLOATARR o(chara->pos.orientation);
		for (int i=0; i<8; i++) {
			sendbuff[bufint]=o.b[i];
			bufint++;
		}
		FLOATARR x(chara->pos.x);
		for (int i=0; i<8; i++) {
			sendbuff[bufint]=x.b[i];
			bufint++;
		}
		FLOATARR y(chara->pos.y);
		for (int i=0; i<8; i++) {
			sendbuff[bufint]=y.b[i];
			bufint++;
		}
		FLOATARR z(chara->pos.z);
		for (int i=0; i<8; i++) {
			sendbuff[bufint]=z.b[i];
			bufint++;
		}
		///
		sendlength=bufint;
		break;
			  }
	case 1:   {  //stats
		sendbuff[0]=0;  // return type 0 = stats

		///
		sendbuff[1]=1;  // health stats

		//char test=(char)chara->pos.orientation;
		int bufint=2;
		INTARR h(chara->stats.health);
		for (int i=0; i<4; i++) {
			sendbuff[bufint]=h.b[i];
			bufint++;
		}
		INTARR m(chara->stats.magika);
		for (int i=0; i<4; i++) {
			sendbuff[bufint]=m.b[i];
			bufint++;
		}
		INTARR e(chara->stats.energy);
		for (int i=0; i<4; i++) {
			sendbuff[bufint]=e.b[i];
			bufint++;
		}
		INTARR ha(chara->stats.hate);
		for (int i=0; i<4; i++) {
			sendbuff[bufint]=ha.b[i];
			bufint++;
		}
		INTARR fo(chara->stats.focus);
		for (int i=0; i<4; i++) {
			sendbuff[bufint]=fo.b[i];
			bufint++;
		}
		INTARR mh(chara->stats.maxhealth);
		for (int i=0; i<4; i++) {
			sendbuff[bufint]=mh.b[i];
			bufint++;
		}
		INTARR mm(chara->stats.maxmagika);
		for (int i=0; i<4; i++) {
			sendbuff[bufint]=mm.b[i];
			bufint++;
		}
		///
		sendlength=bufint;
		break;
			  }
	case 2:   {  //move
		char secondByte = recvbuf[1];
		switch (secondByte)
		{
		case 0: {
			ARRFLOAT o(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5],recvbuf[6],recvbuf[7],recvbuf[8],recvbuf[9]);
			chara->pos.orientation=o.f;
			ARRFLOAT x(recvbuf[10],recvbuf[11],recvbuf[12],recvbuf[13],recvbuf[14],recvbuf[15],recvbuf[16],recvbuf[17]);
			ARRFLOAT y(recvbuf[18],recvbuf[19],recvbuf[20],recvbuf[21],recvbuf[22],recvbuf[23],recvbuf[24],recvbuf[25]);
			ARRFLOAT z(recvbuf[26],recvbuf[27],recvbuf[28],recvbuf[29],recvbuf[30],recvbuf[31],recvbuf[32],recvbuf[33]);
			if (chara->pos.x!=x.f) { chara->info.casting=NULL; chara->stats.focus=0;}
			if (chara->pos.y!=y.f) { chara->info.casting=NULL; chara->stats.focus=0;}
			if (chara->pos.z!=z.f) { chara->info.casting=NULL; chara->stats.focus=0;}
			chara->pos.x=x.f;
			chara->pos.y=y.f;
			chara->pos.z=z.f;
			for (unsigned int i=0; i<areatriggers.size(); i++) {
				if (areatriggers.at(i)->areatrigger_enabled && (areatriggers.at(i)->areatrigger_faction==-1 || areatriggers.at(i)->areatrigger_faction==chara->info.faction) && areatriggers.at(i)->WithinTrigger(chara)) {
					areatriggers.at(i)->Action(chara);
				}
			}
			break;
				}
		case 1: {
			ARRINT h(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
			chara->stats.health=h.i;
			ARRINT m(recvbuf[6],recvbuf[7],recvbuf[8],recvbuf[9]);
			chara->stats.magika=m.i;
			ARRINT e(recvbuf[10],recvbuf[11],recvbuf[12],recvbuf[13]);
			chara->stats.energy=e.i;
			ARRINT ha(recvbuf[14],recvbuf[15],recvbuf[16],recvbuf[17]);
			chara->stats.hate=ha.i;
			ARRINT f(recvbuf[18],recvbuf[19],recvbuf[20],recvbuf[21]);
			chara->stats.focus=f.i;
			break;
				}
		}
		needSend=false;
		break;
			  }
	case 3:   {  //listallchars
		sendbuff[0]=1;  // charlist
		sendlength=1;
		int bufint=1;
		for (unsigned int j=0; j<characters.size();j++) {
			if (characters.at(j)->id!=chara->id) {
				INTARR c(characters.at(j)->id);
				for (int i=0; i<4; i++) {
					sendbuff[bufint]=c.b[i];
					bufint++;
				}
				sendlength=sendlength+4;
			}
		}
		INTARR c(-1);
		for (int i=0; i<4; i++) {
			sendbuff[bufint]=c.b[i];
			bufint++;
		}
		///
		break;
			  }
	case 4:   {  //listachar
		char secondByte = recvbuf[1];
		switch (secondByte)
		{
		case 0: {
			ARRINT c(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
			bool found=false;
			if (c.i!=chara->id) {
				for (unsigned int j=0; j<characters.size() && !found;j++) {
					if (characters.at(j)->id==c.i) {
						found=true;
						sendbuff[0]=2;  // char info
						sendbuff[1]=0;  // poisition stats

						int bufint=2;
						INTARR c(characters.at(j)->id);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=c.b[i];
							bufint++;
						}
						FLOATARR o(characters.at(j)->pos.orientation);
						for (int i=0; i<8; i++) {
							sendbuff[bufint]=o.b[i];
							bufint++;
						}
						FLOATARR x(characters.at(j)->pos.x);
						for (int i=0; i<8; i++) {
							sendbuff[bufint]=x.b[i];
							bufint++;
						}
						FLOATARR y(characters.at(j)->pos.y);
						for (int i=0; i<8; i++) {
							sendbuff[bufint]=y.b[i];
							bufint++;
						}
						FLOATARR z(characters.at(j)->pos.z);
						for (int i=0; i<8; i++) {
							sendbuff[bufint]=z.b[i];
							bufint++;
						}
					}
				}
			} else {
				needSend=false;
			}
			break;
				}
		case 1: {
			ARRINT c(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
			bool found=false;
			if (c.i!=chara->id) {
				for (unsigned int j=0; j<characters.size() && !found;j++) {
					if (characters.at(j)->id==c.i) {
						found=true;
						sendbuff[0]=2;  // char info
						sendbuff[1]=1;  // health stats

						int bufint=2;
						INTARR c(characters.at(j)->id);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=c.b[i];
							bufint++;
						}
						INTARR h(characters.at(j)->stats.health);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=h.b[i];
							bufint++;
						}
						INTARR m(characters.at(j)->stats.magika);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=m.b[i];
							bufint++;
						}
						INTARR e(characters.at(j)->stats.energy);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=e.b[i];
							bufint++;
						}
						INTARR ha(characters.at(j)->stats.hate);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=ha.b[i];
							bufint++;
						}
						INTARR fo(characters.at(j)->stats.focus);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=fo.b[i];
							bufint++;
						}
						INTARR mh(characters.at(j)->stats.maxhealth);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=mh.b[i];
							bufint++;
						}
						INTARR mm(characters.at(j)->stats.maxmagika);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=mm.b[i];
							bufint++;
						}
						sendlength=bufint;
					}
				}
			} else {
				needSend=false;
			}
			break;
				}
		case 2: {
			ARRINT c(recvbuf[2],recvbuf[3],recvbuf[4],recvbuf[5]);
			bool found=false;
			if (c.i!=chara->id) {
				for (unsigned int j=0; j<characters.size() && !found;j++) {
					if (characters.at(j)->id==c.i) {
						found=true;
						sendbuff[0]=2;  // char info
						sendbuff[1]=2;  // info

						int bufint=2;
						INTARR c(characters.at(j)->id);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=c.b[i];
							bufint++;
						}
						for (unsigned int i=0; i<characters.at(j)->info.name.length(); i++) {
							sendbuff[bufint]=characters.at(j)->info.name[i];
							bufint++;
						}
						sendbuff[bufint]=' ';
						bufint++;
						INTARR m(characters.at(j)->info.model);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=m.b[i];
							bufint++;
						}
						INTARR cl(characters.at(j)->info.classs);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=cl.b[i];
							bufint++;
						}
						INTARR fa(characters.at(j)->info.faction);
						for (int i=0; i<4; i++) {
							sendbuff[bufint]=fa.b[i];
							bufint++;
						}
					}
				}
			} else {
				needSend=false;
			}
			break;
				}
		default: needSend=false; break;
		}
		break;
			  }
	case 5:   {  //target
		ARRINT c(recvbuf[1],recvbuf[2],recvbuf[3],recvbuf[4]);
		sendbuff[0]=5;  //current target
		sendlength=1;
		int bufint=1;
		bool targeted=false;
		for (unsigned int j=0; j<characters.size() &&!targeted;j++) {
			if (c.i==characters.at(j)->id) {
				targeted=true;
				chara->target=characters.at(j);
			}
		}
		if (targeted) {
			INTARR t(chara->target->id);
			for (int i=0; i<4; i++) {
				sendbuff[bufint]=t.b[i];
				bufint++;
			}
		} else {
			needSend=false;
		}
		break;
			  }
	case 6:   {  //cast
		ARRINT s(recvbuf[1],recvbuf[2],recvbuf[3],recvbuf[4]);
		if (chara->target!=NULL) {
			chara->inv.spells[s.i]->Cast(chara, chara->target,false, false);
			needSend=false;
		} else {
			needSend=false;
		}
		break;
			  }
	case 7:   {  //use
		ARRINT s(recvbuf[1],recvbuf[2],recvbuf[3],recvbuf[4]);
		needSend=false;
		break;
			  }
	case 8:
		{  
			needSend=false;
			int buffint=1;
			std::string channel="";
			int i=0;
			while (recvbuf[buffint]!=1) {
				channel=channel+recvbuf[buffint];
				buffint++;
				i++;
			}
			std::string message="";
			i=0;
			buffint++;
			while (recvbuf[buffint]!=1) {
				message=message+recvbuf[buffint];
				buffint++;
				i++;
			}

			if (channel[0]=='#') { //channel
				for (unsigned int i=0; i<characters.size(); i++) {
					char sendbuf[512];
					std::string name=chara->info.name;
					int bufint=2;
					sendbuf[0]=8;
					sendbuf[1]=0;
					for (unsigned int j=0; j<name.length(); j++) {
						sendbuf[bufint]=name[j];
						bufint++;
					}
					sendbuf[bufint]=(char)1;
					bufint++;
					for (unsigned int j=0; j<message.length(); j++) {
						sendbuf[bufint]=message[j];
						bufint++;
					}
					sendbuf[bufint]=(char)1;
					iSendResult = send( characters.at(i)->ClientSocket, sendbuf, 512, 0 );

				}
			} else { //user
				bool found=false;
				std::string destname = toUppercase(channel);
				for (unsigned int i=0; i<characters.size()&&(!found); i++) {
					std::string checkname =toUppercase(characters.at(i)->info.name);
					if (checkname==destname) {
						found=true;
						char sendbuf[512];
						std::string name=chara->info.name;
						int bufint=2;
						sendbuf[0]=8;
						sendbuf[1]=1;
						for (unsigned int j=0; j<name.length(); j++) {
							sendbuf[bufint]=name[j];
							bufint++;
						}
						sendbuf[bufint]=(char)1;
						bufint++;
						for (unsigned int j=0; j<message.length(); j++) {
							sendbuf[bufint]=message[j];
							bufint++;
						}
						sendbuf[bufint]=(char)1;
						iSendResult = send( characters.at(i)->ClientSocket, sendbuf, 512, 0 );

					}
				}
			}
			break;
		}
	case 9:   {  //list spells
		sendbuff[0]=9;  // charlist
		int bufint=1;
		for (unsigned int j=0; j<11;j++) {
			if (chara->inv.spells[j]!=NULL) {
				INTARR c(chara->inv.spells[j]->spell_id);
				for (int i=0; i<4; i++) {
					sendbuff[bufint]=c.b[i];
					bufint++;
				}
				sendlength=sendlength+4;
			}
		}
		INTARR c(-1);
		for (int i=0; i<4; i++) {
			sendbuff[bufint]=c.b[i];
			bufint++;
		}
		///
		break;
			  }
	case 10:   {  //list items
		sendbuff[0]=10; 
		int bufint=3;
		if (recvbuf[1]==0) {
			sendbuff[1]=0;
			if (recvbuf[2]>=0 &&  recvbuf[2]<6) {
				sendbuff[2]=recvbuf[2];
				if (chara->inv.gear[recvbuf[2]]!=NULL) {
					INTARR g(chara->inv.gear[recvbuf[2]]->item_id);
					for (int i=0; i<4; i++) {
						sendbuff[bufint]=g.b[i];
						bufint++;
					}
				} else {
					INTARR g(-1);
					for (int i=0; i<4; i++) {
						sendbuff[bufint]=g.b[i];
						bufint++;
					}
				}
			} else {
				needSend=false;
			}
		} else if (recvbuf[1]==1) {
			sendbuff[1]=1;
			if (recvbuf[2]>=0 &&  recvbuf[2]<6) {
				sendbuff[2]=recvbuf[2];
				if (chara->inv.items[recvbuf[2]]!=NULL) {
					INTARR g(chara->inv.items[recvbuf[2]]->item_id);
					for (int i=0; i<4; i++) {
						sendbuff[bufint]=g.b[i];
						bufint++;
					}
				} else {
					INTARR g(-1);
					for (int i=0; i<4; i++) {
						sendbuff[bufint]=g.b[i];
						bufint++;
					}
				}
			} else {
				needSend=false;
			}
		} else {
			needSend=false;
		}
		///
		break;
			   }
	case 11:
		{  //autoattack
			if (chara->target!=NULL) {
				if (chara->info.autoAttack) {chara->info.autoAttack=false;} else {chara->info.autoAttack=true;}
				needSend=false;
			} else {
				needSend=false;
			}
			break;
		}
	case 12:
		{  //score
			sendbuff[0]=12;
			int bufint=1;
			INTARR h(gamestats->redteam);
			for (int i=0; i<4; i++) {
				sendbuff[bufint]=h.b[i];
				bufint++;
			}
			INTARR c(gamestats->blueteam);
			for (int i=0; i<4; i++) {
				sendbuff[bufint]=c.b[i];
				bufint++;
			}
			break;
		}
	case 15:
		{  //flagpos
			sendbuff[0]=15;  // flag info

			int bufint=1;
			float fx=0;
			float fy=0;
			float fz=0;
			float fo=0;

			if (gamestats->flagcarrier!=NULL) {
				fx=gamestats->flagcarrier->pos.x;
				fy=gamestats->flagcarrier->pos.y-yadjust[gamestats->flagcarrier->info.classs+(gamestats->flagcarrier->info.faction*4)];
				fz=gamestats->flagcarrier->pos.z;
				fo=gamestats->flagcarrier->pos.orientation;
			}

			FLOATARR o(fo);
			for (int i=0; i<8; i++) {
				sendbuff[bufint]=o.b[i];
				bufint++;
			}
			FLOATARR x(fx);
			for (int i=0; i<8; i++) {
				sendbuff[bufint]=x.b[i];
				bufint++;
			}
			FLOATARR y(fy);
			for (int i=0; i<8; i++) {
				sendbuff[bufint]=y.b[i];
				bufint++;
			}
			FLOATARR z(fz);
			for (int i=0; i<8; i++) {
				sendbuff[bufint]=z.b[i];
				bufint++;
			}
			break;
		}
			}
			if (needSend && sendbuff[0]!=-1) {
				iSendResult = send( ClientSocket, sendbuff, 512, 0 );
			}
		}


		Sleep(1);

	} while (true);
	return 0;
}

// Update thread, is a tick thread that will update certain attributes every second.
DWORD WINAPI UpdateThread(LPVOID lpParam)
{
	Character* upchara = (Character*)lpParam;
	int second=0;
	while (true) {
		if (second==1000) {
			second=0;
			upchara->Update();
			for (unsigned int i=0; i<areatriggers.size(); i++) {
				areatriggers.at(i)->Cooldown();
			}
		} else {
			second=second+50;
		}
		upchara->cast();
		Sleep(50);
	}
	return 0;
}


//// The following functions load up the information from the database.
void LoadSpells() {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	Account* acc =NULL;
	Character* chara=NULL;

	conn = mysql_init(NULL);
	if (mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)==NULL) {
		printf("Error Connecting to SQL Server");
		exit(-1);
	}
	mysql_query(conn, "SELECT * FROM spells");

	res = mysql_store_result(conn);
	while(row = mysql_fetch_row(res)){
		Spell* spell = new Spell(atoi(row[0]),atoi(row[2]),atoi(row[3]),atoi(row[4]),atoi(row[5]),atoi(row[6]),atoi(row[7]),atoi(row[8]),atoi(row[9]),atoi(row[10]),atoi(row[11]),atoi(row[12]),atoi(row[13]),atoi(row[14]),atoi(row[15]),atoi(row[16]),atoi(row[17]),atoi(row[18]),atoi(row[19]),(atoi(row[20])? true : false),atoi(row[21]),atoi(row[22]),atoi(row[23]),(atoi(row[24])? true : false),atoi(row[25]),atoi(row[26]),(atoi(row[27])? true : false),(atoi(row[28])? true : false),(atoi(row[29])? true : false),(atoi(row[30])? true : false),(atoi(row[31])? true : false),(atoi(row[32])? true : false),atoi(row[33]),(atoi(row[35])? true : false), &auras);
		spells.push_back(spell);
	}
	mysql_free_result(res);

	mysql_close(conn); 
}

void LoadItems() {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	Account* acc =NULL;
	Character* chara=NULL;

	conn = mysql_init(NULL);
	if (mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)==NULL) {
		printf("Error Connecting to SQL Server");
		exit(-1);
	}

	mysql_query(conn, "SELECT * FROM items");

	res = mysql_store_result(conn);
	while(row = mysql_fetch_row(res)){
		Item* item = new Item(atoi(row[0]),atoi(row[2]),atoi(row[3]),atoi(row[4]),atoi(row[5]),atoi(row[6]),atoi(row[7]),atoi(row[8]),atoi(row[9]),atoi(row[10]),atoi(row[11]),atoi(row[12]),atoi(row[13]),atoi(row[14]),atoi(row[15]),(atoi(row[16])? true : false),atoi(row[17]),atoi(row[18]),atoi(row[19]),(atoi(row[20])? true : false),atoi(row[21]),atoi(row[22]),atoi(row[23]),atoi(row[24]),atoi(row[25]),atoi(row[26]),atoi(row[28]),atoi(row[29]),atoi(row[30]),(atoi(row[31])? true : false),atoi(row[32]),atoi(row[33]),atoi(row[34]),atoi(row[35]),atoi(row[36]),(atoi(row[37])? true : false),atoi(row[38]),atoi(row[39]));
		items.push_back(item);
	}
	mysql_free_result(res);

	mysql_close(conn); 
}

void LoadAuras() {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	Account* acc =NULL;
	Character* chara=NULL;

	conn = mysql_init(NULL);
	if (mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)==NULL) {
		printf("Error Connecting to SQL Server");
		exit(-1);
	}

	mysql_query(conn, "SELECT * FROM auras");

	res = mysql_store_result(conn);
	while(row = mysql_fetch_row(res)){
		Aura* aura = new Aura(atoi(row[0]),atoi(row[1]),atoi(row[2]),atoi(row[3]),atoi(row[4]),atoi(row[5]),atoi(row[6]),atoi(row[7]),atoi(row[8]),atoi(row[9]),atoi(row[10]),atoi(row[11]),atoi(row[12]),atoi(row[13]),atoi(row[14]),atoi(row[17]),atoi(row[18]),atoi(row[19]),atoi(row[20]),atoi(row[21]),atoi(row[22]),atoi(row[23]),atoi(row[24]),atoi(row[25]),(atoi(row[26])? true : false));
		auras.push_back(aura);
	}
	mysql_free_result(res);

	mysql_close(conn); 
}

void LoadAreaTriggers() {
	MYSQL *conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	Account* acc =NULL;
	Character* chara=NULL;

	conn = mysql_init(NULL);
	if (mysql_real_connect(conn, server,user, password, database, 0, NULL, 0)==NULL) {
		printf("Error Connecting to SQL Server");
		exit(-1);
	}

	mysql_query(conn, "SELECT * FROM areatriggers");

	res = mysql_store_result(conn);
	while(row = mysql_fetch_row(res)){
		AreaTrigger* areatrigger = new AreaTrigger(gamestats, &auras, &areatriggers);
		areatrigger->areatrigger_id=atoi(row[0]);
		areatrigger->areatrigger_giveaura=atoi(row[1]);
		areatrigger->areatrigger_needaura=atoi(row[2]);
		areatrigger->areatrigger_addpoints=(atoi(row[3])? true : false);
		areatrigger->box.u.x=(float)atof(row[4]);
		areatrigger->box.u.y=(float)atof(row[5]);
		areatrigger->box.u.z=(float)atof(row[6]);
		areatrigger->box.l.x=(float)atof(row[7]);
		areatrigger->box.l.y=(float)atof(row[8]);
		areatrigger->box.l.z=(float)atof(row[9]);
		areatrigger->areatrigger_faction=atoi(row[10]);
		areatrigger->areatrigger_enabled=(atoi(row[11])? true : false);
		areatrigger->areatrigger_disableontouch=(atoi(row[12])? true : false);
		areatrigger->areatrigger_teleport=(atoi(row[13])? true : false);
		areatrigger->areatrigger_telex=(float)atof(row[14]);
		areatrigger->areatrigger_teley=(float)atof(row[15]);
		areatrigger->areatrigger_telez=(float)atof(row[16]);
		areatrigger->areatrigger_enabletrigger=atoi(row[17]);
		areatrigger->areatrigger_flag=(atoi(row[18])? true : false);
		areatriggers.push_back(areatrigger);
	}
	mysql_free_result(res);

	mysql_close(conn); 

}