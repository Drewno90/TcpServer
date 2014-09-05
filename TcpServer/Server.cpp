#include <cstdio>
#include <iostream>
#include <winsock2.h>
#include <iomanip>
#include <cmath>

using namespace std;

const double eps = 1e-12;

bool ludist(int n, double ** A)
{
	int i, j, k;

	for (k = 0; k < n - 1; k++)
	{
		if (fabs(A[k][k]) < eps) return false;

		for (i = k + 1; i < n; i++)
			A[i][k] /= A[k][k];

		for (i = k + 1; i < n; i++)
		for (j = k + 1; j < n; j++)
			A[i][j] -= A[i][k] * A[k][j];
	}

	return true;
}

bool lusolve(int k, int n, double ** A, double ** X)
{
	int    i, j;
	double s;

	for (i = 1; i < n; i++)
	{
		s = 0;

		for (j = 0; j < i; j++) s += A[i][j] * X[j][k];

		X[i][k] -= s;
	}

	if (fabs(A[n - 1][n - 1]) < eps) return false;

	X[n - 1][k] /= A[n - 1][n - 1];

	for (i = n - 2; i >= 0; i--)
	{
		s = 0;

		for (j = i + 1; j < n; j++) s += A[i][j] * X[j][k];

		if (fabs(A[i][i]) < eps) return false;

		X[i][k] = (X[i][k] - s) / A[i][i];
	}

	return true;
}

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
		cout<<"Server: The Winsock dll not found!"<<endl;
		return 0;
	}
	else
	{
		cout<<"Server: The Winsock dll found!"<<endl;
		cout<<"Server: The status: "<< wsaData.szSystemStatus<<endl;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		/* Tell the user that we could not find a usable WinSock DLL.*/
		cout<<"Server: The dll do not support the Winsock version "<< LOBYTE(wsaData.wVersion) << HIBYTE(wsaData.wVersion)<< endl;
		WSACleanup();
		return 0;
	}
	else
	{
		cout << "Server: The dll supports the Winsock version " << LOBYTE(wsaData.wVersion) << HIBYTE(wsaData.wVersion) << endl;
		cout<<"Server: The highest version this dll can support: "<< LOBYTE(wsaData.wHighVersion)<< HIBYTE(wsaData.wHighVersion)<<endl;
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
		cout<<"Server: Error at socket():  "<< WSAGetLastError()<<endl;
		WSACleanup();
		return 0;
	}
	else
	{
		cout<<"Server: socket() is OK!"<<endl;
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
		cout<<"Server: bind() failed:  "<< WSAGetLastError()<<endl;
		closesocket(m_socket);
		return 0;
	}
	else
	{
		cout<<"Server: bind() is OK!"<<endl;
	}

	// Call the listen function, passing the created socket and the maximum number of allowed
	// connections to accept as parameters. Check for general errors.

	if (listen(m_socket, 10) == SOCKET_ERROR)
		cout<<"Server: listen(): Error listening on socket  "<< WSAGetLastError()<<endl;
	else
	{
		cout<<"Server: listen() is OK, I'm waiting for connections..."<<endl;
	}

	// Create a temporary SOCKET object called AcceptSocket for accepting connections.

	SOCKET AcceptSocket;

	// Create a continuous loop that checks for connections requests. If a connection
	// request occurs, call the accept function to handle the request.

	cout<<"Server: Waiting for a client to connect..."<<endl;

	cout<<"***Hint: Server is ready...run your client program...***"<<endl;

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

		cout<<"Server: Client Connected!"<<endl;
		m_socket = AcceptSocket;
		break;
	}
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char sendbuf[200] = "This string is a test data from server";

	// initialize to empty data...

	char recvbuf[200] = "";

	// Send some test string to client...

	cout<<"Server: Sending some test data to client..."<<endl;

	bytesSent = send(m_socket, sendbuf, strlen(sendbuf), 0);

	if (bytesSent == SOCKET_ERROR)
		cout<<"Server: send() error  "<< WSAGetLastError()<<endl;
	else
	{
		cout<<"Server: send() is OK."<<endl;
		cout<<"Server: Bytes Sent:  "<< bytesSent<<endl;
	}

	// Receives some test string from client...and client
	// must send something lol...

	bytesRecv = recv(m_socket, recvbuf, 200, 0);
		
	if (bytesRecv == SOCKET_ERROR)
		cout<<"Server: recv() error  "<< WSAGetLastError();
	else
	{
		cout<<"Server: recv() is OK."<<endl;
		int n = strlen(recvbuf);
		int k = (int)sqrt(n);
		int p=0;
		bool ok;
		double **A = new double *[k];
		double **B = new double *[k];
		for (int i = 0; i < k; i++){
			A[i] = new double[k];
			B[i] = new double [k];
		}
		for (int i = 0; i < k; i++)
		for(int j=0;j<k;j++){
			A[i][j] = (double)recvbuf[p]; 
			p++;
			}
		if (ludist(k, A)){
			for (int i = 0; i < k; i++)
			{
				for (int j = 0; j < k; j++) B[i][j] = 0;
				B[i][i] = 1;
			}
			ok = true;
			for (int i = 0; i < k; i++)
			if (!lusolve(i, k, A, B))
			{
				ok = false;
				break;
			}
		}
		else ok = false;
		if (ok)
		{
			for (int i = 0; i < k; i++)
			{
				for (int j = 0; j < k; j++)
					cout << setw(10) << B[i][j] << " ";
				cout << endl;
			}
		}
		else cout << "DZIELNIK ZERO\n";
		
		cout<<"Server: Bytes received:  "<< bytesRecv<<endl;
	}

	getchar();
	WSACleanup();

	return 0;
}