

#include <stdio.h>

#include <winsock2.h>



int main()

{

	WORD wVersionRequested;

	WSADATA wsaData;

	int wsaerr;

	// Using MAKEWORD macro, Winsock version request 2.2

	wVersionRequested = MAKEWORD(2, 2);



	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr != 0)

	{

		/* Tell the user that we could not find a usable WinSock DLL.*/

		printf("Server: The Winsock dll not found!\n");

		return 0;

	}

	else

	{

		printf("Server: The Winsock dll found!\n");

		printf("Server: The status: %s.\n", wsaData.szSystemStatus);

	}



	/* Confirm that the WinSock DLL supports 2.2.*/

	/* Note that if the DLL supports versions greater    */

	/* than 2.2 in addition to 2.2, it will still return */

	/* 2.2 in wVersion since that is the version we      */

	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)

	{

		/* Tell the user that we could not find a usable WinSock DLL.*/

		printf("Server: The dll do not support the Winsock version %u.%u!\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));

		WSACleanup();

		return 0;

	}

	else

	{

		printf("Server: The dll supports the Winsock version %u.%u!\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));

		printf("Server: The highest version this dll can support: %u.%u\n", LOBYTE(wsaData.wHighVersion), HIBYTE(wsaData.wHighVersion));

	}



	//////////Create a socket////////////////////////

	//Create a SOCKET object called m_socket.

	SOCKET m_socket;



	// Call the socket function and return its value to the m_socket variable.

	// For this application, use the Internet address family, streaming sockets, and the TCP/IP protocol.

	// using AF_INET family, TCP socket type and protocol of the AF_INET - IPv4

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);



	// Check for errors to ensure that the socket is a valid socket.

	if (m_socket == INVALID_SOCKET)

	{

		printf("Server: Error at socket(): %ld\n", WSAGetLastError());

		WSACleanup();

		return 0;

	}

	else

	{

		printf("Server: socket() is OK!\n");

	}



	////////////////bind//////////////////////////////

	// Create a sockaddr_in object and set its values.

	sockaddr_in service;



	// AF_INET is the Internet address family.

	service.sin_family = AF_INET;


	service.sin_addr.s_addr = inet_addr("192.168.1.80");

	// 55555 is the port number to which the socket will be bound…

	// Try other non-standard ports ( > 1024). Max = 2 power to 16 = 65536

	service.sin_port = htons(55555);



	// Call the bind function, passing the created socket and the sockaddr_in structure as parameters.

	// Check for general errors.

	if (bind(m_socket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)

	{

		printf("Server: bind() failed: %ld.\n", WSAGetLastError());

		closesocket(m_socket);

		return 0;

	}

	else

	{

		printf("Server: bind() is OK!\n");

	}



	// Call the listen function, passing the created socket and the maximum number of allowed

	// connections to accept as parameters. Check for general errors.

	if (listen(m_socket, 10) == SOCKET_ERROR)

		printf("Server: listen(): Error listening on socket %ld.\n", WSAGetLastError());

	else

	{

		printf("Server: listen() is OK, I'm waiting for connections...\n");

	}



	// Create a temporary SOCKET object called AcceptSocket for accepting connections.

	SOCKET AcceptSocket;



	// Create a continuous loop that checks for connections requests. If a connection

	// request occurs, call the accept function to handle the request.

	printf("Server: Waiting for a client to connect...\n");

	printf("***Hint: Server is ready...run your client program...***\n");

	// Do some verification...

	while (1)

	{

		AcceptSocket = SOCKET_ERROR;

		while (AcceptSocket == SOCKET_ERROR)

		{

			AcceptSocket = accept(m_socket, NULL, NULL);

		}



		// else, accept the connection...

		// When the client connection has been accepted, transfer control from the

		// temporary socket to the original socket and stop checking for new connections.

		printf("Server: Client Connected!\n");

		m_socket = AcceptSocket;

		break;

	}


	getchar();
	WSACleanup();

	return 0;

}