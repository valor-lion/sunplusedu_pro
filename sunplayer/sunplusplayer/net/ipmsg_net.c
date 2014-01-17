
/***************************************************************
  *Project Name:			sunplayer  
  *File Name:				net/ipmsg_net.c  
  *Description:			
  *
  *Date and Edition:		2012-11-30  		v1.0
  *Author:				Valor Lion
  **************************************************************/


static int net_tcp_server_start(struct ipmsg_net *net)
{

}

static int net_udp_server_start(struct ipmsg_net *net)
{

}

static void net_tcp_server_stop(struct ipmsg_net *net)
{

}

static void net_udp_server_stop(struct ipmsg_net *net)
{

}

static void net_broad_cast(struct ipmsg_net *net)
{

}


  struct ipmsg_net_operations ipmsgnet_ops = {
	.net_tcpsvr_start = net_tcp_server_start,
	.net_udpsvr_start = net_udp_server_start,
	.net_tcpsvr_stop = net_tcp_server_stop,
	.net_udpsvr_stop = net_udp_server_stop,
	.net_broad_cast = net_broad_cast,
  };

int 

  