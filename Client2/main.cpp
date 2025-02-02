#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include<thread>
#include<string>

/*
steps-
1. initialize winsock
2. create socket
3. connect to the server
4. send/recieve
5. close the server
*/

#pragma comment(lib, "ws2_32.lib")

using namespace std;

bool initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void sendMessage(SOCKET clientSocket) {
	cout << "Enter your chat name = ";
	string name;
	getline(cin, name);

	string message;
	while (true) {
		getline(cin, message);
		string msg = name + " : " + message;
		int byteSent = send(clientSocket, msg.c_str(), msg.length(), 0);

		if (byteSent == SOCKET_ERROR) {
			cout << "Error ocurred in sending message\n";
			break;
		}

		if (message == "bye") {
			cout << "Stopping the chat\n";
			break;
		}
	}

	//close

	closesocket(clientSocket);

	//cleanup

	WSACleanup();
}

void RecieveMessage(SOCKET clientSocket) {
	char buffer[4096];
	int recievedLength;
	string message = "";

	while (true) {
		recievedLength = recv(clientSocket, buffer, sizeof(buffer), 0);

		if (recievedLength <= 0) {
			cout << "Disconnected from the server\n";
			break;
		}
		else {
			message = string(buffer, recievedLength);
			cout << message << "\n";
		}
	}

	//close
	closesocket(clientSocket);

	//cleanup

	WSACleanup();
}


int main()
{
	//initialize winsock

	if (!initialize()) {
		cout << "initialze failed\n";
		return 1;
	}

	//create socket

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (clientSocket == INVALID_SOCKET) {
		cout << "Invalid socket created\n";
		return 1;
	}

	int port = 3000;
	string serverAdress = "127.0.0.1";
	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(3000);
	if (inet_pton(AF_INET, serverAdress.c_str(), &serverAddress.sin_addr) == 1) {
		cout << "IP conversion successfull\n";
	}
	else {
		cout << "IP conversion failed\n";
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}


	//connect

	if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
		cout << "Cannot connect to the server\n";
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	cout << "Successfully connected to the server\n";

	//send

	thread senderThread(sendMessage, clientSocket);
	thread recieverThread(RecieveMessage, clientSocket);

	senderThread.join();
	recieverThread.join();

}

