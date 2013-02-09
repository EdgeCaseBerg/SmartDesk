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
#include <math.h>           //sqrt (compile with -lm flag)

//Local includes
#include "conf.h"
#include "graphics.h"

//Global variables
int mouseDown = 0;			           //True/False for if the mouse is 
int brushSize = 6;                     //Size of the brush
int static buffered[CLICKBUFFERSIZE];  //Buffer to hold x,y coordinates to draw
int static bufferPointer = 0;          //When to stop reading from the buffered


//Returns -1 on failure, 0 on success, sets up the module
int setupGraphicModule(int fd, GraphicModule * module){

	//Zero the buffered array
	memset(buffered,0,sizeof(buffered));

	//Setup the void pointer for the mapped file
	module->memShareAddr = (void*)malloc(sizeof(void*));
	module->memShareFD = -1;
	if(module->memShareAddr == NULL){
		puts("Failed allocating memory for Graphics Module");
		return -1;
	}

	module->memShareAddr = mmap(NULL, MEMSHARESIZE, PROT_READ, MAP_SHARED, fd, 0);
	if(module->memShareAddr == MAP_FAILED){
		perror("memsharegraphic");
		puts("Failed to map memory share to network module");
		return -1;
	}
	module->memShareFD = fd;
	msync(module->memShareAddr,sizeof(int),MS_SYNC|MS_INVALIDATE);

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

void smoothPath(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel){
    //The relative x,y tell us how far away from the previous x,y we were.
    printf("%d %d, %d %d\n", x,y,xrel,yrel);
    //x-xrel gives us the previous coordinate
    //We can use this to define a path between the points and fill in holes
    float dist = sqrt((x-(x-xrel))*(x-(x-xrel)) + (y-(y-yrel))*(y-(y-yrel)));

    //Consider making these static or pre-declared for time saving
    float xSteps = xrel/dist;
    float ySteps = yrel/dist;

    int i = 1;
    for(; i < dist/SMOOTHINGSTEPS; i++){
        buffered[bufferPointer] = (int)(x - xSteps*i);
        buffered[bufferPointer+1] = (int)(y - ySteps*i);
        bufferPointer= bufferPointer+2;
    }
    

}

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

void drawBuffered(SDL_Surface *screen){
	if(SDL_MUSTLOCK(screen)) 
    {
        if(SDL_LockSurface(screen) < 0){
        	return;	
        } 
    }

    int i = 0;
    int x,y=0;
    for(; i < bufferPointer && i < CLICKBUFFERSIZE; i=i+2){
    	//Each odd number is an x, each even is a y
        for(x=0; x < brushSize; x++ ){
            for(y=0; y < brushSize; y++){
                setpixel(screen, buffered[i]+x,(buffered[i+1]+y)*screen->pitch/BITSPERPIXEL,0,0,0);        
            }
        }
    }
    //Done drawing reset:
    bufferPointer=0;

    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  
    SDL_Flip(screen); 
}

void clearScreen(SDL_Surface* screen){ 
    if(SDL_MUSTLOCK(screen)) 
    {
        if(SDL_LockSurface(screen) < 0){
        	return;
        }
    }

    int x, y, ytimesw;
  

    for(y = 0; y < screen->h; y++ ) 
    {
    	//pitch is the scanline
        ytimesw = y*screen->pitch/BITSPERPIXEL;
        for( x = 0; x < screen->w; x++ ) 
        {
            setpixel(screen, x, ytimesw, 255, 255, 255);
        }
    }

    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  
    SDL_Flip(screen); 
}

void runGraphics(GraphicModule * module){
    SDL_Event event;
  
    int keyQuit = 0;
    
    clearScreen(module->screen);
    //Main graphics event loop goes until an event causes keyquit != 0
    while(keyQuit == 0){
        
        //Loop until there are no more events to process
        while(SDL_PollEvent(&event)) {    
        	handleGraphicEvent(event, module,&keyQuit);  
		}
        //Since smoothing is a preprocessor, if it's set to !1 then this call
        //should be optimized out by the compiler
        drawBuffered(module->screen);
    }

    SDL_Quit();
  

}

void handleGraphicEvent(SDL_Event  event, GraphicModule * module, int * stopFlag){
	//Giant Case to handle all events
	switch (event.type){
        case SDL_QUIT:
	       	//Halt the execution of the graphics
	       	*stopFlag = 1;
	       	break;
	    case SDL_KEYDOWN:
	    	handleKeyEvent(event,stopFlag,module);
	    	break;
	    case SDL_MOUSEBUTTONDOWN:
	    	mouseDown = 1;
            handleMouseEvent(event);
	    	break;
	    case SDL_MOUSEBUTTONUP:
	    	mouseDown = 0;
	    	break;
	    case SDL_MOUSEMOTION:
	    	handleMouseEvent(event);
	    	break;
        default:
            break;
	}
}

void handleKeyEvent(SDL_Event  event, int *stopFlag, GraphicModule * module){
	switch( event.key.keysym.sym ){
		case SDLK_ESCAPE:
	    	*stopFlag = 1;
	    	break;
        case SDLK_F1:
            clearScreen(module->screen);
            break;
        default:
            break;
	}
}

void handleMouseEvent(SDL_Event event){
    if(mouseDown){
        buffered[bufferPointer] = event.motion.x;
        buffered[bufferPointer+1] = event.motion.y;
        //If smoothing is off this will be optimized out by the compiler
        if(SMOOTHING==1){
            smoothPath(event.motion.x,event.motion.y,event.motion.xrel,event.motion.yrel);
        }
        bufferPointer = bufferPointer+2;
        if(bufferPointer > CLICKBUFFERSIZE){
            puts("aw shit ");//what a good error message for now
        }
    }
}