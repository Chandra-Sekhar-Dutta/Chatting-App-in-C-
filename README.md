
# Chat Application using Winsock (TCP)

This is a simple chat application built using C++ and Windows Sockets (Winsock) API. The application enables multiple clients to communicate with each other by sending and receiving messages through a central server. The server supports multiple client connections using multithreading and broadcasts the messages received from one client to all connected clients.

## Table of contents
1. Libraries
 2. Processes
- Server-Side
- Winsock Initialization
- Socket Creation and Binding
- Listening and Accepting Connections
- Multithreading
- Message Broadcasting
- Socket Closure and Cleanup
 3. Client-Side
- Winsock Initialization
- Socket Creation and Connection
- Message Sending and Receiving
- Socket Closure and Cleanup
 4. How to Run
 5. Additional Information
## Libraries
This application requires the following libraries:
- WinSock2.h: Used for Windows socket programming.
- WS2tcpip.h: Provides additional socket functionality such as working with IP addresses.
- tchar.h: Provides functions for Unicode and ANSI character handling.
- thread: For creating and managing threads to handle multiple clients simultaneously.
- vector: For storing connected clients in the server.


## Code
````
#pragma comment(lib, "ws2_32.lib")

````

This ensures the use of the Winsock library.
## Processes
1. Server-Side
- Winsock Initialization
WSAStartup: Initializes the Winsock library. It must be called before using any Winsock functions. The function takes the requested Winsock version as input (2.2 in this case).

````
WSADATA data;
if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
    cout << "Winsock initialization failed." << endl;
    return 1;
}
````
- Socket Creation and Binding
Socket Creation: A listening socket is created using the socket() function. This socket will listen for incoming client connections.

Binding: The server socket is then bound to a specific IP address (0.0.0.0 to listen on all network interfaces) and port (3000). This is done with the bind() function.

````
SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
bind(listenSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));

````
- Listening and Accepting Connections
Listening: The server socket begins listening for incoming client connections using the listen() function.

Accepting Connections: The server accepts incoming connections using the accept() function, which returns a new socket for communication with the client.

````
listen(listenSocket, SOMAXCONN);
SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);

````

- Multithreading
Handling Multiple Clients: A new thread is spawned for each client that connects. This is done using the C++ thread library to handle the client communication in parallel, allowing multiple clients to interact with the server simultaneously.

````
thread clientThread(InteractWithClient, clientSocket);
clientThread.detach();

````
- Message Broadcasting
Broadcasting: Messages received from one client are broadcast to all other connected clients. This is achieved by iterating over the Clients vector, which stores the sockets of all connected clients, and using the send() function to forward the message to each client except the sender.

````
for (auto& client : Clients) {
    if (client != clientSocket) {
        send(client, message.c_str(), message.length(), 0);
    }
}

````
- Socket Closure and Cleanup
Closing Sockets: Once a client disconnects, the corresponding socket is closed using closesocket().

Cleanup: The WSACleanup() function is called to clean up the Winsock library when the application terminates.

````
closesocket(listenSocket);
WSACleanup();

````

2. Client-Side
- Winsock Initialization
Similar to the server side, the client needs to initialize the Winsock library using WSAStartup().

````
WSADATA data;
if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
    cout << "Winsock initialization failed." << endl;
    return 1;
}
````

- Socket Creation and Connection
The client creates a socket using the socket() function and then connects to the server using the connect() function.

````
SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));

````

- Message Sending and Receiving
Sending: The client reads messages from the user and sends them to the server. The message format includes the user's name.

Receiving: The client listens for incoming messages from the server and displays them.

````
send(clientSocket, message.c_str(), message.length(), 0);
recv(clientSocket, buffer, sizeof(buffer), 0);

````

- Socket Closure and Cleanup
After sending or receiving messages, the client socket is closed, and WSACleanup() is called to clean up the Winsock resources.

````
closesocket(clientSocket);
WSACleanup();

````




## How to Run

1. Clone the Repository: Clone this repository to your local machine.

2. Build the Project: Open the server.cpp and client.cpp files in your C++ IDE (such as Visual Studio 2022). Build the project.

3. Run the Server: First, run the server application. It will listen for incoming client connections on port 3000.

4. Run the Clients: Run the client applications on different terminals or different machines connected to the same network. Each client will connect to the server and will be able to send and receive messages.

5. Interact: Each client can send messages, and all other connected clients will receive those messages. Clients can type "bye" to exit the chat.
## Additional Information
- Multithreading: The server uses C++ threads to handle each client independently. This allows the server to manage multiple client connections simultaneously.

- Broadcasting: The server broadcasts messages from a client to all other clients, ensuring that each client can see the messages sent by others.

- Error Handling: Basic error handling is provided to catch socket creation, connection, and message transmission issues.

- Future Improvements:

    - Authentication mechanism to verify users.
    - File sharing support.
    - Persistent storage (using databases) to store message history.
## Contributing

Contributions are always welcome! If you would like to contribute to this project, please follow the guidelines below:

- Fork the repository and create your own branch.
- Make your changes and ensure the code is well-documented.
- Test your changes thoroughly.
- Submit a pull request with a detailed description of the changes you made.

This project is open-source and can be used freely for educational and personal purposes.







## Authors

[@Chandra Sekhar Dutta](https://github.com/Chandra-Sekhar-Dutta): The author of this chat application project, responsible for the development of both the server and client-side code, implementing the Winsock-based socket communication, multithreading, and message broadcasting features. The author aims to provide a simple yet effective implementation of a TCP chat server and client using C++ for educational purposes.
