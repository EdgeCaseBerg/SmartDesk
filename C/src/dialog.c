/*
*Ethan Eldridge
*February 21st 2013
*
*Dialog.c
*
*This file defines the implementation of the Dialogs, drawing, hiding, setting up and destroying
*
*
*/


#include "dialog.h"



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
int createDialog(Dialog * dialog, char * text, int visible, int x, int y, const SDL_Surface * display, BitFont * font){
	dialog->surface = createSurface(SDL_SWSURFACE, STANDARD_DIALOG_WIDTH, STANDARD_DIALOG_HEIGHT, display);
	dialog->visible = visible;
	dialog->location.x = x;
	dialog->location.y = y;
	dialog->location.w = STANDARD_DIALOG_WIDTH;
	dialog->location.y = STANDARD_DIALOG_HEIGHT;
	dialog->font = font;
	dialog->text = text;
	//create buttons
	dialog->buttons[CONFIRM_CLICK] = malloc(sizeof(ShadedButton*));
	if(setupShadedButton(CONFIRM_X_LOCATION, DIALOG_BUTTON_VERTICAL_LOCATION, DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT, 60, 60, 60, "Confirm" ,dialog->buttons[CONFIRM_CLICK], font, 1, display ) < 0){
		puts("Failed setting up confirm button in dialog");
		return -1;
	}

	dialog->buttons[CANCEL_CLICK] = malloc(sizeof(ShadedButton*));
	if(setupShadedButton(CANCEL_X_LOCATION, DIALOG_BUTTON_VERTICAL_LOCATION, DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT, 60, 60, 60, "Cancel", dialog->buttons[CANCEL_CLICK], font, 1, display ) < 0){
		puts("Failed setting up cancel button in dialog");
		return -1;
	}
	printf("%p\n",dialog);
	if(SDL_MUSTLOCK(dialog->surface)) 
    {
        if(SDL_LockSurface(dialog->surface) < 0){
        	return -1;	
        } 
    }

	//Color in the dialog
	SDL_FillRect(dialog->surface, NULL, 0xF00FFF);
	SDL_Rect temp;
	temp.x = BUTTONBACKGROUNDOFFSET;
	temp.y = BUTTONBACKGROUNDOFFSET;
	temp.w = STANDARD_DIALOG_WIDTH - BUTTONBACKGROUNDOFFSET;
	temp.y = STANDARD_DIALOG_HEIGHT - BUTTONBACKGROUNDOFFSET;
	SDL_FillRect(dialog->surface, &temp, DIALOG_COLOR);

	//Add some nice lines to the dialog for looks
	if(SDL_MUSTLOCK(dialog->surface)) SDL_UnlockSurface(dialog->surface);
  

	return 0;
}

/*
*Draws the dialog to the screen
*	screen: The screen to draw the dialog to
*	dialog: The dialog to draw
*/
void drawDialog(Dialog * dialog, SDL_Surface * screen ){
	if(dialog->visible != 0){
		return;
	}

	drawShadedButton(dialog->buttons[CONFIRM_CLICK], dialog->surface);
	drawShadedButton(dialog->buttons[CANCEL_CLICK], dialog->surface);
	
	SDL_BlitSurface(dialog->surface, NULL, screen, &dialog->location);
	return;
}

/*
*Boolean for whether or not a give x,y pair is within the Dialog
*	dialog: The Dialog to check within
*	x: The x coordinate of the x pair
*	y: The y coordinate of the y pair
*	returns -1 if the dialog is not within the Dialog, 0 if it is
*/
int withinDialog(Dialog * dialog, const int x, const int y){
	return 0;
}


/*
*Hide the Dialog from the given screen (done by blitting white to its location)
*	dialog: The Dialog to hide from the screen  -see below
*	screen: The screen to hide the dialog from  -Seems awfully circuler this documentation doesn't it? see above
*/
void hideDialog(Dialog * dialog, SDL_Surface * screen){
	return;
}

/*
*Free memory used by this dialog (DOES NOT FREE THE ACTUAL POINTER PASSED IN. That's up to the calling context)
*	dialog: The dialog whose memory we're freeing
*/
void freeDialog(Dialog * dialog){
	SDL_FreeSurface(dialog->surface);
	int i;
	for(i=0; i < 2; i++){
		free(dialog->buttons[i]);
	}
	return;
}



