#ifndef FT_PING_H
#define FT_PING_H

/* Library includes */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
//#include <netinet/ip_icmp.h>
#include <linux/icmp.h>

/* Error messages */

#define USAGE						"Usage: sudo ft_ping [-v] [-h] destination\n"

#define OPERATION_NOT_PERMITTED		"ft_ping: icmp open socket: Operation not permitted\n"
#define SOCKET						"ft_ping: socket: Failed to create raw socket\n"

#define UNKNOWN_HOST				"ft_ping: unknown host %s\n"
#define INVALID_OPTION				"ft_ping: invalid option -- '%c'\n%s"
#define OPTION_PARAM_MISSING		"ft_ping: option requires an argument -- '%c'\n%s"

/* Structures */

typedef struct s_params
{
	int flags;
	int ttl;
	int packet_size;
	int verbose;
	struct timeval timeout;
} t_params;

typedef struct s_socket
{
	int fd;
	struct sockaddr_in addr;
} t_socket;

typedef struct s_recipient
{
	char* name;
	char addr[INET6_ADDRSTRLEN];
} t_recipient;

typedef struct s_sig
{
	int sigint;
	int	sigalrm;
} t_sig;

typedef struct s_stats
{
	int sent;
	int received;
	struct timeval start;
	struct timeval end;
} t_stats;

typedef struct s_ping
{
	t_params params;
	t_socket socket;
	t_recipient recipient;
	t_sig sig;
	t_stats stats;
} t_ping;

/* Global variables */

t_ping g;

/* Function prototypes from output.c */

void prompt();
void statistics();
void error(char* err);
void fatal_error(char* err);

/* Function prototypes from utilities.c */

size_t ft_strlen(char* s);
int ft_strcmp(const char *s1, const char *s2);
void *ft_memset(void *b, int c, size_t len);

#endif
