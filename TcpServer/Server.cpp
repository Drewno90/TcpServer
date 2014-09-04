#include <stdio.h>

#include <winsock2.h>



int main()

{

	// Initialize Winsock.

	WSADATA wsaData;

	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != NO_ERROR)

		printf("Server: Error at WSAStartup().\n");

	else

		printf("Server: WSAStartup() is OK.\n");



	// Create a SOCKET for listening for

	// incoming connection requests.

	SOCKET ListenSocket;

	ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (ListenSocket == INVALID_SOCKET)

	{

		printf("Server: Error at socket(): %ld\n", WSAGetLastError());

		WSACleanup();

		return 0;

	}

	else

		printf("Server: socket() is OK.\n");



	// The sockaddr_in structure specifies the address family,

	// IP address, and port for the socket that is being bound.

	sockaddr_in service;

	service.sin_family = AF_INET;

	service.sin_addr.s_addr = inet_addr("127.0.0.1");

	service.sin_port = htons(55555);



	if (bind(ListenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)

	{

		printf("Server: bind() failed.\n");

		closesocket(ListenSocket);

		return 0;

	}

	else

		printf("Server: bind() is OK.\n");



	// Listen for incoming connection requests on the created socket

	if (listen(ListenSocket, 10) == SOCKET_ERROR)

		printf("Server: Error listening on socket.\n");

	else

		printf("Server: listen() is OK.\n");



	// Create a SOCKET for accepting incoming requests.

	SOCKET AcceptSocket;

	printf("Server: Waiting for client to connect...\n");



	// Accept the connection if any...

	while (1)

	{

		AcceptSocket = SOCKET_ERROR;

		while (AcceptSocket == SOCKET_ERROR)

		{

			AcceptSocket = accept(ListenSocket, NULL, NULL);

		}

		printf("Server: accept() is OK.\n");

		printf("Server: Client connected...ready for communication.\n");

		ListenSocket = AcceptSocket;

		break;

	}


	getchar();
	WSACleanup();

	return 0;

}