#ifndef __ENGINE_H__
	#define __ENGINE_H__

//Creates a memory mapped file using the constants defined in conf.h
//Returns -1 on failure, file descriptor for file on success.
int createMemShare();

#endif