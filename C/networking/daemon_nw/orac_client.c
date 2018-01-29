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

int do_send_recv( int );

uint16_t DEFAULT_PORT;
#define MAX_FDS 32
// 5 min
#define POLL_TIMEOUT_MILLSEC (5*60*1000) 

pthread_t t_handles[MAX_FDS];
// For Each listening Daemon Thread
struct sockaddr_in	d_addr;

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
    printf ("%s -m <IP>\n", prog);
    return;
}

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

int process_E_MS_PORT_TABLE_REPLY(INFO_PAYLOAD *p)
{
    
    char addr_buf[INET_ADDRSTRLEN] = {'\0'};
    printf("%s\n",__func__);
    printf("========================================\n");

    printf("Cluster IP:	%s \n",    
	    inet_ntop(AF_INET, (struct in_addr *)&p->data_size, 
			    addr_buf, INET_ADDRSTRLEN));

    printf("Hosts are listening on ports:");
    dump_port_seq(p->port_numbers);
    return 0;
}

int do_send_recv( int connFd) 
{
    ssize_t rs = -1;
    INFO_PAYLOAD data;
    data.info_type = E_MS_PORT_TABLE_REQUEST;
    data.data_size = sizeof(data);
    //write_port_numbers(data.port_numbers);
    if ((rs = send(connFd, &data, sizeof(data), 0)) != sizeof(data)) {
        printf("Error sending E_MS_PORT_TABLE_REQUEST: %zd\n", rs);
    } else {
        printf("sent E_MS_PORT_TABLE_REQUEST: %zd\n", rs);
    }
	
    if ((rs = recv(connFd, &data, sizeof(data), 0)) != sizeof(data)) {
	printf("Could not Recv all the data. Only recvd: %zd bytes\n", rs);
    } else {
	printf("Recvd ");
	switch (data.info_type) {
	    case E_MS_PORT_TABLE_REQUEST: {
		printf("E_MS_PORT_TABLE_REQUEST : %zd\n",rs);
		break;
	    }
	    case E_MS_PORT_TABLE_REPLY: {
		printf("E_MS_PORT_TABLE_REPLY: %zd\n", rs);
		process_E_MS_PORT_TABLE_REPLY(&data);
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

void * ms_start_connect (struct sockaddr_in *arg) 
{
    int s = -1;
    struct sockaddr_in ms_addr = *arg;
    char laddr_buf[INET_ADDRSTRLEN] = {'\0'};
    int reuse_addr = 1;	    
			   
    inet_ntop(AF_INET, &ms_addr.sin_addr, 
			    laddr_buf, INET_ADDRSTRLEN);
    if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
	printf("socket(): %s\n", strerror(errno));
    } 

    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse_addr, 
					    sizeof(reuse_addr));

    errno = 0;
    if (connect(s, (struct sockaddr *) &ms_addr, 
			    sizeof(struct sockaddr_in)) != 0) {
	printf("connect(): %s: %s::%u\n",strerror(errno), 
			    laddr_buf, ntohs(ms_addr.sin_port));
    } else {
     
	printf ("connected to Remote address: %s port: %u\n", 
				laddr_buf, ntohs(ms_addr.sin_port)); 
	do_send_recv(s);
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
	.sin_port = htons(DEFAULT_PORT) 
    };

    if (argc < 5) {
	usage(argv[0]);
	return 0;
    }
    memset(&d_addr,0, sizeof(d_addr));
    while ((option = getopt(argc, argv, "p:m:")) != -1) {
	//printf("option: %c , optarg: %s\n", option, optarg);
	switch (option) {	
	    case 'p': {
		DEFAULT_PORT = ms_addr.sin_port = htons(atoi(optarg));
		break;
	   }
	    case 'm': 
		if (!inet_pton(AF_INET, optarg, &ms_addr.sin_addr)) {
		    printf ("Error: Address not correct: %s\n",optarg);
		    return 0;
		}
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
    ms_start_connect(&ms_addr);

    return 0;
}
