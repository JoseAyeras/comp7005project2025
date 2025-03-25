// Keys for options without short options.
#define OPT_PROXY_IP 1
#define OPT_SERVER_IP 2
#define OPT_PORT 3
#define OPT_

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

        break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
}
