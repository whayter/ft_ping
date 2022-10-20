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
#include <netinet/ip_icmp.h>
#include <math.h>

#include <float.h>
#include <string.h>
#include <errno.h>

//#include <sys/types.h>

/* Messages */

#define PROMPT						"PING %s (%s): %d(%d) bytes of data.\n"

#define STATS_1						"\n--- %s ft_ping statistics ---\n"
#define STATS_2						"%d packets transmitted, %d received, %d%% packet loss, time %lums\n"
#define STATS_3						"rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n"

/* Error messages */

#define USAGE						"Usage: sudo ft_ping [-v] [-h] [-c count] [-i interval] destination\n"

#define OPERATION_NOT_PERMITTED		"ft_ping: icmp open socket: Operation not permitted\n"
#define SOCKET						"ft_ping: socket: Failed to create raw socket\n"

#define UNKNOWN_HOST				"ft_ping: unknown host %s\n"
#define INVALID_OPTION				"ft_ping: invalid option -- '%c'\n%s"
#define OPTION_PARAM_MISSING		"ft_ping: option requires an argument -- '%c'\n%s"

#define BAD_COUNT					"ft_ping: bad number of packets to transmit\n"
#define BAD_INTERVAL				"ft_ping: bad timing interval\n"
#define BAD_TTL						"ft_ping: can't set unicast time-to-live: Invalid argument\n"
#define OUT_OF_RANGE_TTL			"ft_ping: ttl %d out of range\n"

#define NO_CONNEXION				"ft_ping: "

/* Structures */

typedef struct s_params
{
	int count;
	int timestamp;
	unsigned int interval;
	int flags;
	unsigned int ttl;
	int packet_size;
	int verbose;
	struct timeval timeout;
} t_params;

typedef struct s_socket
{
	int fd;
	struct sockaddr_in addr;
} t_socket;

typedef struct s_host
{
	char *name;
	char addr[INET6_ADDRSTRLEN];
	struct sockaddr_in sockaddr;
} t_host;

typedef struct s_sig
{
	int sigint;
	int	sigalrm;
} t_sig;

typedef struct s_stats
{
	int seq;
	int sent;
	int received;
	int errors;
	struct timeval start;
	struct timeval end;
	double min;
	double max;
	double sum;
	double ssum;
} t_stats;

typedef struct s_pckt
{
	struct icmphdr hdr;
	char data[56];			// tmp
	//char *data;
} t_pckt;

typedef struct s_ping_pckt
{
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	u_int16_t id;
	u_int16_t sequence;
	char data[56];			// tmp
	//char* data;
} t_ping_pckt;

typedef struct s_sequence
{
	int n;
	struct timeval start;
	struct timeval end;
	double rtt;
	ssize_t nbytes_sent;
	ssize_t nbytes_received;
	t_pckt pckt;
	//t_ping_pckt pckt;
} t_sequence;

typedef struct s_ping
{
	t_params params;
	t_socket socket;
	t_host host;
	t_sig sig;
	t_stats stats;
} t_ping;

/* Global variables */

extern t_ping g;

/* Function prototypes from parsing.c */

void parse_args(int ac, char **args);

/* Function prototypes from ping.c */

int send_echo_request(t_sequence *seq);

/* Function prototypes from pong.c */

int receive_echo_reply(t_sequence *seq);

/* Function prototypes from output.c */

void prompt();
void seq_info(struct msghdr *hdr, t_sequence *seq);
void statistics();
void error(char *err);
void fatal_error(char *err);

/* Function prototypes from utilities.c */

int ft_strcmp(const char *s1, const char *s2);
void *ft_memset(void *b, int c, size_t len);
void *ft_memcpy(void *dst, const void *src, size_t n);
int	ft_isspace(char c);
int ft_atoi(const char *str);

#endif