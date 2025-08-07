#include "RawMessenger.h"
#include "RedisMessenger.h"

namespace Messages
{
	RAWMessenger::RAWMessenger() {}

	void RAWMessenger::set(SOCKET client, const char* kv)
	{ // Sets a key to a value in the store
		std::string kvStr(kv);
		std::string key = kvStr.substr(0, kvStr.find(' ')).c_str();
		std::string value = kvStr.substr(kvStr.find(' ') + 1).c_str();

		RedisStore::store[key] = value;
		send(client, "+OK\r\n", 5, 0);
	};
	void RAWMessenger::get(SOCKET client, const char* key)
	{ // If the store finds said key, return value
		if (RedisStore::store.find(key) != RedisStore::store.end())
		{
			std::string val = RedisStore::store[std::string(key)];
			std::string resp = "$" + std::to_string(val.size()) + "\r\n" + val + "\r\n";
			send(client, resp.c_str(), static_cast<int>(resp.length()), 0);
		}
		else
		{
			send(client, "$-1\r\n", 5, 0);
		}
	};
	void RAWMessenger::del(SOCKET client, const char* keys)
	{ // Iterate through all keys entered and delete them if they exist; then return number deleted
		int counter = 0;
		std::string input(keys);

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
		std::string resp = ":" + std::to_string(counter) + "\r\n";
		send(client, resp.c_str(), static_cast<int>(resp.length()), 0);
	};
	void RAWMessenger::quit(SOCKET client)
	{ // closes socket
		send(client, "+OK\r\n", 5, 0);
		closesocket(client);
	};
	void RAWMessenger::echo(SOCKET client, const char* msg)
	{ // sends message back
		std::string resp = "$" + std::to_string(strlen(msg)) + "\r\n" + msg + "\r\n";
		send(client, resp.c_str(), static_cast<int>(resp.length()), 0);
	};
	void RAWMessenger::err(SOCKET client)
	{
		send(client, "-ERR unknown command\r\n", 22, 0);
	};

}