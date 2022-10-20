#include "ft_ping.h"

void prompt()
{
    printf(BANNER, g.host.name, g.host.addr, g.params.packet_size, g.params.packet_size + 28);
    // The number in parenthesis represents the ping bytes sent including 28 bytes of the header packet.
    // not quite sure about the way to compute the size here
}

void seq_info(t_sequence *seq)
{
    if (g.params.timestamp)
		printf("[%ld.%ld] ", seq->end.tv_sec, seq->end.tv_usec);
    if (ft_strcmp(g.host.name, g.host.addr) == 0)
		printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.2lf ms\n",
		seq->nbytes_received, g.host.addr, g.stats.seq, g.params.ttl, seq->rtt);
	else
		printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2lf ms\n",
		seq->nbytes_received, g.host.name, g.host.addr, g.stats.seq, g.params.ttl, seq->rtt);
}

void statistics()
{
    int failure_rate;
    unsigned long time;
    double avg;
    double savg;
    double mdev;

    if (g.stats.received == 0 && g.stats.sent > 0)
        failure_rate = 100;
    else if (g.stats.received == 0 && g.stats.sent == 0)        // utile ?
        failure_rate = 0;
    else
        failure_rate = (int)((g.stats.sent - g.stats.received) * 100 / g.stats.received);

    time = ((g.stats.end.tv_sec * 1000) + (g.stats.end.tv_usec / 1000))
	- ((g.stats.start.tv_sec * 1000) + (g.stats.start.tv_usec / 1000));

    printf(STATS_BANNER, g.host.name);

    if (!g.stats.errors)
    {
        avg = g.stats.sum / g.stats.received;
        savg = g.stats.ssum / g.stats.received;
        mdev = sqrt(savg - (avg * avg));

        printf(STATS, g.stats.sent, g.stats.received, failure_rate, time);
        printf(STATS_RTT, g.stats.min, avg, g.stats.max, mdev);
    }
    else
    {
        printf(STATS_ERR, g.stats.sent, g.stats.received, g.stats.errors, failure_rate, time);
    }
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