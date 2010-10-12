#include <iostream>
#include <vector>
#include "HelloSocket.h"
#define pLog(msg) (cout<<msg<<endl)
using namespace std;

HelloSocket::HelloSocket(string host,size_t port):m_host(host),m_port(port),BUF_SIZE(1024)
{
	//init member variable
	m_rwfd = -1;
	m_workSockfd = -1;
	m_sdata = NULL;
	//get and cache all interface socket
	struct addrinfo hints,*ptr;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	char sPort[6];
	sprintf(sPort,"%d",port);
	const char *shost = NULL;
	shost = host.length()>0 ? host.c_str():NULL;
	int ret = getaddrinfo(shost,sPort,&hints,&m_pAddrInfo); // cache addrinfo
	if(ret!=0)
	{
		pLog(gai_strerror(ret));
		return ;
	}
	int sockfd = 0;
	for(ptr = m_pAddrInfo; ptr!=NULL;ptr=ptr->ai_next) // cache socket descriptor
	{
		sockfd = socket(ptr->ai_family,ptr->ai_socktype,ptr->ai_protocol)	;
		m_sockfds.push_back(sockfd);
	}

	//malloc memory for buf 
	m_sdata = (char *)malloc(BUF_SIZE);
	if(m_sdata == NULL)
	{
		pLog("can not malloc memory");
		abort();
	}
}

HelloSocket & HelloSocket::setOption()
{
	return *this;
}
/*
 *function name:HelloSocket::listen 
 *return:reference of HelloSocket object
 *param: void
 *
 * bind and listion 
 * */
HelloSocket & HelloSocket::listen()
{
	struct addrinfo *p = m_pAddrInfo;
	for(size_t i = 0 ;i < m_sockfds.size()&&p!=NULL; ++i) // loop to bind 
	{
		if(bind(m_sockfds[i],p->ai_addr,p->ai_addrlen)==-1)
		{
			p = p ->ai_next;
			continue;
		}
		m_workSockfd = m_sockfds[i];
		m_workAddr = p;
		break;
	}
	if(p == NULL)
	{
		perror("bind fiald");
		exit(1);
	}
	if(m_workSockfd!=-1 && ::listen(m_workSockfd,10)==-1) //repeat name ,so add limited scope
	{
		perror("listen");
		exit(1);
	}
	assert(m_workSockfd!=-1);

	return *this;
}
/*func: HelloSocket::accept
 *return: reference to HelloSocket object
 *param: void
 *
 *do  accept and wait for connection
 *
 * */
HelloSocket & HelloSocket::accept()
{
	socklen_t sa_size = sizeof(struct sockaddr);
	m_rwfd = ::accept(m_workSockfd,(struct sockaddr *)sockAddr((struct sockaddr*)m_workAddr->ai_addr),&sa_size);
	if(m_rwfd == -1)
	{
		perror("accept:");
	}
	return *this;
}
HelloSocket & HelloSocket::read()
{
	int cur = 0;
	int nrecv = 0 ;
	int const len = 128;
	while(true)
	{
		if(cur > BUF_SIZE){pLog("error: no enough memory");}
		nrecv = ::recv(m_rwfd,m_sdata+cur,len,0);
		if(nrecv == -1)
		{
			perror("recv");
			break;
		}
		else if(nrecv == len) cur += nrecv ;
		else if(nrecv < len)break;
	}
	m_sdata[cur] = 0;

	return *this;
}
HelloSocket & HelloSocket::send(char *data,size_t len)
{
	if(!data)return *this;
	int nsend = 0;
	int cur = 0;
	while(true) // loop until send complete
	{
		nsend = ::send(m_rwfd,data+cur,len,0);
		if(nsend == -1)
		{
			perror("send");
			break;
		}
		else if(nsend < len)
		{
			len -= nsend ;
			cur += nsend;
		}
		else if(nsend == len) break;
	}
	return *this;
}
std::string  HelloSocket::response()
{
	return m_sdata;
}
void * HelloSocket::sockAddr(struct sockaddr * saddr)
{
	if(saddr->sa_family==AF_INET)
	{
		return &(((struct sockaddr_in *)saddr)->sin_addr);
	}
	return &(((struct sockaddr_in6 *)saddr)->sin6_addr);
}
HelloSocket::~HelloSocket()
{
	freeaddrinfo(m_pAddrInfo);

	for(size_t i = 0 ;i < m_sockfds.size(); ++i)
	{
		close(m_sockfds[i]);
	}
}
HelloSocket &HelloSocket::connect()
{
	struct addrinfo *p = m_pAddrInfo;
	for(size_t i = 0 ;i < m_sockfds.size()&&p!=NULL; ++i) // loop to bind 
	{
		if(::connect(m_sockfds[i],p->ai_addr,p->ai_addrlen)==-1)
		{
			p = p ->ai_next;
			continue;
		}
		m_workSockfd = m_sockfds[i];
		m_workAddr = p;
		break;
	}
	return *this;
}
