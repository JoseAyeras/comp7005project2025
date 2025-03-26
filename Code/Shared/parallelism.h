#include <pthread.h>	//needed for packet threads

//using a doubly linked list for janitorial duties
struct pt_node {
    pt_node * prev;
    pthread packet_thread;
    pt_node * next;
}

//creating the parameter bag that our threads will drink from
struct pt_param {
	bool is_client,				//is this a client, or a server?
	arguments& arguments,			//arguments
	pt_node * self,				//node in doubly linked list
	int sockfd,				//socket file descriptor
	char* buffer,				//message
	ssize_t msg_len,			//message length
	struct sockaddr_in* target_addr,	//destination address
	socklen_t* addr_len			//address length
}
