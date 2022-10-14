#include "ft_ping.h"

void prompt()
{
    printf("PING %s (%s): %d(%d) bytes of data.\n",
    g.recipient.name, g.recipient.addr,
    g.params.packet_size, g.params.packet_size + 28);
    // The number in parenthesis represents the ping bytes sent including 28 bytes of the header packet.
}

void statistics()
{
    int success_rate;
    unsigned long time;

    printf("\n--- %s ft_ping statistics ---\n", g.recipient.name);

    success_rate = g.stats.sent > 0 ?
    ((g.stats.sent - g.stats.received) / g.stats.sent) * 100 : 0;

    time = ((g.stats.end.tv_sec * 1000) + (g.stats.end.tv_usec / 1000))
	- ((g.stats.start.tv_sec * 1000) + (g.stats.start.tv_usec / 1000));

    printf("%d packets transmitted, %d received, %d%% packet loss, time %lums\n",
    g.stats.sent, g.stats.received, success_rate, time);

    //printf("rtt min/avg/max/mdev = %f/%f/%f/%f\n", );
}

void error(char* err)
{
    fprintf(stderr, "%s", err);
}

void fatal_error(char* err)
{
    fprintf(stderr, "%s", err);
    exit(EXIT_FAILURE);
}