#include "ft_ping.h"

/*
** compute Internet Checksum for 'count' bytes,
** beginning at location 'addr', as defined in RFC 1071.
*/

static unsigned short checksum(void *addr, int count)
{
	unsigned int sum = 0;
	unsigned short *buffer = addr;

	while (count > 1) 
	{
		sum += *buffer++;
		count -= 2;
	}
	if (count > 0)
		sum += *(unsigned char *)addr;

	while (sum >> 16)
		sum = (sum & 0xffff) + (sum >> 16);
	return (~sum);
}

/*
** set up an icmp packet to send an echo request to the host
*/

int send_echo_request(t_sequence *seq)
{
	ft_memset(&seq->pckt, 0, sizeof(seq->pckt));
	seq->pckt.hdr.un.echo.id = getpid();
	seq->pckt.hdr.un.echo.sequence = g.stats.seq++;
	seq->pckt.hdr.type = ICMP_ECHO;
	seq->pckt.hdr.code = 0;
	seq->pckt.hdr.checksum = checksum(&seq->pckt, sizeof(seq->pckt));

	seq->nbytes_sent = sendto(g.socket.fd, &seq->pckt, sizeof(seq->pckt), 0,
	(struct sockaddr *)&g.host.sockaddr, sizeof(g.host.sockaddr));

	if (seq->nbytes_sent <= 0)
	{
		fprintf(stderr, "ft_ping: sendto: %s\n", strerror(errno));
		return (-1);
	}
	gettimeofday(&seq->start, NULL);
	alarm(g.params.interval);
	g.sig.sigalrm = 1;
	g.stats.sent++;
	return (0);
}