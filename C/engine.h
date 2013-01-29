#ifndef __ENGINE_H__
	#define __ENGINE_H__

//Creates a memory mapped file using the constants defined in conf.h
//Returns -1 on failure, file descriptor for file on success.
int createMemShare();

/*Function to pass to pthread for network, creates the network process
*	memFD: The file descriptor for the file being memory mapped between
*		   graphics and network (passed as void due to pthread, but cast to int)
*/
void createAndRunNetwork(void * memFD);


#endif