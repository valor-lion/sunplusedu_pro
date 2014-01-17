
#ifndef	__IPMSG_NET_H
#define 	__IPMSG_NET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

struct ipmsg_net{

	int fd_tcpsvr;
	int fd_tcpclt;
	
	int fd_udpsvr;
	int fd_udpclt;

	unsigned long		skport_local;
	struct sockaddr_in		skaddr_local;
	
	struct sockaddr_in		skaddr_tcpclt;
	struct sockaddr_in		skaddr_udpclt;

	struct ipmsg_net_operations	*net_ops;
	void *private_data;
};

struct ipmsg_net_operations{
	int (*net_tcpsvr_start)(struct ipmsg_net *net);
	int (*net_udpsvr_start)(struct ipmsg_net *net);

	

	void (*net_tcpsvr_stop)(struct ipmsg_net *net);
	void (*net_udpsvr_stop)(struct ipmsg_net *net);

	void (*net_broad_cast)(struct ipmsg_net *net);

};


#endif

