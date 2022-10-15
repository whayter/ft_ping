#include "ft_ping.h"

void sig_handler(int signum)
{
	switch (signum)
	{
		case SIGINT:
			g.sig.sigint = 0;
			break ;
		case SIGALRM:
			g.sig.sigalrm = 1;
			break ;
		default:
			return ;
	}
}

void init()
{
	g.params.ttl = 64;
	g.params.packet_size = 56;
	g.params.timeout.tv_sec = 1;
	g.params.timeout.tv_usec = 0;
	g.params.verbose = 0;

	g.sig.sigint = 1;
	g.sig.sigalrm = 0;

	g.stats.sent = 0;
	g.stats.received = 0;
}

void get_addr()
{
	struct addrinfo hints;
	struct addrinfo* res;
	struct sockaddr_in* sockaddr;
	int ret;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC; // AF_INET
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	ret = getaddrinfo(g.recipient.name, NULL, &hints, &res);
	if (ret != 0)
	{
		fprintf(stderr, UNKNOWN_HOST, g.recipient.name);
		exit(EXIT_FAILURE);
	}
	sockaddr = (struct sockaddr_in *)res->ai_addr;
	inet_ntop(AF_INET, &sockaddr->sin_addr, g.recipient.addr, INET6_ADDRSTRLEN);
	freeaddrinfo(res);
}

void parse_args(char** args)
{
	int i = 0;
	
	while (args[++i])
	{
		if (ft_strcmp(args[i], "-h") == 0)
		{
			printf("%s", USAGE);
			exit(2);
		}
		else if (ft_strcmp(args[i], "-v") == 0)
			g.params.verbose = 1;
		else if (args[i][0] == '-' && args[i][1] != '\0')
		{
			fprintf(stderr, INVALID_OPTION, args[i][1], USAGE);
			exit(EXIT_FAILURE);
		}
		else
		{
			g.recipient.name = args[i];
			get_addr();
			return ;
		}
	}
}

void ping()
{
	//struct timeval start;

	int seq = 1;

	gettimeofday(&g.stats.start, NULL);
	prompt();

	while (g.sig.sigint)
	{
		//gettimeofday(&start, NULL);

		struct icmphdr pckt_hdr;
		char pckt_data[g.params.packet_size];

		ft_memset(&pckt_hdr, 0, sizeof(pckt_hdr));
		ft_memset(&pckt_data, 0, g.params.packet_size);

		pckt_hdr.type = ICMP_ECHO;
		pckt_hdr.un.echo.id = getpid();

		(void)pckt_data;
		(void)pckt_hdr;
		


		//int r = sendto(g.sockfd, &pckt, sizeof(pckt), 0);

		seq++;
	}

	gettimeofday(&g.stats.end, NULL);

	statistics();
}


// need to review error messages
void open_socket()
{
	if ((g.socket.fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		fatal_error(SOCKET);

	if (setsockopt(g.socket.fd, IPPROTO_IP, IP_TTL,
	&g.params.ttl, sizeof(g.params.ttl)) != 0)
		fatal_error(SOCKET);

	if (setsockopt(g.socket.fd, SOL_SOCKET, SO_RCVTIMEO,
	(const char*)&g.params.timeout, sizeof g.params.timeout) != 0)
		fatal_error(SOCKET);
}

int main(int ac, char** av)
{
	if (getuid() != 0)
		fatal_error(OPERATION_NOT_PERMITTED);
	if (ac < 2)
		fatal_error(USAGE);
	init();
	parse_args(av);
	open_socket();

	signal(SIGINT, sig_handler);
	signal(SIGALRM, sig_handler);

	ping();

	close(g.socket.fd);
	return (0);
}