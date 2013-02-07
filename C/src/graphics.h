
#ifndef __GRAPHICS_H__
	#define __GRAPHICS_H__

#include <SDL.h>
#include <SDL/SDL.h>

typedef struct{
	void * memShareAddr;
	int memShareFD;
	SDL_Surface *screen;
} GraphicModule;

/*Set's a pixel on the screen at the x,y coordinates to the r g b value.
*/
void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b);

/*Draws the screen
* 	screen: SDL_Surface to draw on
*	h: A variable to remove when I'm done testing
*/
void drawScreen(SDL_Surface* screen, int h);

/*Runs the GraphicModule for the program
*	module: GraphicModule to run
*/
void runGraphics(GraphicModule * module);


/*
*Set's up the graphics module
*	fd: File Descriptor of the memory mapped file shared between graphics and network
*	module: Pointer to a GraphicModule struct to setup
*	returns -1 on failure to set up the graphics module, 0 on success
*/
int setupGraphicModule(int fd, GraphicModule * module);

#endif