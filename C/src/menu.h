#ifndef __MENU_H__
	#define __MENU_H__

/*
*Ethan Eldridge
*February 9th 2013
*
*
*Menu.h defines function prototypes for the menu functions
*Also defines constants such as handedness, locations of buttons and the like
*
*/

//System includes
#include <SDL.h>
#include <SDL/SDL.h>

//local includes
#include "conf.h"
#include "button.h"

//Menu and Button constants
//Define either right handed or left handed 
#define RIGHT_HANDED
#ifdef RIGHT_HANDED
	#define MENU_X_START 0
	#define MENU_X_END (SCREENWIDTH/5)
	#define MENU_DIVIDER MENU_X_END
#else
	// 1/4 of the screen will be the menu (unless that turns out to be too big)
	#define MENU_X_START (4*(SCREENWIDTH/5))
	#define MENU_X_END SCREENWIDTH
	#define MENU_DIVIDER MENU_X_START
#endif
//Coloring of the menu's dividers and such
#define MENU_LINE_R 0
#define MENU_LINE_B 0
#define MENU_LINE_G 0
#define MENU_BACKCOLOR 0xD3D3D3

#define MENU_DIVIDER_WIDTH 2;
#define MENU_WIDTH (MENU_X_END - MENU_X_START)
#define BUTTON_WIDTH  (3*(MENU_WIDTH/4))
#define BUTTON_HEIGHT 30
#define BUTTON_HORIZONTAL_OFFSET (MENU_WIDTH/8)
#define BUTTON_VERTICAL_OFFSET (BUTTON_HEIGHT/4)
#define NUMBER_OF_BUTTONS 1
#define EXIT_BUTTON_INDEX 0
#define EXIT_BUTTON_LOCATION (SCREENHEIGHT - BUTTON_HEIGHT - BUTTON_VERTICAL_OFFSET)



typedef struct{
	ShadedButton *buttons[NUMBER_OF_BUTTONS];
	SDL_Rect * divider;
	Uint8 r;
	Uint8 g;
	Uint8 b;
} Menu;

/*Creates all the buttons for the menu and orientes them on the screen
*	menu: The Menu struct to configure
*/
int setupMenu(Menu * menu);

/*
*	screen: The SDL_Surface to draw the menu onto
*	menu: The Menu struct to use to figure out what to draw
*/
void drawMenu(SDL_Surface *screen, Menu * menu);


#endif