
#include <argp.h>	//needed for arg parsing
#include <pthread.h>	//needed for packet threads

#include "../Shared/networking.h" //go to the shared networking header file

/*
 *  Unfortunately there are some global variables that are needed due to cleanup function. Because of this I got lazy with variable security.
 *  proxy_socket
 *  packet_threads
 *  num_threads
 *
 *  port_to_client map
 *  client_to_port map
 */
int proxy_socket = -1;
pthread_t * packet_threads;
int num_threads = 0;


// Keys for options without short options.
// We may not actually need it but the documentation here is useful regardless.
enum opt_codes {
    //there are 18 optcodes, the bits are as follows:
    //^4  0| proxy behavior     1| ip address
    //^3  01| broad     00| local
    //^2  11| broad delay++   10| both client and server      01| client      00| server
    //^1  111| broad delay m* 110| broad delay/port     1| delay time    0| chances
    //^0  11| max delay 10| min delay   01| delay chance        00| drop chance
     OPT_SDRP	//0     server drop chance
    ,OPT_SDLY	//1     server delay chance
    ,OPT_STMN	//2     server delay time minimum
    ,OPT_STMX	//3     server delay time maximum
    
    ,OPT_CDRP	//4     client drop chance
    ,OPT_CDLY	//5     client delay chance
    ,OPT_CTMN	//6     client delay time minimum
    ,OPT_CTMX   //7     client delay time maximum
    
    ,OPT_UDRP	//8     drop chance
    ,OPT_UDLY	//9     delay chance
    ,OPT_UTMN	//A     delay time minimum
    ,OPT_UTMX   //B     delay time maximum
    
    ,OPT_ST	//C     server delay time
    ,OPT_CT	//D     client delay time
    ,OPT_T	//E     delay time
    ,OPT_PORT   //F     port
    
    ,OPT_SI	//10    server ip address
    ,OPT_PI	//11    proxy ip address
}

/* Used by main to communicate with parse_opt. */
struct arguments
{
    char* //the following are strings:
    //proxy IP
    proxy_ip,
    //server IP
    server_ip;
    //port
    int port;
    float //the following are floats:
    //drop chances
        //range is from 0.0 (never) to 1.0 (always)
    client_drop_chance, server_drop_chance,
    //delay chances
        //same range as drop
        //chance of delayed packet = (1-drop chance)(delay chance)
        //delay roll only triggers when drop doesn't roll
    client_delay_chance, server_delay_chance,
    //delay min and max times
        //separate numbers used to allow greater experimentation scope, if needed
        //when delay rolls, delay time is chosen between min and max times
    client_delay_time_min, client_delay_time_max,
    server_delay_time_min, server_delay_time_max;

    //order of options:
        //widest scope options (most easily overridden)
            //--drop_chance
            //--delay_chance
            //--delay_time
        //tier 2
            //--client_drop_chance
            //--server_drop_chance
            //--client_delay_chance
            //--server_delay_chance
            //--client_delay_time
            //--server_delay_time
        //tier 3
            //--delay_time_min
            //--delay_time_max
        //final tier
            //--client_delay_time_min
            //--client_delay_time_max
            //--server_delay_time_min
            //--server_delay_time_max
    //lowest tiers will be coded first to ensure full functionality for project
};

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;
        switch (key)
    {
	case 0x00:
            //server drop chance
            arguments->server_drop_chance = strtof(arg, 0);
        break;
	case 0x01:
	    //server delay chance
            arguments->server_delay_chance = strtof(arg, 0);
        break;
	case 0x02:
	    //server delay time minimum
            arguments->server_delay_time_min = strtof(arg, 0);
        break;
	case 0x03:
	    //server delay time maximum
            arguments->server_delay_time_max = strtof(arg, 0);
        break;
        
	case 0x04:
	    //client drop chance
            arguments->client_drop_chance = strtof(arg, 0);
        break;
	case 0x05:
	    //client delay chance
            arguments->client_delay_chance = strtof(arg, 0);
        break;
	case 0x06:
	    //client delay time minimum
            arguments->client_delay_time_min = strtof(arg, 0);
        break;
	case 0x07:
	    //client delay time maximum
            arguments->client_delay_time_max = strtof(arg, 0);
        break;
        
	case 0x08:
	    //drop chance
            arguments->server_drop_chance = strtof(arg, 0);
            arguments->client_drop_chance = strtof(arg, 0);
        break;
	case 0x09:
	    //delay chance
            arguments->server_delay_chance = strtof(arg, 0);
            arguments->client_delay_chance = strtof(arg, 0);
        break;
	case 0x0A:
	    //delay time minimum
            arguments->server_delay_time_min = strtof(arg, 0);
            arguments->client_delay_time_min = strtof(arg, 0);
        break;
	case 0x0B:
	    //delay time maximum
            arguments->server_delay_time_max = strtof(arg, 0);
            arguments->client_delay_time_max = strtof(arg, 0);
        break;
        
	case 0x0C:
	    //server delay time
            arguments->server_delay_time_min = strtof(arg, 0);
            arguments->server_delay_time_max = strtof(arg, 0);
        break;
	case 0x0D:
	    //client delay time
            arguments->client_delay_time_min = strtof(arg, 0);
            arguments->client_delay_time_max = strtof(arg, 0);
        break;
	case 0x0E:
	    //delay time
            arguments->server_delay_time_min = strtof(arg, 0);
            arguments->server_delay_time_max = strtof(arg, 0);
            arguments->client_delay_time_min = strtof(arg, 0);
            arguments->client_delay_time_max = strtof(arg, 0);
        break;
	case 0x0F:
	    //port
	    arguments->port = atoi(arg);
        break;
        
	case 0x10:
	    //server ip address
	    arguments->server_ip = arg;
        break;
	case 0x11:
	    //proxy ip address
	    arguments->proxy_ip = arg;
        break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
}

static char doc[] =
	"This documentation is incomplete. There may be repeated information.\
	\vSee design document and/or user guide for details on how this program is meant to work.\
	";

static char args_doc[] =
	"\
	All of the following needs to be set:\n\
	--server-drop-chance\tfloat, [0.0,1.0]\n\
	--server-delay-chance\tfloat, [0.0,1.0]\n\
	--server-delay-time-min\tfloat, ∈R\n\
	--server-delay-time-max\tfloat, ∈R\n\
	\n\
	--client-drop-chance\tfloat, [0.0,1.0]\n\
	--client-delay-chance\tfloat, [0.0,1.0]\n\
	--client-delay-time-min\tfloat, ∈R\n\
	--client-delay-time-max\tfloat, ∈R\n\
	\n\
	--port\tunsigned integer\n\
	\t(this is the port that the proxy imitates from the server,\n\
	\ttherefore it must match the port number that the clients attempt to connect to.)\n\
	--server-ip\tIPv4 address (IPv6 not supported)\n\
	--proxy-ip\tIPv4 address (IPv6 not supported)\n\
	\n\
	Removing 'server'/'client' from any of the chance/time parameter names above will set both the server and the client.\n\
	Removing 'min'/'max' from any of the delay time parameter names above will set both the min and the max.\n\
	The above generalizations also work together. Therefore, the option --delay-time sets:\n\
	--server-delay-time-min\n\
	--server-delay-time-max\n\
	--client-delay-time-min\n\
	--client-delay-time-max\n\
	";

static struct argp_option options[] = {
	{"server-drop-chance",		0x00, 0, 0, "float, [0.0,1.0]\tDrop chance of server packets."},
	{"server-delay-chance",		0x01, 0, 0, "float, [0.0,1.0]\tDelay chance of server packets that aren't dropped."},
	{"server-delay-time-min",	0x02, 0, 0, "float, ∈R+\tMinimum delay time of delayed server packets."},
	{"server-delay-time-max",	0x03, 0, 0, "float, ∈R+\tMaximum delay time of delayed server packets."},
	
	{"client-drop-chance",		0x04, 0, 0, "float, [0.0,1.0]\tDrop chance of client packets."},
	{"client-delay-chance",		0x05, 0, 0, "float, [0.0,1.0]\tDelay chance of client packets that aren't dropped."},
	{"client-delay-time-min",	0x06, 0, 0, "float, ∈R+\tMinimum delay time of delayed client packets."},
	{"client-delay-time-max",	0x07, 0, 0, "float, ∈R+\tMaximum delay time of delayed client packets."},
	
	{"drop-chance",			0x08, 0, 0, "float, [0.0,1.0]\tDrop chance of ALL packets."},
	{"delay-chance",		0x09, 0, 0, "float, [0.0,1.0]\tDelay chance of ALL packets that aren't dropped."},
	{"delay-time-min",		0x0A, 0, 0, "float, ∈R+\tMinimum delay time of delayed packets."},
	{"delay-time-max",		0x0B, 0, 0, "float, ∈R+\tMaximum delay time of delayed packets."},
	
	{"server-delay-time",		0x0C, 0, 0, "float, ∈R+\tFIXED delay time of delayed server packets."},
	{"client-delay-time",		0x0D, 0, 0, "float, ∈R+\tFIXED delay time of delayed client packets."},
	{"delay-time",			0x0E, 0, 0, "float, ∈R+\tFIXED delay time of delayed packets."},
	{"port",			0x0F, 0, 0, "unsigned integer\tPort that the proxy imitates from the server; client's expected connection port."},
	
	{"server-ip",			0x10, 0, 0, "IPv4 address of the server. IPv6 is not supported."},
	{"proxy-ip",			0x11, 0, 0, "IPv4 address of the proxy. Remember, if your computer is connected to the internet, you have multiple IP addresses!"},
}

void arg_init(arguments& arguments){
    arguments->server_drop_chance	=
    arguments->server_delay_chance	=
    arguments->server_delay_time_min	=
    arguments->server_delay_time_max	=
    
    arguments->client_drop_chance	=
    arguments->client_delay_chance	=
    arguments->client_delay_time_min	=
    arguments->client_delay_time_max	=
    
    -1.0; //this value will fail the arg validation function, which is a good thing
    arguments->port = -1;
}

bool arg_valid(arguments& arguments) {
    return
	//stop program if a delay time is negative! time travel is fiction!
    	arguments->server_delay_time_min < 0.0 || //NO BRACKETS NEEDED on account of operator precedence: dereference occur before comparison occur before logical OR
    	argumetns->server_delay_time_max < 0.0 ||
    	arguments->client_delay_time_min < 0.0 ||
    	arguments->client_delay_time_max < 0.0 ||
    	//stop program if any probabilities fall outside [0.0,1.0]
    	arguments->server_drop_chance  < 0.0 ||
    	arguments->server_delay_chance < 0.0 ||
    	arguments->client_drop_chance  < 0.0 ||
    	arguments->client_delay_chance < 0.0 ||
    	
    	arguments->server_drop_chance  < 1.0 ||
    	arguments->server_delay_chance < 1.0 ||
    	arguments->client_drop_chance  < 1.0 ||
    	arguments->client_delay_chance < 1.0 ||
    	//stop program if, somehow, port is below 0
    	arguments->port < 0
    ;
}

//pass in arguments from outside
//YOU ALSO NEED:
//  1. arguments struct (handle_args will initialize it for you
//  2. struct sockaddr_in proxy_addr already empty initialized. You WILL need it outside this function, don't lose it!
int handle_args(arguments& arguments, sockaddr_in& proxy_addr, int& socket int argc, char** argv){
    struct argp parser = { options, parse_opt, args_doc, doc };
    //parse arguments
    argp_parse (&argp, argc, argv, 0, 0, &arguments);
    //error checking; need to check if all params fit in the ranges
    if(arg_valid(arguments)){
    	fprintf(stderr, "FATAL ERROR: At least one chance, delay time, or port is outside the acceptable range! Type --help for more info.");
        cleanup();
        exit(EXIT_FAILURE);
    };
    //attempt to create sockets
    setup_socket(&socket);
    //attempt to start listening on the proxy's IP address through the port copied from the server
    bind_socket(&proxy_addr, &socket, arguments->proxy_ip, arguments->port);
}

//bad news - cleanup necessitates global variables somehow?
void cleanup() {
    //closing proxy socket
    if (proxy_socket >= 0) {
        close(proxy_socket);
        printf("Server socket closed.\n");
    }
    //closing packet threads
    printf("Closing %d threads.\n", num_threads);
    for(int t = 0; t < num_threads; ++t){
    	pthread_cancel(packet_threads[t]);
    }
}
