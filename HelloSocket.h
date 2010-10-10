#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <string>
#include <vector>

class HelloSocket
{
	private:
		std::string m_host;
		size_t m_port;
		std::vector<int> m_sockfds;
		int m_workSockfd;
		int m_rwfd;
		char *m_sdata;
		struct addrinfo *m_pAddrInfo;
		struct addrinfo *m_workAddr;
		void *sockAddr(struct sockaddr*);
			
	public:
		HelloSocket(std::string host="",size_t port=8804); //create socket
		HelloSocket &setOption(); //set options
		HelloSocket &listen(); //bind and listen
		HelloSocket &accept(); // accept connections
		HelloSocket &read();  // read network data
		HelloSocket &send(std::string msg="");  // read network data
/*
		std::string &getMyIp();
		std::string &getPeerIp();*/
		std::string response();
};
