#pragma once
#include "framework.h"
#include "RedisMessenger.h"

#include <winsock.h>
#include <conio.h>
#include <iostream>
#include <vector>
#include <unordered_map>


using namespace std;

class Redis
{
public:
	Redis(int port);
	Redis();
	~Redis();

	int terminate();

	int serve();
	int start();
	void setAddress(std::string address);
	void setServable(bool);
	bool isServable() const;

	static void setConsoleColor(int color)
	{
		HANDLE hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hHandle, color);
	}

protected:
	int port;
	std::string address = "127.0.0.1";
	bool canServe = false;

	int currColor = 0;

	WSADATA wsadata;
	SOCKET serverSocket;
	std::unordered_map<SOCKET, std::string> clients;

	struct sockaddr_in serverAddr;
	fd_set readfds;

private:

	void startWinsock()
	{
		int wsaResult = WSAStartup(
			MAKEWORD(2, 2), // Makeword (lowbyte, highbyte)
			&wsadata // api data
		);
		if (wsaResult != 0)
		{
			std::cout << "ERROR GETTING WINSOCK API; CODE: " << wsaResult << endl;
			exit(1);
		}
		std::cout << "INFO: Winsock API received successfully." << endl;
	}
	void getSocket()
	{
		serverSocket = socket(
			AF_INET, // IPV4 (my ip address)
			SOCK_STREAM, // TCP
			0
		);

		if (serverSocket == INVALID_SOCKET)
		{
			cerr << "Socket creation failed." << endl;
			checkSocketError();
			WSACleanup();
			exit(1);
		}

		std::cout << "INFO: Socket created successfully." << endl;
	}
	void bindSocket() const
	{
		int result = bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
		if (result == SOCKET_ERROR)
		{
			cerr << "ERROR: Socket unable to be binded." << endl;
			closesocket(serverSocket);
			WSACleanup();

			exit(1);
		}

		std::cout << "INFO: Socket binded successfully." << endl;
	}

	void checkSocketError()
	{
		int errorCode = WSAGetLastError();
		cerr << "Socket creation failed with error code: " << errorCode << endl;
		// You can add more specific checks for common error codes here
		if (errorCode == WSANOTINITIALISED)
		{
			cerr << "Winsock not initialized properly." << endl;
		}
		else if (errorCode == WSAEADDRINUSE)
		{
			cerr << "Port is already in use." << endl;
		}
		else if (errorCode == WSAEACCES)
		{
			cerr << "Permission denied (check firewall or system permissions)." << endl;
		}
		else
		{
			cerr << "Unknown error occurred." << endl;
		}
	}
	void stopServerOnChar()
	{
		if (_kbhit())
		{
			char ch = _getch();
			if (ch == 'u')
			{
				cout << "Sockets currently open:\n";
				for (auto const& client : clients)
				{
					cout << "Socket #" << client.first << '\n';
				}
			}
			else if (ch == 'h')
			{
				cout << help << "\n\n";
			}
			else if (ch)
			{
				std::string answer;
				std::cout << "Are you sure you want to stop the server? [Y] for yes.\n";
				std::cin >> answer;
				if (answer == "Y" || answer == "y")
				{
					std::cout << "INFO: Server Stopping on key: \'" << ch << '\'' << endl;
					setConsoleColor(WHITE);
					canServe = false;
				}
				else {
					std::cout << "Understood. Server still running.\n";
				}
			}
		}
	}
};
