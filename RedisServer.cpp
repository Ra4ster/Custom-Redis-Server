
#include "Redis.h"

using namespace std;

/*
* argsc = number of cmdline arguments
* argsv = argument words
*/
int main(int argsc, char* argsv[])
{
	Redis redisServer(6379);
	redisServer.setServable(true);

	while (redisServer.isServable())
	{
		redisServer.serve();
	}
}