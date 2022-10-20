#include "ft_ping.h"

void prompt()
{
    printf(PROMPT, g.host.name, g.host.addr, g.params.packet_size, g.params.packet_size + 28);
    // The number in parenthesis represents the ping bytes sent including 28 bytes of the header packet.
}

void seq_info(struct msghdr *hdr, t_sequence *seq)
{
    if (g.params.timestamp)
		printf("[%ld.%ld] ", seq->end.tv_sec, seq->end.tv_usec);

	if (ft_strcmp((char *)hdr->msg_name, g.host.addr) == 0)
		printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.2lf ms\n",
		seq->nbytes_received, g.host.addr, g.stats.seq, g.params.ttl, seq->rtt);
	else
		printf("%ld bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2lf ms\n",
		seq->nbytes_received, (char *)hdr->msg_name, g.host.addr, g.stats.seq, g.params.ttl, seq->rtt);
}

void statistics()
{
    int success_rate;
    unsigned long time;
    double avg;
    double savg;
    double mdev;

    success_rate = (int)((g.stats.sent - g.stats.received) * 100 / g.stats.received);

    time = ((g.stats.end.tv_sec * 1000) + (g.stats.end.tv_usec / 1000))
	- ((g.stats.start.tv_sec * 1000) + (g.stats.start.tv_usec / 1000));

    avg = g.stats.sum / g.stats.received;
    savg = g.stats.ssum / g.stats.received;
    mdev = sqrt(savg - (avg * avg));

    printf(STATS_1, g.host.name);
    printf(STATS_2, g.stats.sent, g.stats.received, success_rate, time);
    printf(STATS_3, g.stats.min, avg, g.stats.max, mdev);
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