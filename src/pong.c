#include "ft_ping.h"



/* TMP */

char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen)
{
    switch(sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr),
                    s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr),
                    s, maxlen);
            break;

        default:
            strncpy(s, "Unknown AF", maxlen);
            return NULL;
    }

    return s;
}

/* END TMP */

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

void unpack_packet(t_sequence *seq)
{
	
}

int receive_echo_reply(t_sequence *seq)				// tmp name
{
	struct msghdr mhdr;
	struct iovec iov;
	char buf[g.params.packet_size];
	struct sockaddr_in in;

	ft_memset(&mhdr, 0, sizeof(mhdr));
	iov.iov_base = buf;
	iov.iov_len = sizeof(buf);
	mhdr.msg_iov = &iov;
	mhdr.msg_iovlen = 1;
	//mhdr.msg_name = g.host.name;

	mhdr.msg_name = &in;
	mhdr.msg_namelen = sizeof(in);

	if ((seq->nbytes_received = recvmsg(g.socket.fd, &mhdr, 0)) <= 0)
	{
		fprintf(stderr, "ft_ping: recvmsg: %s\n", strerror(errno));
		return (-1);
	}

	char source_ip[INET6_ADDRSTRLEN];
	inet_ntop(AF_INET, &in.sin_addr, ip, INET6_ADDRSTRLEN);

	if (ft_strcmp(source_ip, g.host.addr) != 0)
	{
		g.stats.errors++;
		printf("from %s icmp_seq=%d Destination Net Unreachable\n")
	}

	printf("ip: %s\n", ip);

	update_stats(seq);
	seq_info(&mhdr, seq);
	return (0);

}
