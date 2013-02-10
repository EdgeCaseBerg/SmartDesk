/*
*Ethan Eldridge
*February 9th 2013
*
*
*Defines the menu implementation. Dealing with buttons, drawing and where everything goes is handled here 
*/


#include "menu.h"
#include "button.h"

//0 for win, -1 for fail
int setupMenu(Menu * menu){
	if(menu == NULL){
		puts( "Menu pointer is null");
		return -1;
	}


	//Exit button details
	menu->buttons[EXITBUTTONINDEX] = malloc(sizeof(ShadedButton));
    if(setupShadedButton(MENUXSTART + BUTTONHORIZONTALOFFSET, EXITBUTTONLOCATION, BUTTONWIDTH, BUTTONHEIGHT, 60, 40, 40, "poop",menu->buttons[EXITBUTTONINDEX]) < 0){
        puts("Failed creating exit button");
        return -1;
    }

	return 0;
}

void drawMenu(SDL_Surface *screen, Menu * menu){
	drawShadedButton(menu->buttons[EXITBUTTONINDEX],screen);

}