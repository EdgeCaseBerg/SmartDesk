/*
Ethan Eldridge
January 28th 2013

This file handles running the network and the graphics simultaneously. 

-Initial thoughts:
	Use memory mapped file (size specified in conf.h) with read for the graphics, write access for network

*/

#include "conf.h"
#include <sys/mman.h>
#include <fcntl.h>

int createMemShare(){
	int fd;
	//Making up our own file extension here
	//We want to open the file with readwrite,create it, and empty it if it exists
	//We want the user to have permission to read and write from it
	fd = open("Network-Graphic-MemShare.mshare", O_RDWR| O_CREAT | O_TRUNC, S_IRUSR| S_IWUSR );


}

int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}