
#include <argp.h>	//needed for arg parsing
#include <stdio.h>
#include <stdlib.h>	//needed for float params


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
    
    ,OPT_CDRP	//C     server delay time
    ,OPT_CDLY	//D     client delay time
    ,OPT_CTMN	//E     delay time
    ,OPT_CTMX   //F     port
    
    ,OPT_CDRP	//10    server ip address
    ,OPT_CDLY	//11    proxy ip address
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



int handle_args(int argc, char** argv){
    struct argp parser = { options, parse_opt, args_doc, doc };
    struct arguments arguments;
    
}
