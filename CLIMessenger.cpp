#include "CLIMessenger.h"
#include "framework.h"
#include <iostream>

namespace Messages
{
	CLIMessenger::CLIMessenger() {};

	//void printLine(SOCKET client)
	//{
	//	char address[16];
	//	strcpy_s(address, inet_ntoa(serverAddr.sin_addr));
	//	std::string result = std::string("\r\n") + address + std::string(":") + to_string(port) + std::string(">") + yellow;
	//	send(client, result.c_str(), static_cast<int>(result.length()), 0);
	//}

	std::string CLIMessenger::processTerminal(const char* msg, size_t len)
	{
		std::string fin;

		for (int i = 0; i < len; i++)
		{
			if (msg[i] == '\b')
			{
				if (!fin.empty())
				{
					fin.pop_back();
				}
			}
			else
			{
				fin += msg[i];
			}
		}

		return fin;
	}

	void CLIMessenger::echo(SOCKET socket, const char* msg)
	{
		std::string msgStr = processTerminal(msg, strlen(msg));
		int len = static_cast<int>(msgStr.length());
		std::vector<char> echomsg(len + 1);

		memcpy(echomsg.data(), msg, len);
		echomsg[len] = '\0';

		send(socket, echomsg.data(), len + 1, 0);
	};
	void CLIMessenger::quit(SOCKET client) 
	{
		send(client, reset, static_cast<int>(strlen(reset)), 0);
		send(client, "+OK Goodbye", 12, 0);
		closesocket(client);
	};
	void CLIMessenger::err(SOCKET client)
	{
		send(client, "-ERR unknown command", 20, 0);
	};
	void CLIMessenger::set(SOCKET client, const char* kv)
	{
		std::string kvStr = processTerminal(kv, strlen(kv));
		std::string key = kvStr.substr(0, kvStr.find(' '));
		std::string value = kvStr.substr(kvStr.find(' ') + 1);

		RedisStore::store[key] = value;
		std::cout << "Inserted pair: (" << key << ", " << value << ")" << std::endl;
		send(client, "+OK", 3, 0);
	};
	void CLIMessenger::get(SOCKET client, const char* key)
	{
		std::string keyStr = processTerminal(key, strlen(key));
		if (RedisStore::store.find(key) != RedisStore::store.end())
		{
			std::string val = "\"" + RedisStore::store[keyStr] + "\"";
			std::string resp = "$" + std::to_string(val.size()) + "\r\n" + val + "\r\n";
			std::cout << "Got value for key \"" << keyStr << "\": " << val << std::endl;
			send(client, resp.c_str(), static_cast<int>(resp.length()), 0);
		}
		else
		{
			send(client, "$-1", 4, 0);
		}
	};
	void CLIMessenger::del(SOCKET client, const char* keys)
	{
		int counter = 0;
		std::string input = processTerminal(keys, strlen(keys));

		size_t pos = 0;
		while (pos < input.length())
		{ // pos = beginning of word
			pos = input.find_first_not_of(' ', pos);
			if (pos == std::string::npos) break;
			// end = end of word
			size_t end = input.find(' ', pos);

			std::string currKey = input.substr(pos, end - pos); // currKey = end - beginning of word
			if (RedisStore::store.find(currKey) != RedisStore::store.end())
			{
				RedisStore::store.erase(currKey);
				counter++;
			}

			pos = end;
		}
		std::string resp = "(integer) " + std::to_string(counter);
		send(client, resp.c_str(), static_cast<int>(resp.length()), 0);
	};
}
