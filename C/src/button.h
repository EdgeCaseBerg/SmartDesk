#ifndef __BUTTON_H__
	#define __BUTTON_H__

#include <SDL.h>
#include <SDL/SDL.h>

#include "bitfont.h"

#define BUTTONBACKGROUNDOFFSET 5

typedef struct{
	Sint16 x,y;
	Uint16 width,height;
	Uint8 r,g,b; //We could replace these 3 by a Uint32 color from SDL_MapRGB
	int clicked; //Boolean for if the button is being clicked or not (to shade or not)
	int hover; 	 //Boolean for if the button is being hovered over
	int visible; //Boolean for if the button is being displayed or not
	char * text; //Text to be placed in the button (Haven't figured out best way to do this)
	BitFont * font;
	SDL_Surface *surface;
} ShadedButton;


/*Creates and allocates a button object with the specified parameters
*
*	x: The x location of the button's top left corner
*	y: The y location of the button's top left corner
*	w: The width of the button
*	y: The height of the button
*	r: The red value of the color of the button
*	g: The green value of the color of the button
*	b: The blue value of the color of the button
*	text: The string to be displayed in the button
*	button: A reference to the button variable you are filling out
*	display: The display to copy the settings from onto the surface for fast blitting
*	returns -1 if the button is not created correctly
*/
int setupShadedButton(Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint8 r, Uint8 g, Uint8 b, char * text,ShadedButton * button,BitFont * font, const int visible, const SDL_Surface * display);

/*Free's the memory used by the button
*	button: The button whose memory will be free-ed
*/
void freeButton(ShadedButton * button);

/*Boolean to tell if the given x,y are within the button's limits
*	button: The button we're checking within
*	x: The x coordinate to check
*	y: The y coordinate to check
*	returns -1 if the x,y pair are not within the button or the button is null, 0 if they are
*/
int within(const ShadedButton * button, const int x, const int y);

/*Draws the button to the screen
*
* 	button: The button to be drawn, specified by its rgb,(x,y) coordinate and width and height values
*	screen: The screen for the button to be drawn to
*/
void drawShadedButton(ShadedButton * button, SDL_Surface *screen);

/*Removes the button from the screen (by drawing white over it)
*	button: The button to be undrawn
*	screen: The screen for the button to be undrawn from
*	Note: This just draws white over the button, not a good way of doing it, and this should be changed
*		  so that buttons reside on their own surface and we can not lose data underneath the buttons
*/
void hideButton(ShadedButton * button, SDL_Surface * screen);


#endif