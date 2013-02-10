/*
*Ethan Eldridge
*February 9th 2013
*
*
*Defines the menu implementation. Dealing with buttons, drawing and where everything goes is handled here 
*/


#include "menu.h"
#include "button.h"
#include "conf.h"

//0 for win, -1 for fail
int setupMenu(Menu * menu){
	if(menu == NULL){
		puts( "Menu pointer is null");
		return -1;
	}

	//setup the divider
	menu->divider = malloc(sizeof(SDL_Rect));
	if(menu == NULL){
		puts("memory not allocated for divider");
		return -1;
	}
	menu->divider->x = MENU_DIVIDER;
	menu->divider->y = 0;
	menu->divider->w = MENU_DIVIDER_WIDTH;
	menu->divider->h = SCREENHEIGHT;

	//Decide on the color:
	menu->r = MENU_LINE_R;
	menu->g = MENU_LINE_G;
	menu->b = MENU_LINE_B;

	//Exit button details
	menu->buttons[EXIT_BUTTON_INDEX] = malloc(sizeof(ShadedButton));
    if(setupShadedButton(MENU_X_START + BUTTON_HORIZONTAL_OFFSET, EXIT_BUTTON_LOCATION, BUTTON_WIDTH, BUTTON_HEIGHT, 60, 40, 40, "poop",menu->buttons[EXIT_BUTTON_INDEX]) < 0){
        puts("Failed creating exit button");
        return -1;
    }

	return 0;
}

void drawMenu(SDL_Surface *screen, Menu * menu){
	//Draw the background color
	SDL_Rect background = {0,0,MENU_WIDTH,SCREENHEIGHT};
	Uint32 backColor = MENU_BACKCOLOR;
	if(SDL_FillRect(screen, &background, backColor) < 0){
		puts("failed to color menu background in");
	}else{
		SDL_BlitSurface(screen,&background,screen,&background);	
	}
	

	//Draw the lining of the interface
	Uint32 divColor = SDL_MapRGB( screen->format, menu->r, menu->g, menu->b );
	if(SDL_FillRect(screen, menu->divider, divColor) < 0){
		puts("failed on the menu divider");
	}else{
		SDL_BlitSurface(screen,menu->divider,screen,menu->divider);
	}


	//Draw all the buttons
	drawShadedButton(menu->buttons[EXIT_BUTTON_INDEX],screen);

}