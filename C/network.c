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
//For perror
#include <stdio.h>
//For binding in non blocking mode
#include <fcntl.h>
//Let's get our assert on
#include <assert.h>

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
*/
int createSocket(uint16_t port, const char * ip_address){
	//double check the ip address pointer is not null
	assert(ip_address != NULL);

	//Create the socket with ipv4 style, streaming and default protocol
	int socketIdentity;
	socketIdentity = socket(AF_INET,SOCK_STREAM,0);

	if(socketIdentity == 0){
		perror("Error creating socket");
		return -1;
	}

	//This is the standard struct used by C for internet addresses 
	struct sockaddr_in address;


	//We're using ipv4 settings by saying AF_INET
	address.sin_family = AF_INET;
	//htons converts 3490's bytes into a form the network can process
	address.sin_port = htons(port);
	//Convert an ip address into it's network byte form and store it in our struct
	inet_aton(ip_address, &address.sin_addr.s_addr);

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
	result = bind(socketIdentity, (struct sockaddr*)&address, sizeof(address));

	if(result!=0){
		perror("Error binding sockey to address and port");
	}

	return socketIdentity;
}

int main(){
	//Poke port!
	int test = createSocket(30001,"127.0.0.1");

	puts("hi");
	sleep(2);
	puts("bye");

	//Close the port
	close(test);

}