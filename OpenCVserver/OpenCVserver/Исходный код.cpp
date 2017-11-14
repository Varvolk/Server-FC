#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

using namespace std;
#pragma comment(lib,"Ws2_32.lib")

SOCKET Connect;
SOCKET* Connections;
SOCKET Listen;
int client_count = 0;
WSAData data2;
WORD version = MAKEWORD(1, 0);
int res = WSAStartup(version, &data2);
char message[MAX_PATH];
struct addrinfo hints;
struct addrinfo *result;

DWORD WINAPI NetThread(LPVOID lpParam)
{
	SOCKET  sServerListen;
	sServerListen = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sServerListen == INVALID_SOCKET)
	{   
		return 0;   
	}
	/////////////////////////////////////////////////////////////////////
	Connections = (SOCKET*)calloc(64, sizeof(SOCKET)); //инициализация масива
	ZeroMemory(&hints, sizeof(hints)); //очистка структуры нулями

	hints.ai_family = AF_INET; //тип сокета 
	hints.ai_flags = AI_PASSIVE;
	hints.ai_socktype = SOCK_STREAM; // тип самого сокета
	hints.ai_protocol = IPPROTO_TCP;

	getaddrinfo(NULL, "6050", &hints, &result);// Передаем инфу о нас /// 6050 - порт, 
	Listen = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	bind(Listen, result->ai_addr, result->ai_addrlen);
	listen(Listen, 64);
	//старт работы сервера
	freeaddrinfo(result);
	/////////////////////////////////////////////////////////////////////

	char save[MAX_PATH];
	char save2[MAX_PATH];
	char name[23];
	char mes[3];

	//структура будет использоваться для отлова ip адресе присоеденяющихся клиентов.
	struct sockaddr_in peer;
	int size = sizeof(peer);
	while (1){
		if (Connect = accept(Listen, (struct sockaddr*)&peer, (socklen_t*)&size)) //accept возрвщвет тру, когда кто то подключится 
		{
			recv(Connect, mes, 3, 0);

			if ((mes[0] == 'r'))
			{
				char freand[40] = "freand";
				recv(Connect, save, MAX_PATH, 0);
				ofstream fout;
				int i = 0;
				for (; (save[i] != '+'); ++i)
				{
					name[i] = save[i];
				}
				name[i] = '.';
				name[i + 1] = 't';
				name[i + 2] = 'x';
				name[i + 3] = 't';
				name[i + 4] = '\0';
				if (!std::ifstream(name)){
					fout.open(name);
					fout << save;
					fout.close();
					strcat(freand, name);
					fout.open(freand);
					fout.close();
					mes[0] = 't'; mes[1] = 'r'; mes[2] = 'u';
					send(Connect, mes, 3, 0);
				}
				else{
					mes[0] = 'f'; mes[1] = 'a'; mes[2] = 'l';
					send(Connect, mes, 3, 0);
				}
			}
			if ((mes[0] == 'a') && (mes[1] == 'v') && (mes[2] == 't'))
			{
				recv(Connect, save, MAX_PATH, 0);
				ofstream fout;
				int i = 0;
				for (; save[i] != '+'; ++i)
				{
					name[i] = save[i];
				}
				name[i] = '.';
				name[i + 1] = 't';
				name[i + 2] = 'x';
				name[i + 3] = 't';
				name[i + 4] = '\0';

				mes[0] = 't'; mes[1] = 'r'; mes[2] = 'u';
				if (std::ifstream(name)){
					ifstream fin(name);
					fin >> save2;
					i++;
					for (;save[i]!='\0'; ++i)
					{
						if (save2[i]!=save[i]){
							mes[0] = 'f'; mes[1] = 'a'; mes[2] = 'l';
							break;
						}
					}
					fin.close();
					send(Connect, mes, 3, 0);
				}
				else{
					mes[0] = 'f'; mes[1] = 'a'; mes[2] = 'l';
					send(Connect, mes, 3, 0);
				}
			}
			//запрос об выводе информации об пользователе
			if (mes[0] == 's')
			{		
				recv(Connect, save, MAX_PATH, 0);
				ofstream fout;
				int i = 0;
				char sendIp[15]="";
				for (; save[i] != '\0'; ++i)
				{
					name[i] = save[i];
				}
				//проверяем онлайн ли пользователь
				ifstream fin2("online.txt");
				char online[260];
				fin2 >> online;
				fin2.close();
				name[i] = '\0';
				char *st = strstr(online, name);
				name[i] = '.';
				name[i + 1] = 't';
				name[i + 2] = 'x';
				name[i + 3] = 't';
				name[i + 4] = '\0';

				if (std::ifstream(name)){
					ifstream fin(name);
					fin >> save2;
					fin.close();
					int i=0,j;
					if (st != NULL) {
						strcat(save2, "+online+");
						for (j = 0; st[j] != '-'; ++j);
						for ( ++j; st[j] != '+'; ++i,++j) {
							sendIp[i] = st[j];
						}
						
						sendIp[++i] = '\0';
						strcat(save2, sendIp);
					}
					else strcat(save2, "+offline");
					mes[0] = 'f'; mes[1] = 'n'; mes[2] = 'd';

					send(Connect, mes, 3, 0);
					send(Connect, save2, MAX_PATH, 0);		
	

					}
				else{
					mes[0] = 'n'; mes[1] = 'f'; mes[2] = 'n';
					send(Connect, mes, 3, 0);
				}
			}
			//добавление пользователя в список друзей
			if ((mes[0] == 'a') && (mes[1] == 'd') && (mes[2] == 'd'))
			{
				char freand[40] = "freand";
				for (int i = 0; i < 30; ++i)
					save[i] = '\0';
				recv(Connect, save, MAX_PATH, 0);
				ofstream fout;
				int i = 0;
				for (; (save[i] != '+'); ++i)
				{
					name[i] = save[i];
				}
				name[i] = '.';
				name[i + 1] = 't';
				name[i + 2] = 'x';
				name[i + 3] = 't';
				name[i + 4] = '\0';
				strcat(freand, name);
				if (std::ifstream(freand)){
					char Nik[20];
					ofstream fout(freand, std::ios_base::app);
					i++;
					int z = 0;
					for (; save[i] != '\0'; ++i)
					{
						Nik[z] = save[i];
						++z;
					}
					Nik[z] = '\0';
					fout << Nik;
					fout.close();
					send(Connect, mes, 3, 0);
				}
				else{
					mes[0] = 'f'; mes[1] = 'a'; mes[2] = 'l';
					send(Connect, mes, 3, 0);
				}
			}
			//Добавление пользователя в список людей онлайн
			if ((mes[0] == 'z') && (mes[1] == 'd') && (mes[2] == 'd'))
			{
				
				for (int i = 0; i < 40; ++i)
					save[i] = '\0';
				recv(Connect, save, MAX_PATH, 0);
				*strstr(save, "+")='-';
				strcat(save, inet_ntoa(peer.sin_addr));
				strcat(save, "+");
				if (std::ifstream("online.txt")){
					char Nik[20];
					ofstream fout("online.txt", std::ios_base::app);
					cout << "online - "<<save << endl;
					fout << save;
					fout.close();
					send(Connect, mes, 3, 0);
				}
				else{
					mes[0] = 'f'; mes[1] = 'a'; mes[2] = 'l';
					send(Connect, mes, 3, 0);
				}
			}
			//паередача клиенту списка его друзей
			if (mes[0] == 'e')
			{
				char freand[40] = "freand";
				recv(Connect, save, MAX_PATH, 0);
				ofstream fout;
				int i = 0;
				for (; save[i] != '\0'; ++i)
				{
					name[i] = save[i];
				}
				name[i] = '.';
				name[i + 1] = 't';
				name[i + 2] = 'x';
				name[i + 3] = 't';
				name[i + 4] = '\0';
				strcat(freand, name);
				if (std::ifstream(freand)){
					ifstream fin(freand);
					fin >> save2;
					fin.close();
					mes[0] = 'f'; mes[1] = 'n'; mes[2] = 'd';
					send(Connect, mes, 3, 0);
					strcat(save2, "\0");
					send(Connect, save2, MAX_PATH, 0);
				}

				else{
					mes[0] = 'n'; mes[1] = 'f'; mes[2] = 'n';
					send(Connect, mes, 3, 0);
				}
			}
			client_count++;
		}
	}
	return 0;
}
int doit()
{
	char ac[80];
	if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
		cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << endl;
		return 1;
	}
	cout << "Host name is " << ac << "." << endl;

	struct hostent *phe = gethostbyname(ac);
	if (phe == 0) {
		cerr << "Yow! Bad host lookup." << endl;
		return 1;
	}

	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		cout << "Address " << i << ": " << inet_ntoa(addr) << endl;
	}

	return 0;
}

int main()
{
	HANDLE hNetThread;
	DWORD dwNetThreadId;
	hNetThread = CreateThread(NULL, 0, NetThread, 0, 0, &dwNetThreadId);

	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
		return 255;
	}
	int retval = doit();
	WSACleanup();
	int seconds = time(NULL);
	while (true){
		if (seconds + 120 < time(NULL)){
			seconds = time(NULL);
			std::ofstream("online.txt");
		}
	}
	return 0;
}