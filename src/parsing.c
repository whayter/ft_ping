#include "ft_ping.h"

/*
** -h option
*/

static void help()
{
	printf("%s", USAGE);
	exit(EXIT_FAILURE);
}

/*
** -c [count] option
*/

static int set_count(int i, int ac, char **args)
{
	if (i + 1 == ac)
	{
		fprintf(stderr, OPTION_PARAM_MISSING, args[i][1], USAGE);
		exit(EXIT_FAILURE);
	}
	g.params.count = ft_atoi(args[++i]);
	if (g.params.count == 0)
	{
		fprintf(stderr, BAD_COUNT);
		exit(EXIT_FAILURE);
	}
	return (1);
}

/*
** -i [interval] option
*/

static int set_interval(int i, int ac, char **args)
{
	if (i + 1 == ac)
	{
		fprintf(stderr, OPTION_PARAM_MISSING, args[i][1], USAGE);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(args[++i], "0") == 0)
		g.params.interval = 0;
	else
	{
		g.params.interval = ft_atoi(args[i]);
		if (g.params.interval == 0)
		{
			fprintf(stderr, BAD_INTERVAL);
			exit(EXIT_FAILURE);
		}
	}
	return (1);
}

/*
** -t [time-to-live] option
*/

static int set_ttl(int i, int ac, char **args)
{
	if (i + 1 == ac)
	{
		fprintf(stderr, OPTION_PARAM_MISSING, args[i][1], USAGE);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(args[++i], "0") == 0 || (g.params.ttl = ft_atoi(args[i])) == 0)
	{
		fprintf(stderr, BAD_TTL);
		exit(EXIT_FAILURE);
	}
	if (g.params.ttl > 255)
	{
		fprintf(stderr, OUT_OF_RANGE_TTL, g.params.ttl);
		exit(EXIT_FAILURE);
	}
	return (1);
}

/*
** -V option: show version and exit
*/

static void version()
{
	printf("%s", VERSION);
	exit(EXIT_SUCCESS);
}

/*
** bad option
*/

static void bad_option(char opt)
{
	fprintf(stderr, INVALID_OPTION, opt, USAGE);
	exit(EXIT_FAILURE);
}

/*
** Get addrinfo from host name or ip address
*/

static void get_addr(char *hostname)
{
	struct addrinfo hints;
	struct addrinfo* res;
	int ret;

	g.host.name = hostname;
	ft_memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; // AF_UNSPEC
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	ret = getaddrinfo(g.host.name, NULL, &hints, &res);
	if (ret != 0)
	{
		fprintf(stderr, UNKNOWN_HOST, g.host.name);
		exit(EXIT_FAILURE);
	}
	ft_memcpy(&g.host.sockaddr, res->ai_addr, sizeof(struct sockaddr_in));
	inet_ntop(AF_INET, &g.host.sockaddr.sin_addr, g.host.addr, INET6_ADDRSTRLEN);
	freeaddrinfo(res);
}

/*
** Parameters parsing hub
*/

void parse_args(int ac, char **args)
{
	int i = 0;
	
	while (++i < ac)
	{
		if (ft_strcmp(args[i], "-h") == 0)
			help();
		else if (ft_strcmp(args[i], "-v") == 0)
			g.params.verbose = 1;
		else if (ft_strcmp(args[i], "-c") == 0)
			i += set_count(i, ac, args);
		else if (ft_strcmp(args[i], "-D") == 0)
			g.params.timestamp = 1;
		else if (ft_strcmp(args[i], "-i") == 0)
			i += set_interval(i, ac, args);
		else if (ft_strcmp(args[i], "-q") == 0)
			g.params.quiet = 1;
		else if (ft_strcmp(args[i], "-t") == 0)
			i += set_ttl(i, ac, args);
		else if (ft_strcmp(args[i], "-V") == 0)
			version();
		else if (args[i][0] == '-' && args[i][1] != '\0')
			bad_option(args[i][1]);
		else if (i + 1 == ac)
			return (get_addr(args[i]));
	}
	help();
}