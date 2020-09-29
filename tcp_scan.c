#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#define SA struct sockaddr 
#define MAX_PORT_ENTRY 500
void dump_ports(unsigned short *port_used, int num, unsigned char *path)
{
	int i;
	FILE *fptr;
	printf("\nDumping ports %d \n",num);

	fptr = fopen(path,"w");

	if(fptr == NULL)
	{
		printf("Error!");   
		exit(1);             
	}
	for(i = 0; i < num; i++)
	{
		fprintf(fptr,"%d\n",port_used[i]);
	}
	fclose(fptr);
}
int main( int argc, char *argv[] )
{
	if(argc < 2)
	{
		printf("Arguement is not valid\n");
		printf("usage: ./tcpscan.o lowerbound upperbound ipaddress filename\n");
		return 0;
	}
	unsigned short port_used[MAX_PORT_ENTRY];
	int port_idx = 0;
	int lower_bound = atoi(argv[1]);
	int upper_bound = atoi(argv[2]);
	if(upper_bound < lower_bound)
	{
		printf("upper bound must be greater than lower bound");
		return 0;
	}
	//int mode;//active or passive scan
	printf("\nupper bound %d lower bound %d ip_addr %s path %s \n", upper_bound, lower_bound, argv[3], argv[4]);


	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 
	printf("scanning port...");
	do
	{
		//socket create and varification 
		sockfd = socket(AF_INET, SOCK_STREAM, 0); 
		if (sockfd == -1) { 
			printf("socket creation failed...\n"); 
			exit(0); 
		} 
		bzero(&servaddr, sizeof(servaddr)); 

		// assign IP, PORT 
		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = inet_addr(argv[3]); 
		servaddr.sin_port = htons(lower_bound); 

		// connect the client socket to server socket 
		if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
			printf("connection with the server failed...%d\n", lower_bound); 
		} 
		else
		{
			printf("port %d connected to the server..\n", lower_bound); 
			port_used[port_idx++] = lower_bound;
		}
		lower_bound++;
		close(sockfd); 
	}while(upper_bound != lower_bound);
	unsigned char path[100];
	sprintf(path, "./%s", argv[4]);
	printf("\npath is %s\n", path);
	dump_ports(port_used, port_idx-1, path);
} 

