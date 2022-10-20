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
	if ((g.socket.fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		fatal_error(SOCKET);
	if (setsockopt(g.socket.fd, IPPROTO_IP, IP_TTL, &g.params.ttl, sizeof(g.params.ttl)) != 0)
		fatal_error(SOCKET);
	if (setsockopt(g.socket.fd, SOL_SOCKET, SO_RCVTIMEO, &g.params.timeout, sizeof(g.params.timeout)) != 0)
		fatal_error(SOCKET);
}

int main(int ac, char **av)
{
	if (getuid() != 0)
		fatal_error(OPERATION_NOT_PERMITTED);
	if (ac < 2)
		fatal_error(USAGE);
	init();
	parse_args(ac, av);
	open_socket();
	signal(SIGINT, sig_handler);
	signal(SIGALRM, sig_handler);

	prompt();
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
	statistics();

	close(g.socket.fd);
	return (0);
}