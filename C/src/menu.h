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
#include "bitfont.h"


//Menu and Button constants
//Define either right handed or left handed 
#define MENU_ON_TOP
#ifdef MENU_ON_TOP
	#define MENU_Y_START 0
	#define MENU_Y_END (SCREENHEIGHT/16) //based on 480/x = 30
	#define MENU_DIVIDER_Y MENU_Y_END
#else
	// 1/5 of the screen will be the menu (unless that turns out to be too big)
	#define MENU_Y_START (SCREENHEIGHT - (SCREENHEIGHT/16))
	#define MENU_Y_END SCREENHEIGHT
	#define MENU_DIVIDER_Y MENU_Y_START
#endif
#define MENU_WIDTH (MENU_Y_END - MENU_Y_START)
//Coloring of the menu's dividers and such
#define MENU_LINE_R 0
#define MENU_LINE_B 0
#define MENU_LINE_G 0
#define MENU_BACKCOLOR 0xD3D3D3
#define MENU_DIVIDER_WIDTH 2
#define BUTTON_WIDTH  (SCREENWIDTH/6)
#define BUTTON_HEIGHT (3*(MENU_WIDTH/4))
#define BUTTON_HORIZONTAL_OFFSET (MENU_WIDTH/8)
#define BUTTON_VERTICAL_OFFSET (BUTTON_HEIGHT/4)
#define NUMBER_OF_BUTTONS 4
#define EXIT_BUTTON_INDEX 0
#define EXIT_BUTTON_LOCATION_X (SCREENWIDTH - BUTTON_WIDTH - BUTTON_HORIZONTAL_OFFSET )
#define EXIT_BUTTON_LOCATION_Y (MENU_Y_START + BUTTON_VERTICAL_OFFSET)
//The brush button, when clicked will pop out a increase and decrease popup menu
#define BRUSH_BUTTON_INDEX 1
#define BRUSH_BUTTON_LOCATION_X BUTTONBACKGROUNDOFFSET //(EXIT_BUTTON_LOCATION_X - 2*BUTTON_WIDTH - 2*BUTTON_HORIZONTAL_OFFSET)
//The brush buttons submenu will be defined a bit here relatively:
#ifdef MENU_ON_TOP
	#define BRUSH_BUTTON_LOCATION_Y (MENU_Y_START + BUTTON_VERTICAL_OFFSET)
	#define BRUSH_SUBMENU_START MENU_Y_END
	#define BRUSH_SUBMENU_END (MENU_Y_START +  (BUTTON_HEIGHT*1.2))
	#define BRUSH_INCREASE_LOCATION_Y (BRUSH_SUBMENU_START + BUTTON_VERTICAL_OFFSET + BUTTONBACKGROUNDOFFSET)
	#define BRUSH_DECREASE_LOCATION_Y BRUSH_INCREASE_LOCATION_Y
#else
	#define BRUSH_BUTTON_LOCATION_Y (MENU_Y_END - BUTTON_VERTICAL_OFFSET - BUTTON_HEIGHT)
	#define BRUSH_SUBMENU_START (MENU_Y_START - BUTTON_HEIGHT*1.2)
	#define BRUSH_SUBMENU_END MENU_Y_START
	#define BRUSH_INCREASE_LOCATION_Y (BRUSH_SUBMENU_START - BUTTON_HEIGHT - BUTTON_VERTICAL_OFFSET - BUTTONBACKGROUNDOFFSET)
	#define BRUSH_DECREASE_LOCATION_Y BRUSH_INCREASE_LOCATION_Y
#endif
#define BRUSH_INCREASE_INDEX 2
#define BRUSH_INCREASE_LOCATION_X (BRUSH_BUTTON_LOCATION_X + BUTTON_HORIZONTAL_OFFSET + BUTTONBACKGROUNDOFFSET)
#define BRUSH_DECREASE_INDEX 3
#define BRUSH_DECREASE_LOCATION_X ((BRUSH_BUTTON_LOCATION_X + BUTTON_WIDTH) - (BUTTON_WIDTH/3) - BUTTON_HORIZONTAL_OFFSET - BUTTONBACKGROUNDOFFSET)



typedef struct{
	ShadedButton *buttons[NUMBER_OF_BUTTONS];
	SDL_Rect * divider;
	Uint8 r;
	Uint8 g;
	Uint8 b;
	BitFont * font;
	int subMenuActive;		//Boolean for if any of the menu's submenus are active
} Menu;

/*Creates all the buttons for the menu and orientes them on the screen
*	menu: The Menu struct to configure
*/
int setupMenu(Menu * menu, BitFont * font);

/*
*Free's the memory malloced by menu
*/
void freeMenu(Menu * menu);

/*Checks if the x coordinate is within the menu
*	x: The x coordinate to compare within the menu
*	returns -1 if false, 0 if true
*/
int withinMenu(Menu * menu, const int x, const int y);

/*Checks to see if the given (x,y) pair is within any buttons. 
*	menu: The Menu struct to check within
*	x: The x coordinate of the (x,y) pair to check against
*	y: The y coordinate of the (x,y) pair to check against
*	returns -1 if not within any, the index of the button the (x,y) is within otherwise
*/
int checkButtons(const Menu * menu, const int x, const int y);

/*
*	screen: The SDL_Surface to draw the menu onto
*	menu: The Menu struct to use to figure out what to draw
*/
void drawMenu(SDL_Surface *screen, Menu * menu);


#endif