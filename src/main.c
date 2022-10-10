#include "ft_ping.h"

void sig_handler(int signum)
{
	switch (signum)
	{
		case SIGINT:
			g_ping.sig.sigint = 0;

		case SIGALRM:
			g_ping.sig.sigalrm = 1;
	}
}

void init()
{
	g_ping.ttl = 64;
	g_ping.pid = getpid();
	g_ping.sig.sigint = 1;
	g_ping.sig.sigalrm = 0;

	g_ping.verbose = 0;
}


void get_addr(char* destination)
{
	struct addrinfo hints;
	int r;

	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;			// use AF_UNSPEC in order to allow IPv4 or IPv6
	hints.ai_socktype = SOCK_RAW;
	hints.ai_flags = 0;
	hints.ai_protocol = IPPROTO_ICMP;
	r = getaddrinfo(destination, NULL, &hints, &g_ping.recipient.ai);
	if (r != 0)
	{
		fprintf(stderr, "ft_ping: getaddrinfo error: %s\n", gai_strerror(r));
		exit(EXIT_FAILURE);
	}
}


void parse_args(char** args)
{
	int i = 0;
	
	while (args[++i])
	{
		if (ft_strcmp(args[i], "-h"))
		{
			printf("%s\n", USAGE);
			exit(2);
		}
		else if (ft_strcmp(args[i], "-v"))
			g_ping.verbose = 1;
		else
		{
			g_ping.recipient.name = args[i];
			get_addr(args[i]);
		}
	}
}

int main(int ac, char** av)
{
	if (getuid() != 0)
		fatal_error(OPERATION_NOT_PERMITTED);
	if (ac < 2)
		fatal_error(USAGE);
	init();
	parse_args(av);
	if ((g_ping.sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == -1)
		fatal_error(SOCKET);
	signal(SIGINT, sig_handler);
	signal(SIGALRM, sig_handler);

	freeaddrinfo(g_ping.recipient.ai);
	close(g_ping.sockfd);
	return (0);
}