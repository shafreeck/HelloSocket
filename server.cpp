#include <iostream>
#include <cstdlib>
#include <cctype>
#include "HelloSocket.h"

using namespace std;

int main(int argc, char *argv[])
{
	HelloSocket hs("localhost",8804);//create socket 
	hs.listen();//listen on 8804
	cout<<"listening on 8804 ..."<<endl;
	hs.accept();   // wait to connection
	hs.read();
	//cout<<hs.response()<<endl;
	hs.send("hello",sizeof("hello"));
	hs.read();
	//cout<<hs.response()<<endl;
	return 0;
}
