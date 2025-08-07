#pragma once
#include <winsock.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

namespace Messages
{
	struct Message
	{
		const char* msg;
		int sz;
	};

	extern Message PONG;
	extern Message ERR;
	extern Message CLOSE;


	extern void echo(SOCKET socket, const char* msg);
	extern bool handleAll(SOCKET client, const char* arr, int len);

	class Messenger
	{
	public:
		virtual void set(SOCKET client, const char* pair) = 0;
		virtual void get(SOCKET client, const char* key) = 0;
		virtual void del(SOCKET client, const char* keys) = 0;
		virtual void quit(SOCKET client) = 0;
		virtual void echo(SOCKET client, const char* msg) = 0;
		virtual void err(SOCKET client) = 0;

		~Messenger() {};
	};
}
