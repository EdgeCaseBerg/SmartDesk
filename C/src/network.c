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
//Need mmap for interprocess comm
#include <sys/mman.h>
//malloc and free
#include <stdlib.h>
//Odd enough, memset is in string.h
#include <string.h>

//Non standard includes:
#include "conf.h"
#include "network.h"

//Work around for my apparently messed up header:
#ifndef SOL_TCP
	#define SOL_TCP 6
#endif
#ifndef TCP_NODELAY
	#define TCP_NODELAY 1
#endif



//Free's memory used by the NetworkModule
void destroyNetworkModule(NetworkModule * module){
	close(module->memShareAddr);
	close(module->memShareFD);
	close(module->serverSockFD);
	module->memSeekInt =0;
}

//Instantiates the networkModule returns -1 on failure, 0 on success
int createNetworkModule(NetworkModule * module){
	//(void*)malloc(sizeof(void*));
	//memShareAddr was originally allocated with the above
	//but it's really not nessecary, and might cause a small memleak
	module->memShareAddr = NULL;
	module->memShareFD   = -1;
	module->serverSockFD = -1;
	module->memSeekInt = 0;


	return 0;
}

//Creates a socket for the server to run on, returns -1 on failure, 0 on success
int createServerSocket(NetworkModule * module){
	struct sockaddr_in serv_addr;
	//Internet based socket, tcp and default protocol
	module->serverSockFD = socket(AF_INET, SOCK_STREAM, 0);
	if(module->serverSockFD < 0){
		return -1;
	}

	//Zero memory for struct
	memset((char *) &serv_addr, 0, sizeof(serv_addr));

    //Setup internet portwith default addressing
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //Convert port to network byte order
    serv_addr.sin_port = htons(SERVERPORT);

    if (bind(module->serverSockFD , (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
    	//Really want to make a logging function here
    	return -1;
    }
    return 0;
}

/*Set's up the memory share for the module and initializes network connections. Also calls
  createNetworkModule, so you only need to pass an uninitialized struct of NetworkModule to
  the function, it will handle instantiating it for you.
	fd: File descriptor for file to write to
	module: NetworkModule struct to fill out with information
Really good example of nonblocking io.
http://publib.boulder.ibm.com/infocenter/iseries/v5r3/index.jsp?topic=%2Frzab6%2Frzab6xnonblock.htm 

Returns -1 on failure, 0 on success
*/
int setupNetworkModule(int fd, NetworkModule * module){
	int result = createNetworkModule(module);
	if(result == -1){
		puts("Problem allocating memory for network module");
		return -1;
	}

	//Set up the memory share:
	// Write only for the network, it doesn't need to read it at all
	module->memShareAddr = mmap(NULL, MEMSHARESIZE, PROT_WRITE, MAP_SHARED, fd, 0);
	if(module->memShareAddr == MAP_FAILED){
		puts("Failed to map memory share to network module");
		munmap(module->memShareAddr,MEMSHARESIZE);
		return -1;
	}
	module->memShareFD = fd;

	//Create the server 
	if(createServerSocket(module) < 0){
		puts("Failed to create the socket for the server");
		return -1;
	}

	return 0;
}

void runServer(NetworkModule * module){
	//Set up client structures for incoming messages
	struct sockaddr_in cli_addr;
	socklen_t clientLen;

	//list for incoming messages. (up to 5)
	//Note that 5 is chosen because its the max for most systems
	listen(module->serverSockFD,5);
	clientLen = sizeof(cli_addr);
	//This is where we would accept an incoming message
	printf("%s on %d \n", "Server Running", module->serverSockFD);
	//This part should loop, but for testing purposes just once
	//Accept an incoming connection
	int incomingFD=-1;
	
	puts("waiting");
	incomingFD = accept(module->serverSockFD,
							(struct sockaddr *) &cli_addr,
							&clientLen);
	printf("%d\n", incomingFD);
	if(incomingFD < 0){
		perror("incomingFD");
	}
	puts("accepting");
	handleIncoming(incomingFD, module);
	close(incomingFD);
	//Let's try a write to the mapped file
	//*(((int *)module->memShareAddr)) = 0xFFFFAAAA;
	//*(((int *)module->memShareAddr)+1) = 0xFFFFAAAA;
	
	msync(module->memShareAddr,sizeof(int),MS_SYNC|MS_INVALIDATE);
	
	
	close(module->serverSockFD);
	destroyNetworkModule(module);
}


void handleIncoming(int fd, NetworkModule * module){
	char buffer[256];
	memset(buffer,0,256);

	int bytesRead = 0;
	bytesRead = read(fd,buffer,255);

	if(bytesRead < 0){
		puts("I read no bytes");
		return;
	}else{
		printf("I read %d bytes \n", bytesRead);
	}

	printf("%s\n", buffer);

	//Handle the Size of our file pointer getting crazy
	if(module->memSeekInt + bytesRead >= MEMSHARESIZE){
		//Essentially loop it around
		module->memSeekInt=0;
	}

	//Load the buffer to the shared memory
	//There should probably be some semaphore action going on
	int i;
	for(i =0; i < bytesRead; i++){
		//Write to the buffer at the current seek position
		puts(&buffer[i]);
		*(((int *)module->memShareAddr)+module->memSeekInt+i) = buffer[i];	
	}
	module->memSeekInt = module->memSeekInt + bytesRead;	

	msync(module->memShareAddr,sizeof(int),MS_SYNC|MS_INVALIDATE);

}