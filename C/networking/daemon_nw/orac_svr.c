#include <stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <getopt.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <sys/poll.h>

int do_send_recv( int connFd);

uint16_t DEFAULT_PORT;
#define MAX_FDS 32
// 5 min
#define POLL_TIMEOUT_MILLSEC (5*60*1000) 

pthread_t t_handles[MAX_FDS];
// For Each listening Daemon Thread
struct sockaddr_in	d_addr = { 
	    .sin_family = AF_INET, 
    } ;

struct pollfd fds[MAX_FDS];

typedef enum _INFO_HDR_TYPE{
    E_MS_PORT_TABLE_REQUEST,
    E_MS_PORT_TABLE_REPLY,
    E_DMN_WRITE_INVALID,
    E_DMN_WRITE_INVALID_ACK,
    E_DMN_WRITE_COMPLETE,
    E_DMN_WRITE_COMPLETE_ACK,
    E_DMN_HB
} INFO_HDR_TYPE;

typedef struct _DMN_WRITE_INVALID {
    uint64_t lba;
    uint32_t lbc;
    uint64_t volid;
} DMN_WRITE_INVALID;

typedef struct _DMN_WRITE_INVALID_ACK {
    uint64_t lba;
    uint32_t lbc;
    uint64_t volid;
    uint32_t confirm; 
} DMN_WRITE_INVALID_ACK;

typedef struct _DMN_WRITE_COMPLETE {
    uint64_t lba;
    uint32_t lbc;
    uint64_t volid;
    uint32_t confirm; 
} DMN_WRITE_COMPLETE;

typedef struct _DMN_WRITE_COMPLETE_ACK {
    uint64_t lba;
    uint32_t lbc;
    uint64_t volid;
    uint32_t confirm; 
} DMN_WRITE_COMPLETE_ACK;

typedef struct _DMN_HB {
    uint32_t hb_millisec_token; 
} DMN_HB;


typedef struct _INFO_PAYLOAD {
    INFO_HDR_TYPE info_type;
    uint32_t	  data_size;  
    union {
	uint16_t port_numbers[MAX_FDS];
	DMN_WRITE_INVALID;
	DMN_WRITE_INVALID_ACK;
	DMN_WRITE_COMPLETE;
	DMN_WRITE_COMPLETE_ACK;
    };
} INFO_PAYLOAD;


void usage(char *prog)
{
    printf ("%s -p <ms_port> -m <IP> -d <IP> -n <numsockets>\n\n", prog);
    printf ("-m: MS IP\n-d: Daemon IP (all listen(s) are spawned on this IP)\n");
    printf ("-p: MS port number\n-n: number of hosts in cluster\n");
    return;
}
#if 0
int setnonblocking(int s)
{
    int opts;
    opts = fcntl(s, F_GETFL);
    if (opts < 0) {
	perror("fcntl(F_GETFL)");
	return 1;
    }
    opts = (opts | O_NONBLOCK);
    if (fcntl(s, F_SETFL,opts) < 0) {
    	perror("fcntl(F_SETFL): could not set Socket Non Blocking");
	return 1;
    }
    printf ("Socket Set to non-blocking mode \n");
return 0;
}

int add_to_poll(int fd, short event, int index)
{
    fds[index].fd = fd;
    fds[index].events = event; 

    do {
	int nRevents = 0;
	printf("Issued POll... with %d FDs\n", index);
	nRevents = poll(fds, index, POLL_TIMEOUT_MILLSEC);
	if (nRevents < 0) {
	    printf("Error Poll: %s\n", strerror(errno));
	    return 1;
	} else if (nRevents == 0) {
	    continue;
	} else {
	    int i = 0;
	    for (i = 0; i < index; i++) {
		switch(fds[i]) {
		    case POLLIN: {

		    break;
		    }
		    case POLLOUT: {
			break;
		    }

		}
	    }
	}

    } while(1);
    return 0;
}
#endif

void dump_port_seq(uint16_t *p)
{
    int i = 0;
    printf ("\nPort Number list\n");
    for (;i < MAX_FDS; i++) {
	printf ("%u\n ",*p++);
    }
    printf("\n");
}

void write_port_numbers( uint16_t *p)
{
    int i = 0;
    uint16_t *p2 = p;
    uint16_t port = DEFAULT_PORT + 1;
    /* Note we dont check if these port numbers are
     * available. If not the change the DEFAULT_PORT 
     * and try again */
    for (; i<MAX_FDS; i++) {
	*p++ = port + i;
    }
    //dump_port_seq(p2);

}

void *deamon_start_listen (void *arg) 
{
    int s = -1;
    struct sockaddr_in ms_addr = *(struct sockaddr_in *)arg;
    struct sockaddr_in rsa;
    socklen_t rsa_addr_len = -1;
    char laddr_buf[INET_ADDRSTRLEN] = {'\0'};
    char raddr_buf[INET_ADDRSTRLEN] = {'\0'};
    int reuse_addr = 1;	    
    int connFd = -1;
			   
    inet_ntop(AF_INET, &ms_addr.sin_addr, 
			    laddr_buf, INET_ADDRSTRLEN);
    errno = 0;
    if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
	printf("socket(): %s\n", strerror(errno));
    } 
    /* Make this listening socket non-blocking. We intend to use it in
     * a poller */
    //if (setnonblocking(s))
    //	return 0;

    /* So that we can re-bind to it without TIME_WAIT problems */
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, 
					    sizeof(reuse_addr));
    
    errno = 0;
    if (bind(s, (struct sockaddr *) &ms_addr, 
			    sizeof(struct sockaddr_in)) != 0) {
	printf("bind(): %s: %s::%u\n",strerror(errno), 
			    laddr_buf, ntohs(ms_addr.sin_port));
    } 
    

    errno = 0;
    if (listen(s, 64) != 0) {
	printf("listen(): %s\n", strerror(errno));
    } else {
	printf("listening : %s: %s::%u\n",strerror(errno), 
			    laddr_buf, ntohs(ms_addr.sin_port));
    }
	/* This is a blocking FD. We wait for clients to
	 * issue connects. We then accept. 
	 * TODO: Some thread here should issue connects rather 
	 *	than listen say even number threads.
	 */
    errno = 0;
    rsa_addr_len = sizeof(rsa);
    if ((connFd = accept(s, (struct sockaddr *) &rsa, 
					&rsa_addr_len)) != -1) {
	printf("accept(): %s\n",strerror(errno));
    }
    printf ("connected to Remote address: %s port: %u\n",
	    inet_ntop(AF_INET, (void *) &(rsa.sin_addr.s_addr), 
		raddr_buf, INET_ADDRSTRLEN), ntohs(rsa.sin_port)); 
    do {
	do_send_recv(connFd);
    } while (1);
    close(connFd);
    
    if (s != -1) {
	close(s);
    }
    return (void *) NULL;
}

void create_daemon_threads(uint16_t *p)
{ 
    int i = 0;

    for (; i<MAX_FDS; i++) {
	struct sockaddr_in d;	
	memcpy(&d, &d_addr, sizeof(d));
	d.sin_port = htons(p[i]);
        if(pthread_create(&(t_handles[i]), NULL, 
		   deamon_start_listen, &d) != 0) {
	    printf("[%d] Daemon thread creation Error\n",i);
       }
    }
}


int do_send_recv( int connFd) 
{
    INFO_PAYLOAD data;
    ssize_t rs = recv(connFd, &data, sizeof(data), 0);
    if (rs != sizeof(data)) {
	printf("Could not Recv all the data. Only recvd: %zd bytes\n", rs);
    } else {
	switch (data.info_type) {
	    case E_MS_PORT_TABLE_REQUEST: {
		printf("Recvd E_MS_PORT_TABLE_REQUEST : %zd\n",rs);
		data.info_type = E_MS_PORT_TABLE_REPLY;
		// XXX this field though data_size, we are putting the daemon IP 
		// here. Q&D hack. Note its in Nw byte order
		data.data_size = d_addr.sin_addr.s_addr;
		write_port_numbers(data.port_numbers);
		if ((rs = send(connFd, &data, sizeof(data), 0)) != sizeof(data)) {
		    printf("Error sending E_MS_PORT_TABLE_REPLY: %zd\n", rs);
		} else {
		    create_daemon_threads(data.port_numbers);
		}

		break;
	    }
	    case E_MS_PORT_TABLE_REPLY: {
		printf("E_MS_PORT_TABLE_REPLY: %zd\n", rs);
		break;
	    }
	    case E_DMN_WRITE_INVALID: {
		printf("E_DMN_WRITE_INVALID: %zd\n", rs);

		break;
	    }
	    case E_DMN_WRITE_INVALID_ACK: {
		printf("E_DMN_WRITE_INVALID_ACK: %zd\n", rs);

		break;
	    }
	    case E_DMN_WRITE_COMPLETE: {
		printf("E_DMN_WRITE_COMPLETE: %zd\n", rs);
		break;
	    }
	    case E_DMN_WRITE_COMPLETE_ACK: {
		printf("E_DMN_WRITE_COMPLETE_ACK: %zd\n", rs);
		break;
	    }
	    case E_DMN_HB: {
		printf("E_DMN_HB: %zd\n", rs);
		break;
	    }
	    default:
		printf("Error recv: Unknown request :%d\n", data.info_type);
		return 0;
	}

    }
    return 0;
}

void * ms_start_listen (struct sockaddr_in *arg) 
{
    int s = -1;
    struct sockaddr_in ms_addr = *arg;
    struct sockaddr_in rsa;
    socklen_t rsa_addr_len = -1;
    char laddr_buf[INET_ADDRSTRLEN] = {'\0'};
    char raddr_buf[INET_ADDRSTRLEN] = {'\0'};
    int reuse_addr = 1;	    
			   
    inet_ntop(AF_INET, &ms_addr.sin_addr, 
			    laddr_buf, INET_ADDRSTRLEN);
    errno = 0;
    if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
	printf("socket(): %s\n", strerror(errno));
    } 
    /* Make this listening socket non-blocking. We intend to use it in
     * a poller */
    //if (setnonblocking(s))
    //	return 0;

    /* So that we can re-bind to it without TIME_WAIT problems */
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, sizeof(reuse_addr));

    errno = 0;
    if (bind(s, (struct sockaddr *) &ms_addr, 
			    sizeof(struct sockaddr_in)) != 0) {
	printf("bind(): %s: %s::%u\n",strerror(errno), 
			    laddr_buf, ntohs(ms_addr.sin_port));
    } 
    errno = 0;
    if (listen(s, 5) != 0) {
	printf("listen(): %s\n", strerror(errno));
    } 
    for (;;) {
	int connFd = -1;
	/* This is a blocking FD. We wait for clients to
	 * issue connects. We then accept. On their request
	 * we send the data which the num_sock and each port 
	 * number
	 */
	rsa_addr_len = sizeof(rsa);
	errno = 0;
	if ((connFd = accept(s, (struct sockaddr *) &rsa, 
					&rsa_addr_len)) != -1) {
	    printf("accept(): %s\n",strerror(errno));
	}
	printf ("connected to Remote address: %s port: %u\n",
	    inet_ntop(AF_INET, &rsa.sin_addr, 
		raddr_buf, INET_ADDRSTRLEN), ntohs(rsa.sin_port)); 
	do_send_recv(connFd);
	close(connFd);
    }
    if (s != -1) {
	close(s);
    }
    return (void *) NULL;
}


int main(int argc, char **argv)
{
    int option = 0;  
    struct sockaddr_in	ms_addr = {
	.sin_family = AF_INET, 
    };
    int num_sock = 0;

    if (argc < 8) {
	usage(argv[0]);
	return 0;
    }
    while ((option = getopt(argc, argv, "p:m:d:n:")) != -1) {
	//printf("option: %c , optarg: %s\n", option, optarg);
	switch (option) {
	    case 'p': {
		DEFAULT_PORT = atoi(optarg);
	        ms_addr.sin_port = htons(DEFAULT_PORT);
		break;
	    }
	    case 'm': 
		if (!inet_pton(AF_INET, optarg, &ms_addr.sin_addr)) {
		    printf ("Error: Address not correct: %s\n",optarg);
		    return 0;
		}
		break;
	    case 'd':
		if (!inet_pton(AF_INET, optarg, &d_addr.sin_addr)) {
		    printf ("Error: Address not correct: %s\n",optarg);
		    return 0;
		}
		break;
	    case 'n':
		num_sock = atoi(optarg);
		printf ("Num sock %d\n", num_sock);
		break;
	    default:
		usage(argv[0]);
		return 0;
	}
    }
    //printf("Args: m = %08X, d = %08X, n = %u option: %d,"
    //		" optarg: %s\n", ms_addr.sin_addr.s_addr, 
    //		d_addr.sin_addr.s_addr, num_sock, option, optarg);
    //t_handles[0] = create_MS_thread(&ms_addr);
    //pthread_join(t_handles[0], NULL);
    ms_start_listen(&ms_addr);

    return 0;
}
