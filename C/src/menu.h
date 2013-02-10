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
#define RIGHTHANDED
#ifdef RIGHTHANDED
	#define MENUXSTART 0
	#define MENUXEND SCREENWIDTH/4
#else
	// 1/4 of the screen will be the menu (unless that turns out to be too big)
	#define MENUXSTART 3*(SCREENWIDTH/4)
	#define MENUXEND SCREENWIDTH
#endif
#define NUMBEROFBUTTONS 1
#define EXITBUTTONINDEX 0


typedef struct{
	ShadedButton *buttons[NUMBEROFBUTTONS];
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