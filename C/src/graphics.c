/*
*Ethan Eldridge
*January 30 2013
*
*The purpose of this file is to provide graphics to the program
*and to define how to read the memory mapped file
*
* To be honest I've gotta set up some type of protocol for reading from the
*thing. It's easy enough to sync the memory on each call, but how do I know
*if I'm reading 'too fast' and the networking isn't giving me anything? There
*needs to be some null value coming in from the file or something. Or a global
*count of how many bytes I've written so far. Then graphics reads up to that, if
*the reading count is higher than the writting count than the writer looped around
*the file and began writing at the beginning, so we need to reflect that...
*
* I could have the writer write the offset it's on at the 0th byte so the reader 
*could just msync, read that, and then continue on... 
*/

//System includes
#include <sys/mman.h>		//memory mapped files
#include <stdlib.h>			//malloc and free
#include <stdio.h>			//puts

//Local includes
#include "conf.h"
#include "graphics.h"


void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 colour;  

	//Convert RGB to a color with the proper screen format 
    colour = SDL_MapRGB( screen->format, r, g, b );
  
  	//Set the pixels for 32 bit depth (if want different depth will have to mess)
    pixmem32 = (Uint32*) screen->pixels  + y + x;
    *pixmem32 = colour;
}

void drawScreen(SDL_Surface* screen, int h){ 
    if(SDL_MUSTLOCK(screen)) 
    {
        if(SDL_LockSurface(screen) < 0) return;
    }

    int x, y, ytimesw;
  

    for(y = 0; y < screen->h; y++ ) 
    {
    	//pitch is the scanline
        ytimesw = y*screen->pitch/BITSPERPIXEL;
        for( x = 0; x < screen->w; x++ ) 
        {
            setpixel(screen, x, ytimesw, (x*x)/256+3*y+h, (y*y)/256+x+h, h);
        }
    }

    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  
    SDL_Flip(screen); 
}

void runGraphics(GraphicModule * module){
    SDL_Event event;
  
    int keyQuit = 0;
    int h=0; 

    //Main graphics event loop
    while(keyQuit != 0){
        drawScreen(module->screen,h++);
        while(SDL_PollEvent(&event)) {      
            switch (event.type){
                case SDL_QUIT:
	                keyQuit = 1;
	                break;
                case SDL_KEYDOWN:
                    keyQuit = 1;
                    break;
			}
		}
    }

    SDL_Quit();
  

}

//Returns -1 on failure, 0 on success, sets up the module
int setupGraphicModule(int fd, GraphicModule * module){
	//Setup the void pointer for the mapped file
	module->memShareAddr = (void*)malloc(sizeof(void*));
	module->memShareFD = -1;
	if(module->memShareAddr == NULL){
		puts("Failed allocating memory for Graphics Module");
		return -1;
	}

	module->memShareAddr = mmap(NULL, MEMSHARESIZE, PROT_READ, MAP_SHARED, fd, 0);
	if(module->memShareAddr == MAP_FAILED){
		puts("Failed to map memory share to network module");
		return -1;
	}
	module->memShareFD = fd;
	msync(module->memShareAddr,sizeof(int),MS_SYNC|MS_INVALIDATE);
	close(fd);

	//Set up the graphics
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ){
		puts("Failed to initalize video");
		return -1;
	}

	module->screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, SCREENDEPTH, SDL_HWSURFACE);
	if(module->screen == NULL){
		puts("Failed to set video mode");
		SDL_Quit();
		return -1;
	}
   

	return 0;
}
