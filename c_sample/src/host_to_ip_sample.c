#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/socket.h>
#include "sys/types.h"
#include <netinet/in.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ctype.h>
#include "sample.h"


#define SET_H_ERRNO(err) ((void)(h_errno = (err)))
#define XDIGIT_TO_NUM(h) ((h) < 'A' ? (h) - '0' : c_toupper (h) - 'A' + 10)
#define X2DIGITS_TO_NUM(h1, h2) ((XDIGIT_TO_NUM (h1) << 4) + XDIGIT_TO_NUM (h2))
#define c_isxdigit(x) isxdigit (x)
#define c_toupper(x) toupper (x)
#define xzero(x) memset (&(x), '\0', sizeof (x))
#define IP_INADDR_DATA(x) ((void *) &(x)->data)
#define xmalloc checking_malloc
#define xmalloc0 checking_malloc0
#define xrealloc checking_realloc
#define xstrdup checking_strdup
#define xfree checking_free
#define xnew(type) (xmalloc (sizeof (type)))
#define xnew0(type) (xmalloc0 (sizeof (type)))
#define xnew_array(type, len) (xmalloc ((len) * sizeof (type)))
#define xnew0_array(type, len) (xmalloc0 ((len) * sizeof (type)))

struct ghbnwt_context {
	const char *host_name;
	struct hostent *hptr;
};

enum {
	LH_SILENT = 1,
	LH_BIND = 2,
	LH_REFRESH = 4
};

typedef struct {
	int family;
	union {
		struct in_addr d4; /* IPv4 address */
		struct in6_addr d6; /* IPv6 address */
	} data;
	int ipv6_scope;
} ip_address;

struct gaiwt_context {
	const char *node;
	const char *service;
	const struct addrinfo *hints;
	struct addrinfo **res;
	int exit_code;
};

struct address_list {
	int count; /* number of adrresses */
	ip_address *addresses; /* pointer to the string of addresses */
	int faulty; /* number of addresses known not to work. */
	int connected; /* whether we were able to connect to
	one of the addresses in the list,
	at least once. */
	int refcount; /* reference count; when it drops to
	0, the entry is freed. */
};

enum log_options { LOG_VERBOSE, LOG_NOTQUIET, LOG_NONVERBOSE, LOG_ALWAYS };


void *checking_malloc0 (size_t size)
{
	void *ptr = calloc (1, size);
	return ptr;
}

void *checking_malloc (size_t size)
{
	void *ptr = malloc (size);
	return ptr;
}




char *print_address(const ip_address *addr)
{
	return inet_ntoa (addr->data.d4);
}

int run_with_timeout (double timeout, void (*fun) (void *), void *arg)
{
	fun (arg);
	return FALSE;
}

static struct address_list *address_list_from_addrinfo (const struct addrinfo *ai)
{
	struct address_list *al;
	const struct addrinfo *ptr;
	int cnt;
	ip_address *ip;

	cnt = 0;
	for (ptr = ai; ptr != NULL ; ptr = ptr->ai_next)
		if (ptr->ai_family == AF_INET || ptr->ai_family == AF_INET6)
	++cnt;
	if (cnt == 0)
		return NULL;

	al = xnew0 (struct address_list);
	al->addresses = xnew_array (ip_address, cnt);
	al->count = cnt;
	al->refcount = 1;

	ip = al->addresses;
	for (ptr = ai; ptr != NULL; ptr = ptr->ai_next)
	if (ptr->ai_family == AF_INET6)
	{
		const struct sockaddr_in6 *sin6 =
		(const struct sockaddr_in6 *)ptr->ai_addr;
		ip->family = AF_INET6;
		ip->data.d6 = sin6->sin6_addr;
		ip->ipv6_scope = sin6->sin6_scope_id;
		++ip;
	}
	else if (ptr->ai_family == AF_INET)
	{
		const struct sockaddr_in *sin =
		(const struct sockaddr_in *)ptr->ai_addr;
		ip->family = AF_INET;
		ip->data.d4 = sin->sin_addr;
		++ip;
	}
	assert (ip - al->addresses == cnt);
	return al;
}

static void
getaddrinfo_with_timeout_callback (void *arg)
{
	struct gaiwt_context *ctx = (struct gaiwt_context *)arg;
	ctx->exit_code = getaddrinfo (ctx->node, ctx->service, ctx->hints, ctx->res);
}

static int
getaddrinfo_with_timeout (const char *node, const char *service,
const struct addrinfo *hints, struct addrinfo **res,
double timeout)
{
	struct gaiwt_context ctx;
	ctx.node = node;
	ctx.service = service;
	ctx.hints = hints;
	ctx.res = res;

	if (run_with_timeout (timeout, getaddrinfo_with_timeout_callback, &ctx))
	{
		errno = ETIMEDOUT;
		return EAI_SYSTEM;
	}
	return ctx.exit_code;
}


static int is_valid_ipv4_address (const char *str, const char *end)
{
	int saw_digit = FALSE;
	int octets = 0;
	int val = 0;

	while (str < end)
	{
		int ch = *str++;

		if (ch >= '0' && ch <= '9')
		{
			val = val * 10 + (ch - '0');

			if (val > 255)
			return FALSE;
			if (!saw_digit)
			{
				if (++octets > 4)
				return FALSE;
				saw_digit = TRUE;
			}
		}
		else if (ch == '.' && saw_digit)
		{
			if (octets == 4)
				return FALSE;
			val = 0;
			saw_digit = FALSE;
		}
		else
			return FALSE;
	}
	if (octets < 4)
		return FALSE;

	return TRUE;
}

int is_valid_ipv6_address (const char *str, const char *end)
	{
		/* Use lower-case for these to avoid clash with system headers. */
	enum {
	ns_inaddrsz = 4,
	ns_in6addrsz = 16,
	ns_int16sz = 2
	};

	const char *curtok;
	int tp;
	const char *colonp;
	int saw_xdigit;
	unsigned int val;

	tp = 0;
	colonp = NULL;

	if (str == end)
		return FALSE;

	/* Leading :: requires some special handling. */
	if (*str == ':')
	{
		++str;
		if (str == end || *str != ':')
		return FALSE;
	}

	curtok = str;
	saw_xdigit = FALSE;
	val = 0;

	while (str < end)
	{
		int ch = *str++;

		/* if ch is a number, add it to val. */
		if (c_isxdigit (ch))
		{
			val <<= 4;
			val |= XDIGIT_TO_NUM (ch);
			if (val > 0xffff)
				return FALSE;
			saw_xdigit = TRUE;
			continue;
		}

		/* if ch is a colon ... */
		if (ch == ':')
		{
			curtok = str;
			if (!saw_xdigit)
			{
				if (colonp != NULL)
					return FALSE;
				colonp = str + tp;
				continue;
			}
			else if (str == end)
				return FALSE;
			if (tp > ns_in6addrsz - ns_int16sz)
				return FALSE;
			tp += ns_int16sz;
			saw_xdigit = FALSE;
			val = 0;
			continue;
		}

		/* if ch is a dot ... */
		if (ch == '.' && (tp <= ns_in6addrsz - ns_inaddrsz)
		&& is_valid_ipv4_address (curtok, end) == 1)
		{
			tp += ns_inaddrsz;
			saw_xdigit = FALSE;
			break;
		}

		return FALSE;
	}

	if (saw_xdigit)
	{
		if (tp > ns_in6addrsz - ns_int16sz)
		return FALSE;
		tp += ns_int16sz;
	}

	if (colonp != NULL)
	{
		if (tp == ns_in6addrsz)
		return FALSE;
		tp = ns_in6addrsz;
	}

	if (tp != ns_in6addrsz)
		return FALSE;

	return TRUE;
}

int get_host_ip(char *host, char *ip)
{
	int bRet = FALSE;
	struct addrinfo hints, *res;
	struct address_list *al;
	double timeout = 0;
	int flags = 0;
	int silent = !!(flags & LH_SILENT);
	int numeric_address = FALSE;
	int err;

	const char *end = host + strlen (host);
	if (is_valid_ipv4_address (host, end) || is_valid_ipv6_address (host, end))
		numeric_address = TRUE;

	xzero (hints);
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_UNSPEC;

	err = getaddrinfo_with_timeout (host, NULL, &hints, &res, timeout);

	if (err != 0 || res == NULL)
		goto GET_HOST_IP_ERROR;

	al = address_list_from_addrinfo (res);
	freeaddrinfo (res);

	if (!al)
		goto GET_HOST_IP_ERROR;

	if (!silent && !numeric_address)
	{
		char *pTmp = print_address (al->addresses);
		//printf("pTmp : %s\n", pTmp);
		strcpy(ip, pTmp);
		bRet = TRUE;
	}

GET_HOST_IP_ERROR:

	if (al)
		free(al);

	return bRet;
}

#ifndef MAKE_LIBRARY_SAMPLE
int main(int argc, char *argv[])
{
	char pTmp[100];
	get_host_ip("www.google.com", pTmp);
	printf("google ip : %s\n", pTmp);
	return 0;
}
#endif /* MAKE_LIBRARY_SAMPLE */

