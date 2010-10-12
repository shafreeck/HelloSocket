#include <iostream>
#include <string>
#include "HelloSocket.h"
using namespace std;

int main(int argc, char *argv[])
{
	HelloSocket hs("localhost",8804);
	hs.connect();
	char *msg = "hello server!";
	hs.send(msg,strlen(msg));
	return 0;
}
