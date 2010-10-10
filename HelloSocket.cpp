#include <iostream>
#include <vector>
#include "HelloSocket.h"
#define pLog(msg) (cout<<msg<<endl)
using namespace std;

HelloSocket::HelloSocket(string host,size_t port):m_host(host),m_port(port)
{
	m_rwfd = -1;
	m_sdata = NULL;
	struct addrinfo hints,*ptr;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	char sPort[6];
	sprintf(sPort,"%d",port);
	int ret = getaddrinfo(NULL,sPort,&hints,&m_pAddrInfo);
	if(ret!=0)
	{
		pLog(gai_strerror(ret));
		return ;
	}
	int sockfd = 0;
	for(ptr = m_pAddrInfo; ptr!=NULL;ptr=ptr->ai_next)
	{
		sockfd = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol)	;
		m_sockfds.push_back(sockfd);
	}
}

HelloSocket & HelloSocket::setOption()
{
	return *this;
}
HelloSocket & HelloSocket::listen()
{
	struct addrinfo *p = m_pAddrInfo;
	for(size_t i = 0 ;i < m_sockfds.size(); ++i) // loop to bind 
	{
		if(bind(m_sockfds[i],p->ai_addr,p->ai_addrlen)==-1)
		{
			close(m_sockfds[i]);
			m_sockfds.erase(m_sockfds.begin()+i);
			p = p ->ai_next;
			continue;
		}
		m_workSockfd = m_sockfds[i];
		m_workAddr = p;
		break;
	}
	if(::listen(m_workSockfd,10)==-1) //repeat name ,so add limited scope
	{
		perror("listen:");
		exit(1);
	}

	return *this;
}
HelloSocket & HelloSocket::accept()
{
	socklen_t sa_size = sizeof(struct sockaddr);
	if(::accept(m_workSockfd,(struct sockaddr *)sockAddr((struct sockaddr*)m_workAddr->ai_addr),&sa_size)==-1)
	{
		perror("accept:");
	}
	return *this;
}
HelloSocket & HelloSocket::read()
{
	return *this;
}
HelloSocket & HelloSocket::send(string msg)
{
	return *this;
}
std::string  HelloSocket::response()
{
	return m_sdata;
}
