#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>
#include<vector>

using namespace std;

#pragma comment(lib,"ws2_32.lib")//this line tell the compiler to use windows socket

vector<SOCKET>Clients;

/*
steps-
1. initialize the winsock library
2. create the socket(listenSocket)
3. get ip and port
4. bind the port with the socket
5. listen on the socket
6. accept(clientSocket)
7. receive and send
8. clean up the win sock
*/

bool initialize() {
	WSADATA data;//WSADATA is a structure that receives details about the Windows Sockets implementation

	return WSAStartup(MAKEWORD(2, 2), &data) == 0;//WSAStartup(MAKEWORD(2, 2), &data): Initializes Winsock with version 2.2.
}

void InteractWithClient(SOCKET clientSocket, vector<SOCKET>&Clients) {
	//send and recieve

	cout << "Client connected\n";
	char buffer[4096];
	while (true) {
		int dataRecieved = recv(clientSocket, buffer, sizeof(buffer), 0);
		//Then we start receiving the data from the client.We can specify the required buffer size so that it has enough space to receive the data sent the the client.
		//clientSocket: The socket connected to the client.
		//buffer: The buffer where received data is stored.
		//sizeof(buffer): Maximum bytes to receive.
		//0: Default flags.

		if (dataRecieved <= 0) {
			cout << "Client disconnected\n";
			break;
		}

		string message(buffer, dataRecieved);
		cout << "Message from client = " << message << "\n";

		//broadcasting messages to other client sockets stored in the vector Clients
		for (auto i : Clients) {
			if (i != clientSocket) {
				send(i, message.c_str(), message.length(), 0);//broadcating to other clients except itself
			}
		}
	}

	auto it = find(Clients.begin(), Clients.end(), clientSocket);
	if (it != Clients.end()) {
		Clients.erase(it);
	}

	closesocket(clientSocket);//closesocket(clientSocket) : Closes the client socket.
}

int main() {

	//initialize the winsock library

	if (!initialize()) {
		cout << "Initialization failed\n";
	}

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	//AF_INET: Specifies IPv4.
	//There are mainly two types of sockets-stream socket for TCP and datagram socket for UDP
	//SOCK_STREAM: Specifies TCP (stream-based communication).
	//0: Default protocol(TCP in this case).

	if (listenSocket == INVALID_SOCKET) {
		cout << "Socket creation failed\n";
		return 1;
	}

	//create the socket(address structure)

	int port = 3000;
	sockaddr_in serverAddress;//sockaddr_in: It is the data type that is used to store the address of the socket.
	serverAddress.sin_family = AF_INET;//serverAddress.sin_family = AF_INET: Specifies IPv4.
	serverAddress.sin_port = htons(port);//htons(): This function is used to convert the unsigned int from machine byte order to network byte order.


	//convert the ip address(IPV4) to binary format

	if (InetPton(AF_INET, _T("0.0.0.0"), &serverAddress.sin_addr) != 1)
		//InetPton(AF_INET, _T("0.0.0.0"), &serverAddress.sin_addr): Converts the IP address "0.0.0.0" (which means "listen on all interfaces") into binary format and stores it in serverAddress.sin_addr.
		//If InetPton fails, it returns - 1 or 0, indicating an error.
	{
		cout << "converting address to binary failed\n";
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//bind

	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) 
		//Associates the socket with the specified IP address (0.0.0.0) and port (3000).
		//If bind() fails, it returns SOCKET_ERROR.
	{
		cout << "Binding failed\n";
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//listen

	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) 
		//listen(listenSocket, SOMAXCONN) : Marks the socket as a listening socket to accept incoming connections.
		//SOMAXCONN: A system-defined constant specifying the maximum number of pending connections.
		//If listen() fails, it returns SOCKET_ERROR.
	{
		cout << "Listening failed\n";
	}

	cout << "Listening on port " << port<<"\n";

	//Accept

	while (true) {
		SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);
		//accept(listenSocket, nullptr, nullptr): Waits for an incoming connection and returns a new socket (clientSocket) for communication with the client.
		if (clientSocket == INVALID_SOCKET) {
			cout << "Invalid client socket";
		}

		Clients.push_back(clientSocket);
		thread T1(InteractWithClient, clientSocket, std::ref(Clients));
		T1.detach();
	}

	closesocket(listenSocket);//closesocket(listenSocket): Closes the listening socket.

	//clean up the win sock

	WSACleanup();//WSACleanup(): Cleans up resources used by Winsock.

	return 0;
}