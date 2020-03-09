#include<io.h>
#include<stdio.h>
#include <conio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib")
typedef struct socketstructure {
	SOCKET socket;
	int sira;
}socketstr, *lpsocketstr;

int generalcount = 0;

void CommunicateThread(void * param)
{
	lpsocketstr newsocketstrhere = (lpsocketstr)param;
	while (1)
	{
		Sleep(1000);
		char messagehere[256];
		char sentmessage[256];
		if (recv(newsocketstrhere->socket, messagehere, 256, 0) != 0)
		{
			printf("\n%d Nolu Clientten gelen mesaj : %s",newsocketstrhere->sira, messagehere);
			printf("\nGondermek istediginiz mesaji giriniz : ");
			gets_s(sentmessage);
			send(newsocketstrhere->socket, sentmessage, strlen(sentmessage) + 1, 0);
		}
	}
}

int main(int argc, char *argv[])
{
	SOCKET s,acceptsocket;
	WSADATA wsa;
	struct sockaddr_in server,client;
	int c;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}
	printf("WSA Baslatildi.\n");
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		printf("Socket olusturulamadi...\n");
		return 0;
	}
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
	printf("Socket olusturuldu.\n");
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		return -1;
	}
	printf("Socket bind edildi.\n");
	listen(s, SOMAXCONN);
	printf("Socket dinleniyor.\n");
	c = sizeof(struct sockaddr_in);
	while ((acceptsocket = accept(s, (struct sockaddr*)&client, &c)) != INVALID_SOCKET)
	{
		generalcount++;
		printf("\nYeni Baglanti kabul edildi.\nClient Sayisi : %d\n", generalcount);
		char message[256];
		sprintf_s(message, "Merhaba Client, Ben server.");
		socketstr * newstrsocket = (socketstr*)malloc(sizeof(socketstr));
		newstrsocket->socket = acceptsocket;
		newstrsocket->sira = generalcount;
		send(acceptsocket, message, strlen(message) + 1, 0);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CommunicateThread, (void*)newstrsocket, NULL, NULL);
	}
	getchar();
	return 0;
}