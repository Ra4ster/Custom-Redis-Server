#include "Redis.h"

#pragma comment(lib, "ws2_32.lib") // Links to winsock lib!

#include <string>
#include <thread>

using namespace std;

void rainbowPrint(string words);

int currColor = 0;
bool testedOnce = false;

Redis::Redis(int port) : port(port)
{
	startWinsock();
	getSocket();
}
Redis::Redis() : port(DEFAULT_PORT)
{
	startWinsock();
	getSocket();

}

int Redis::terminate()
{
	std::cout << "Connection terminated.\n";
	closesocket(serverSocket);
	WSACleanup();
	return -1;
}

Redis::~Redis() {}

void Redis::setServable(bool serve)
{
	canServe = serve;
}
void Redis::setAddress(std::string address)
{
	this->address = address;
}
bool Redis::isServable() const
{
	return canServe;
}

int Redis::serve()
{
	canServe = true;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(address.c_str()); // Listen on all interfaces
	serverAddr.sin_port = htons(port);

	if (!testedOnce)
	{
		bindSocket();
		setConsoleColor(GREEN);
		std::cout << "Welcome to... \n";
		std::cout << REDIS_LOGO;
		std::cout << "~Made by RA4.\n";

		std::cout << "INFO: Server listening on port: " << port << endl;

		std::cout << "(h)=help, (u)=users connected, all other keys stop the server..." << endl;
		testedOnce = true;
	}

		if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			cerr << "Listen failed." << endl;
			closesocket(serverSocket);
			WSACleanup();
			return 1;
		}

		while (canServe)
		{
			stopServerOnChar(); // Check for input

			FD_ZERO(&readfds);
			FD_SET(serverSocket, &readfds);
			SOCKET maxfd = serverSocket;

			for (auto& [s, buff] : clients)
			{
				FD_SET(s, &readfds);
				if (s > maxfd) maxfd = s;
			}

			fd_set tempfds = readfds;
			struct timeval timeout = { 0, 100000 }; // 100ms timeout
			int selectResult = select(0, &tempfds, NULL, NULL, &timeout);

			if (selectResult < 0 && WSAGetLastError() != WSAEINTR)
			{
				std::cerr << "select error: " << WSAGetLastError() << std::endl;
				break;
			}

			if (!canServe) return 0;

			// new connections
			if (FD_ISSET(serverSocket, &tempfds))
			{
				SOCKET clientSocket = accept(serverSocket, NULL, NULL);
				if (clientSocket != INVALID_SOCKET)
				{
					std::cout << "New client connected!\n";
					clients[clientSocket];

				}
			}

			// Handle clients
			for (auto it = clients.begin(); it != clients.end();)
			{
				SOCKET clientSocket = it->first;

				if (FD_ISSET(clientSocket, &tempfds))
				{
					char inputBuffer[1024];
					int recvResult = recv(clientSocket, inputBuffer, sizeof(inputBuffer) - 1, 0);

					if (recvResult <= 0)
					{
						std::cout << "Client disconnected.\n";
						closesocket(clientSocket);
						it = clients.erase(it);
						continue;
					}
					else
					{
						inputBuffer[recvResult] = '\0';
						it->second += std::string(inputBuffer, recvResult);

						std::string& buffer = it->second;
						size_t pos;

						while ((pos = buffer.find("\r\n")) != std::string::npos)
						{
							std::string command = buffer.substr(0, pos);
							buffer.erase(0, pos + 2);

							if (!Messages::handleAll(clientSocket, command.c_str(), static_cast<int>(command.length())))
							{ // Client quits
								closesocket(clientSocket);
								it = clients.erase(it);
								goto next_client;
							}
						}
						it++;
					}
				}
				else { it++; }
			}
			next_client:;
		}
		return 0;
}

int Redis::start()
{
	while (isServable())
	{
		int result = serve();
		if (result != 0) return result;
	}

	return 0; // unreachable
}

void rainbowPrint(string words)
{
	int rainbowColors[] = { RED, YELLOW, GREEN, AQUA, BLUE, PURPLE };
	int charCount = 0;
	for (int i = 0; words[i] != '\0'; i++)
	{
		Redis::setConsoleColor(rainbowColors[currColor]);
		charCount++;

		std::cout << words[i];

		if (charCount % 2 == 0) // even
		{
			currColor = (static_cast<unsigned long long>(currColor) + 1) % (sizeof(rainbowColors) / sizeof(rainbowColors[0]));
		}
	}
	Redis::setConsoleColor(WHITE);
}