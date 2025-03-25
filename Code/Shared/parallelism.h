#include <pthread.h>	//needed for packet threads

//using a doubly linked list for janitorial duties
struct pt_node {
    pt_node * prev;
    pthread packet_thread;
    pt_node * next;
}
