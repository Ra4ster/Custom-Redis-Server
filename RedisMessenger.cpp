#include "RedisMessenger.h"
#include "RedisStore.h"
#include "RawMessenger.h"
#include "CLIMessenger.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include "framework.h"
#include <ranges>

namespace Messages
{
	std::unordered_map<SOCKET, std::unique_ptr<Messenger>> socketMode;

	Message PONG = { "+PONG\r\n", 7 };

	bool handleAll(SOCKET client, const char* arr, int len)
	{
		// If not in a mode, set to raw; if in CLI mode, process terminal:
		if (socketMode.find(client) == socketMode.end()) socketMode[client] = std::make_unique<RAWMessenger>();
		std::string msg = std::string(arr);
		if ((dynamic_cast<CLIMessenger*>(socketMode[client].get()))) msg = CLIMessenger::processTerminal(arr, len);

		std::string command = "";
		size_t spacePos = msg.find(' ');
		if (spacePos != std::string::npos)
		{ // transform command to uppercase
			command = msg.substr(0, spacePos);
			std::transform(command.begin(), command.end(), command.begin(), std::toupper);
		}

		if (msg == "PING")
		{
			send(client, PONG.msg, PONG.sz, 0);
		}
		else if (command == "ECHO")
		{
			socketMode[client]->echo(client, msg.substr(spacePos+1, msg.length()).c_str());
		}
		else if (msg.substr(0, 5) == "quit")
		{

			socketMode[client]->quit(client);

			socketMode.erase(client);
			return false;
		}
		else if (command == "SET")
		{
			socketMode[client]->set(client, msg.substr(spacePos+1, msg.length()).c_str());
		}
		else if (command == "GET")
		{
			socketMode[client]->get(client, msg.substr(spacePos+1, msg.length()).c_str());
		}
		else if (command == "DEL")
		{
			socketMode[client]->del(client, msg.substr(spacePos+1, msg.length()).c_str());
		}
		else if (command == "MODE")
		{
			std::string params = msg.substr(spacePos+1, msg.length());
			if (params == "CLI")
			{
				socketMode[client] = std::make_unique<CLIMessenger>();
				send(client, "+OK\r\n", 5, 0);
			}
			else if (params == "RAW")
			{
				socketMode[client] = std::make_unique<RAWMessenger>();
				send(client, "+OK\r\n", 5, 0);
			}
			else
			{
				send(client, "-ERR unknown mode\r\n", 21, 0);
			}
		}
		else
		{
			socketMode[client]->err(client);
		}
		return true;
	}
}