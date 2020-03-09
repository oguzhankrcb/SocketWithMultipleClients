#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib")

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 0;
	}
	printf("\nWSA Baslatildi.");
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		return 1;
	}
	printf("Socket olusturuldu.\n");
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		return 2;
	}
	printf("Serverle baglanti olusturuldu.\n");
	while (1)
	{
		Sleep(1000);
		char message[256];
		if (recv(s, message, 256, 0) != 0)
		{
			char sentmess[256];
			printf("\nServerden gelen mesaj : %s", message);
			printf("\nGondermek istediginiz mesaji girin : ");
			gets_s(sentmess);
			send(s, sentmess, strlen(sentmess) + 1, 0);
		}
	}
	getchar();
	return 0;
}