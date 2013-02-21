#ifndef __DIALOG_H__
	#define __DIALOG_H__

//System includes
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

//Local includes
#include "button.h"
#include "bitfont.h"

typedef struct{
	SDL_Surface * surface;		
	SDL_Rect location;			//Where the surface will be rendered to display
	int visible;
	BitFont * font;				//Font engine pointer
	char * text;				//String pointer for text to display
	ShadedButton * buttons[2];	//Yes|No Buttons
}Dialog;

/*
*Creates the dialog copying the display's settings for fast blit surfacing using the text at the specified location
*	dialog: The Dialog pointer to return containing the constructed
*	text: The \0 ended character string
*	visible: Whether or not the dialog is initially visible ( 0 | 1)
*	x:	The top left corner of the dialog (x coordinate)
*	y:  The top left corner of the dialog (y coordinate)
*	display: The surface to copy its sendings from
*	font: The BitFont engine used to render text
*	returns -1 if the dialog fails to be created, 0 otherwise
*/
int createDialog(char * text, int visible, const int x, const int y, const SDL_Surface * display, const BitFont * font);

/*
*Draws the dialog to the screen
*	screen: The screen to draw the dialog to
*	dialog: The dialog to draw
*/
void drawDialog(const SDL_Surface * screen, const Dialog * dialog );

/*
*Boolean for whether or not a give x,y pair is within the Dialog
*	dialog: The Dialog to check within
*	x: The x coordinate of the x pair
*	y: The y coordinate of the y pair
*	returns -1 if the dialog is not within the Dialog, 0 if it is
*/
int withinDialog(Dialog * dialog, const int x, const int y);


/*
*Hide the Dialog from the given screen (done by blitting white to its location)
*	dialog: The Dialog to hide from the screen  -see below
*	screen: The screen to hide the dialog from  -Seems awfully circuler this documentation doesn't it? see above
*/
void hideDialog(Dialog * dialog, SDL_Surface * screen);


#endif