/*
* Ethan Eldridge
* February 12th 2013
*
* Bitmap loader, this will load a bitmap image with the 256 ASCII characters to use
*as our font.
*
*/

#ifndef __BITFONT_H__
	#define __BITFONT_H__

//System includes
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

typedef struct{
	SDL_Surface * bitmap;	//Surface for bitmaps to reside on
	SDL_Rect chars[256];	//256 ASCII Characters
	int newline;
	int space;
}BitFont;

/*Setup's the BitFont structure to be usable
*returns -1 on failure, 0 on success
*/
int setupBitFont(BitFont * bitFont);

/*Creates the font we're going to use form the font referenced by conf.h
*/
void buildFontFromImage(BitFont * bitFont);

/*Shows the given text on the screen with it's top left starting point specified by (x,y)
*	x: The x coordinate of the (x,y) pair of the location of the text
*	y: The y coordinate of the (x,y) pair of the location of the text
*	text: The text to be shown (should coincide with 256 ASCII)
*	surface: The surface to draw the text onto
*/
void drawFont(int x, int y, char * text, SDL_Surface * surface);





#endif