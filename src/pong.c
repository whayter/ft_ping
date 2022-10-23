#include "ft_ping.h"

void update_stats(t_sequence *seq)
{
	gettimeofday(&seq->end, NULL);
	g.stats.received++;

	seq->rtt = (seq->end.tv_sec * 1000.0) + (seq->end.tv_usec / 1000.0);
	seq->rtt -= (seq->start.tv_sec * 1000.0) + (seq->start.tv_usec / 1000.0);

	if (seq->rtt < g.stats.min)
		g.stats.min = seq->rtt;
	if (seq->rtt > g.stats.max)
		g.stats.max = seq->rtt;

	g.stats.sum += seq->rtt;
	g.stats.ssum += (seq->rtt * seq->rtt);
}

int receive_echo_reply(t_sequence *seq)
{
	struct msghdr mhdr;
	struct cmsghdr *cmsg;
	struct iovec iov;
	char buf[g.params.packet_size];
	struct sockaddr_in in;

	uint8_t ctrlDataBuffer[CMSG_SPACE(sizeof(uint8_t))];

	ft_memset(&mhdr, 0, sizeof(mhdr));
	iov.iov_base = buf;
	iov.iov_len = sizeof(buf);
	mhdr.msg_iov = &iov;
	mhdr.msg_iovlen = 1;
	mhdr.msg_name = &in;
	mhdr.msg_namelen = sizeof(in);
	mhdr.msg_control = &ctrlDataBuffer;
	mhdr.msg_controllen = sizeof(ctrlDataBuffer);

	seq->nbytes_received = recvmsg(g.socket.fd, &mhdr, 0);

	// what should i do here? whould i include 0 ?
	if (seq->nbytes_received < 0)
		return (-1);

	cmsg = CMSG_FIRSTHDR(&mhdr);
	seq->pckt.ttl = *((uint8_t *)CMSG_DATA(cmsg));
	inet_ntop(AF_INET, &in.sin_addr, seq->host_addr, INET6_ADDRSTRLEN);
	if (ft_strcmp(seq->host_addr, g.host.addr) != 0)
	{
		g.stats.errors++;

		// encore un pb : ping -t 3 google.com devrait retourner ttl exceeded
		
		if (seq->pckt.ttl == 255)
			fprintf(stderr, TTL_EXCEEDED, seq->host_addr, g.stats.seq);
		else
			fprintf(stderr, HOST_UNREACHABLE, seq->host_addr, g.stats.seq);
	}
	else
	{
		update_stats(seq);
		if (!g.params.quiet)
			display_sequence_data(seq);
	}
	return (0);
}