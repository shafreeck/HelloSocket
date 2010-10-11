#include <stdlib.h>
#include <assert.h>
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
		const size_t BUF_SIZE ;
		std::string m_host; // hostname or ip address
		size_t m_port; // port to listen
		std::vector<int> m_sockfds; // cache for all socket descirptor for a hostname
		int m_workSockfd; // real work socket descirptor (bind and listen should be invoked success)
		int m_rwfd;      // socket descirptor to read or send data
		char *m_sdata;  // buf for data
		struct addrinfo *m_pAddrInfo; // cache for all addrinfos
		struct addrinfo *m_workAddr; // real work addr
		void *sockAddr(struct sockaddr*); // get addr which adapts to ipv4 or ipv6
			
	public:
		HelloSocket(std::string host="",size_t port=8804); //create socket
		~HelloSocket(); //create socket
		HelloSocket &setOption(); //set options
		HelloSocket &listen(); //bind and listen
		HelloSocket &accept(); // accept connections
		HelloSocket &read();  // read network data
		HelloSocket &send(std::string msg="");  // read network data
		HelloSocket &connect();  // connect to server
/*
		std::string &getMyIp();
		std::string &getPeerIp();*/
		std::string response();
};
