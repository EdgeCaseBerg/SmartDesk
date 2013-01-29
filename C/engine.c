/*
Ethan Eldridge
January 28th 2013

This file handles running the network and the graphics simultaneously. 
Setting up the memory share between the two and sets them each up on
their own thread. Once both are running the engine simply waits for
the two processes/threads to finish and then will clean up the files
made and such.

-Initial thoughts:
	Use memory mapped file (size specified in conf.h) with read for the graphics, write access for network
	pthreads for running the graphics and the network at the same time.

-TODO:
	Create a logging structure and use that instead of 'puts'
	pthreads

*/

#include "conf.h"			
#include <sys/mman.h>		//Allows use of mmap
#include <fcntl.h>    		//For file constants (read write,create,permissions...)
#include <assert.h> 

/*Creates the memory to be shared between the network and graphics processes
*returns -1 on failure, file descriptor for shared file on success
*/
int createMemShare(){
	//File descriptor declaration: (It's amazing how few people realize what fd stands for)
	int fd;
	//We want to open the file with readwrite,create it, and empty it if it exists
	//We want the user to have permission to read and write from it
	fd = open(MEMSHARENAME, O_RDWR| O_CREAT | O_TRUNC, S_IRUSR| S_IWUSR );
	if(fd <= 0){
		//I'd like to replace these puts with a logger
		puts("Failed in creating memory share for engine.");
		return -1;
	}
	//Move the file pointer and write an empty byte, this forces the file to
	//be of the size we want it to be.
	if (lseek(fd, MEMSHARESIZE - 1, SEEK_SET) == -1) {
		puts("Failed to expand the memory share for engine to the correct size.");
		return -1;
	}
	//Write out 1 byte
	write(fd, "", 1);

	//Memory share is now set to use, send it back.
	return fd;
}

/*Function to pass to pthread for network, creates the network process
*	memFD: The file descriptor for the file being memory mapped between
*		   graphics and network (passed as void due to pthread, but cast to int)
*/
void * createAndRunNetwork(void *memFD){

}



int main(int argc, char const *argv[])
{
	int memS;
	memS = createMemShare();
	//We will not execute if we don't have this share. plain and simple.
	assert(memS != -1);

	//Create the threads for the graphics,network,logger, and anything else.

	if(unlink(MEMSHARENAME) < 0){
		puts("Issue removing memory share for engine");
	}
	return 0;
}