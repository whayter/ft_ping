#ifndef FT_PING_H
#define FT_PING_H

#include <stdlib.h>     // exit
#include <stdio.h>      // strerror / printf family
#include <unistd.h>     // alarm / getpid / getuid
#include <signal.h>     // signal
#include <arpa/inet.h>  // inet_ntop / inet_pton
#include <sys/time.h>   // gettimeofday
#include <sys/socket.h> // socket / setsockopt / sendto / recvmsg / getaddrinfo / freeaddrinfo / gai_strerror
#include <sys/types.h>  // getaddrinfo / freeaddrinfo / gai_strerror
#include <netdb.h>      // getaddrinfo / freeaddrinfo / gai_strerror

/* Error messages */

#define OPERATION_NOT_PERMITTED		"ft_ping: icmp open socket: Operation not permitted"
#define SOCKET						"ft_ping: error: could not open socket"
#define USAGE						"usage: ft_ping [-v] [-h] destination"




#define INVALID_OPTION				"ft_ping: invalid option -- %s\n%s"
#define OPTION_PARAM				"ft_ping: option requires an argument -- %s\n%s"

/* Structures */


typedef struct			s_recipient
{
	char*				name;
	struct addrinfo*	ai;
}						t_recipient;

typedef struct			s_sig
{
	int sigint;
	int sigalrm;
}						t_sig;


typedef struct			s_ping
{
	int flags;
	int ttl;


	t_recipient recipient;

	t_sig sig;


	int verbose;

	pid_t pid;

	int sockfd;

	

}				t_ping;

/* Global variables */

t_ping g_ping;

/* Function prototypes from output.c */

void header();
void statistics();
void error(char* err);
void fatal_error(char* err);

/* Function prototypes from utilities.c */

size_t ft_strlen(char* s);
int ft_strcmp(const char *s1, const char *s2);
void *ft_memset(void *b, int c, size_t len);

#endif