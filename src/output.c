#include "ft_ping.h"

void header()
{
    //printf("FT_PING %s (%s): %d data bytes", host, host_addr, size);
}

void statistics()
{
    //printf("--- %s ft_ping statistics ---", host);
}

void error(char* err)
{
    fprintf(stderr, "%s\n", err);
}

void fatal_error(char* err)
{
    fprintf(stderr, "%s\n", err);
    exit(EXIT_FAILURE);
}