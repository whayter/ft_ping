#include "ft_ping.h"

t_ping g;

/*
** Signals handler function
*/

void sig_handler(int signum)
{
	switch (signum)
	{
		case SIGINT:
		{
			g.sig.sigint = 1;
			break ;
		}
		case SIGALRM:
		{
			g.sig.sigalrm = 0;
			break ;
		}
		default:
			break ;
	}
}

/*
** Global variable initialization
*/

static void init()
{
	/* params */

	g.params.count = -1;
	g.params.timestamp = 0;
	g.params.interval = 1;
	g.params.quiet = 0;
	g.params.ttl = 64;
	g.params.packet_size = 56;
	g.params.timeout.tv_sec = 1;
	g.params.timeout.tv_usec = 0;
	g.params.verbose = 0;

	/* signal */

	g.sig.sigint = 0;
	g.sig.sigalrm = 0;

	/* stats */

	g.stats.seq = 0;
	g.stats.sent = 0;
	g.stats.received = 0;
	g.stats.errors = 0;
	g.stats.min = DBL_MAX;
	g.stats.max = 0.0;
	g.stats.sum = 0.0;
	g.stats.ssum = 0.0;

	/* host */

	g.host.name = NULL;
}

/*
** Create raw socket
** need to review error messages !
*/

void open_socket()
{
	g.socket.fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (g.socket.fd == -1)
	{
		fprintf(stderr, SOCKET);
		exit(EXIT_FAILURE);
	}
	setsockopt(g.socket.fd, IPPROTO_IP, IP_TTL,
	&g.params.ttl, sizeof(g.params.ttl));
	setsockopt(g.socket.fd, SOL_SOCKET, SO_RCVTIMEO,
	&g.params.timeout, sizeof(g.params.timeout));

	//test
	int yes = 1;
	setsockopt(g.socket.fd, IPPROTO_IP, IP_RECVTTL, &yes, sizeof(yes));
}

/*
** Main function
*/

int main(int ac, char **av)
{
	if (getuid() != 0)
	{
		fprintf(stderr, OPERATION_NOT_PERMITTED);
		exit(EXIT_FAILURE);
	}
	if (ac < 2)
	{
		fprintf(stderr, USAGE);
		exit(EXIT_FAILURE);
	}
	init();
	parse_args(ac, av);
	open_socket();
	signal(SIGINT, sig_handler);
	signal(SIGALRM, sig_handler);

	display_global_banner();
	gettimeofday(&g.stats.start, NULL);
	while (!g.sig.sigint)
	{
		t_sequence seq;
		if (!g.sig.sigalrm || g.params.interval == 0)
		{
			if (send_echo_request(&seq) == 0)
				receive_echo_reply(&seq);

			if (g.params.count > 0 && --g.params.count == 0)
				break ;
		}
	}
	gettimeofday(&g.stats.end, NULL);
	display_statistics();

	close(g.socket.fd);
	return (0);
}