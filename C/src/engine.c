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

//System Includes
#include "conf.h"			
#include <sys/mman.h>		//Allows use of mmap
#include <fcntl.h>    		//For file constants (read write,create,permissions...)
#include <assert.h> 
#include <pthread.h>		//For threading processes
#include <stdio.h>
#include <unistd.h>			//for sleep

//Nonsystem Includes
#include "network.h"
#include "graphics.h"

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
	//Dereference the passed in integer
	int fd = *((int *) memFD);
	NetworkModule nm;

	if(setupNetworkModule(fd,&nm) < 0){
		puts("Failed setting up NetworkModule");
		return NULL;
	}

	runServer(&nm);
	
	return NULL;
}

/*Function to pass to pthread for graphics, creates the graphics process
*	memFD: The file descriptor for the file being memory mapped between
*		   graphics and network (passed as void due to pthread, but cast to int)
*/
void * createAndRunGraphics(void *memFD){
	int fd = *((int *) memFD);

	GraphicModule gm;

	if(setupGraphicModule(fd,&gm) < 0){
		puts("Failed setting up GraphicModule");
		return NULL;
	}

	puts("reading");
	int test = *((int *)gm.memShareAddr+1);
	printf("%d\n",test );

	puts("running graphics");
	runGraphics(&gm);

	return NULL;
}

int main(int argc, char const *argv[])
{
	int memS;
	memS = createMemShare();
	//We will not execute if we don't have this share. plain and simple.
	assert(memS != -1);

	//Create the threads for the graphics,network,logger, and anything else.
	pthread_t nThread, gThread;

	int nProcThreaded = pthread_create(&nThread,NULL,createAndRunNetwork,(void *)&memS);
	//Wait a moment to allow the mmap filed to be created
	sleep(1);
	int gProcThreaded = pthread_create(&gThread,NULL,createAndRunGraphics,(void *)&memS);

	//Join
	pthread_join( nThread, NULL);
    pthread_join( gThread, NULL);

    close(memS);

    //This is commented out while I manipulate the mapped file to get a feel.
	//if(unlink(MEMSHARENAME) < 0){
	//	puts("Issue removing memory share for engine");
	//}
	return 0;
}