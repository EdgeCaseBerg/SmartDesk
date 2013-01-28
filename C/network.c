/*
Ethan Eldridge
January 27th 2013

This file implements the common networking components for all
base modules. Defining an abstraction that will be easy to use
by including network.h

*/

//Types for compatability with BSD
#include <sys/types.h>
#include <sys/socket.h> 
//If we don't include netinet then we dont know the sizeof(sockaddr_in)
#include <netinet/in.h> 
//Get the correct header for inet_ntoa
#include <arpa/inet.h>
//For perror
#include <stdio.h>
//For binding in non blocking mode
#include <fcntl.h>
//Let's get our assert on
#include <assert.h>
//Rather random, but file descriptor sets are stored in time. who knows why... Aliens.
 #include <sys/time.h> 
//For EAGAIN
#include <errno.h>

//Work around for my apparently messed up header:
#ifndef SOL_TCP
	#define SOL_TCP 6
#endif
#ifndef TCP_NODELAY
	#define TCP_NODELAY 1
#endif

/*
createSocket takes the port and string form of an ip address and returns
the socket descriptor (int) if successful in creating the port, or -1 on 
failure. Note that this binds the port. If you're trying to create a port
from for a listen command, this id one by the listen function
port: The port to be bound to
ip_address: The string representation of the ip address
address: The sockaddr_in struct to fill with the information for the socket (needed to be used by accept)
*/
int createSocket(uint16_t port, const char * ip_address,struct sockaddr_in * address){
	//double check the ip address pointer is not null
	assert(ip_address != NULL);

	//Create the socket with ipv4 style, streaming and default protocol
	int socketIdentity;
	socketIdentity = socket(AF_INET,SOCK_STREAM,0);

	if(socketIdentity == 0){
		perror("Error creating socket");
		return -1;
	}

	//We're using ipv4 settings by saying AF_INET
	address->sin_family = AF_INET;
	//htons converts 3490's bytes into a form the network can process
	address->sin_port = htons(port);
	//Convert an ip address into it's network byte form and store it in our struct
	address->sin_addr.s_addr = inet_addr(ip_address);

	//We want to be able to reuse ip addresses:
	int opt = 1;
	if(setsockopt(socketIdentity, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(&opt)) < 0){
		perror("Error setting socket SO_REUSEADDR");
		return -1;
	}
	//Keep the connection alive incase someone falls asleep.
	if(setsockopt(socketIdentity, SOL_SOCKET, SO_KEEPALIVE, (char *)&opt, sizeof(&opt)) < 0){
		perror("Error setting keep alive on socket");
		return -1;
	}
	//No delay in tcp protocol, we dont want to hang the interface
	if(setsockopt(socketIdentity, SOL_TCP,TCP_NODELAY, (char *)&opt, sizeof(&opt)) < 0){
		perror("Error setting no delay on TCP protocol");
		return -1;
	}

	//Set the socket to be non blocking, we do not want to halt any graphics or anything by creating this.
	fcntl(socketIdentity, F_SETFL, O_NONBLOCK);

	//Bind the socket to the port we've described
	int result;
	result = bind(socketIdentity, (struct sockaddr*)address, sizeof(*address));

	if(result!=0){
		perror("Error binding socket to address and port");
	}

	return socketIdentity;
}

/*Checks to see if there is any data ready to be accepted on the current set of sockets
	sockSet: 	The set of sockets we might accept data on
	maxFileDescriptor: The highest numbered file descriptor so far, this is needed for the polling
  This function will poll the fd_set given in sockSet, and possibly modify it. It is wise to create
  a working set from whatever socket set you're wanting to poll. Returns the number of ready sockets
  in the set, or -1 if there was an error
*/
int isDataReady(fd_set * sockSet, const int maxFileDescriptor ){
	int result;
	//Time structure, 0's becuase of polling
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_sec = 0;

	//Select likes it's highest descriptor +1 for whatever reason...
	//NULL's because we dont need a writing or exception set if we're polling
	result = select(maxFileDescriptor +1, sockSet,NULL,NULL,&timeout);

	if(result < 0){
		perror("select failed");
		return -1;
	}

	return result;
}



int main(){
	//This is the standard struct used by C for internet addresses 
	struct sockaddr_in address, otherAddr;	
	fd_set readset;
	FD_ZERO(&readset);
	//Fun fact: 30001 is defined as the PokePort, which is the port used in a network pokemon game
	int test = createSocket(30001,"127.0.0.1",&address);
	
	
	FD_SET(test,&readset);
	//Listen does not need an abstraction on top of it
	listen(test,10);

	

	int other;
	printf("%i\n",isDataReady(&readset,test));
	other = accept(test,NULL,NULL);
	//http://stackoverflow.com/questions/7635440/error-on-accept-resource-temporarily-unavailable
	if(other == -1){
		if(errno != EAGAIN){
			perror("other");	
		}
	}
	puts("hi");
	sleep(2);
	puts("bye");

	//Close the port
	close(test);

}