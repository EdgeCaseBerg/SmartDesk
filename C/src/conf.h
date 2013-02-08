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

	//Define the network settings for connections
	#define SERVERPORT 30001
	#define LOCALHOST "127.0.0.1"

	//Settings for graphics 
	#define SCREENWIDTH  640
	#define SCREENHEIGHT 480
	#define SCREENDEPTH 32
	#define BITSPERPIXEL 4
	#define CLICKBUFFERSIZE 1024



#endif