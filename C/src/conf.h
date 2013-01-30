#ifndef __CONF_H__
	#define __CONF_H__

	//Define the memory shared file for interprocess communication
	/*
	* It seems reasonable to define 1MB for the sharespace. If we
	* An 8-digit hex will give me 2^15 numbers to work with, and 
	* I'll be able to store 131,072 numbers at a time, plenty to
	* play with...
	*/
	#define MEMSHARESIZE 1024 * 512
	#define MEMSHARENAME "Network-Graphic-MemShare.mshare"





#endif