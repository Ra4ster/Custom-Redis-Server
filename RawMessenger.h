#pragma once
#include "RedisMessenger.h"
#include "RedisStore.h"

namespace Messages
{
	class RAWMessenger : public Messenger
	{
	public:
		RAWMessenger();

		 void set(SOCKET client, const char* pair) override;
		 void get(SOCKET client, const char* key) override;
		 void del(SOCKET client, const char* keys) override;
		 void quit(SOCKET client) override;
		 void echo(SOCKET client, const char* msg) override;
		 void err(SOCKET client) override;
	};
}