/*
Ethan Eldridge
January 27th 2013

Networking Test file.
*/

//Types for compatability with BSD
#include <sys/types.h>
#include <sys/socket.h> 
//If we don't include netinet then we dont know the sizeof(sockaddr_in)
#include <netinet/in.h> 

//For perror
#include <stdio.h>

int main(){
	//Create a socket 
	int socketIdentity;
	socketIdentity = socket(AF_INET,SOCK_STREAM,0);

	if(socketIdentity == 0){
		//Error creating Socket
		perror("Error creating socket");
	}

	//This is the standard struct used by C for internet addresses 
	struct sockaddr_in address;

	//We're using ipv4 settings by saying AF_INET
	address.sin_family = AF_INET;
	//htons converts 3490's bytes into a form the network can process
	address.sin_port = htons(3490);
	//Convert an ip address into it's network byte form and store it in our struct
	inet_aton("63.161.169.137", &address.sin_addr.s_addr);

	//Bind the socket to the port we've described
	bind(socketIdentity, (struct sockaddr*)&address, sizeof(address));

	puts("hi");
	sleep(10);
	puts("bye");

	//Close the port
	close(socketIdentity);

}