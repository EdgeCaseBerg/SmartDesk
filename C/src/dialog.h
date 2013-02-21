#ifndef __DIALOG_H__
	#define __DIALOG_H__

//System includes
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

//Local includes
#include "button.h"
#include "bitfont.h"

#define STANDARD_DIALOG_WIDTH 60
#define STANDARD_DIALOG_HEIGHT 80
#define DIALOG_COLOR 0xD3D3D3
//Button constants (confirm and cancel index into the buttons array of Dialog struct)
#define NO_CLICK 2
#define CONFIRM_CLICK 0
#define CANCEL_CLICK 1
#define DIALOG_BUTTON_WIDTH (STANDARD_DIALOG_WIDTH/3)
#define DIALOG_BUTTON_HEIGHT 30
#define DIALOG_BUTTON_HORIZONTAL_OFFSET  (4*BUTTONBACKGROUNDOFFSET)
#define DIALOG_BUTTON_VERTICAL_LOCATION (STANDARD_DIALOG_HEIGHT - DIALOG_BUTTON_HORIZONTAL_OFFSET - DIALOG_BUTTON_HEIGHT)
#define CONFIRM_X_LOCATION ((STANDARD_DIALOG_WIDTH/2) - DIALOG_BUTTON_WIDTH - 2*BUTTONBACKGROUNDOFFSET)
#define CANCEL_X_LOCATION (CONFIRM_X_LOCATION + DIALOG_BUTTON_WIDTH + DIALOG_BUTTON_HORIZONTAL_OFFSET)

typedef struct{	
	SDL_Rect location;			//Where the surface will be rendered to display
	int visible;
	BitFont * font;				//Font engine pointer
	char * text;				//String pointer for text to display
	ShadedButton * buttons[2];	//Yes|No Buttons
	SDL_Surface * surface;	
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
int createDialog(Dialog * dialog, char * text, int visible, int x, int y, const SDL_Surface * display, BitFont * font);

/*
*Draws the dialog to the screen
*	screen: The screen to draw the dialog to
*	dialog: The dialog to draw
*/
void drawDialog( Dialog * dialog, SDL_Surface * screen );

/*
*Boolean for whether or not a give x,y pair is within the Dialog
*	dialog: The Dialog to check within
*	x: The x coordinate of the x pair
*	y: The y coordinate of the y pair
*	returns -1 if the dialog is not within the Dialog, 0 if it is
*/
int withinDialog(Dialog * dialog, const int x, const int y);


/*
*Returns one of three constants depending on if and what button is clicked
*	dialog: The Dialog whose buttons we're checking out
*	x: The x coordinate of the coordinate pair we're checking against
*	y: The y coordinate of the coordinate pair we're checking against
*	returns NO_CLICK on no buttons clicked, CONFIRM_CLICK or CANCEL_CLICK dependin on which button is clicked
*/
int buttonClicked(Dialog * dialog, const int x, const int y);

/*
*Hide the Dialog from the given screen (done by blitting white to its location)
*	dialog: The Dialog to hide from the screen  -see below
*	screen: The screen to hide the dialog from  -Seems awfully circuler this documentation doesn't it? see above
*/
void hideDialog(Dialog * dialog, SDL_Surface * screen);

/*
*Free memory used by this dialog (DOES NOT FREE THE ACTUAL POINTER PASSED IN. That's up to the calling context)
*	dialog: The dialog whose memory we're freeing
*/
void freeDialog(Dialog * dialog);


#endif