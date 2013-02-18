/*
*Ethan Eldridge
*February 9th 2013
*
*
*Defines the menu implementation. Dealing with buttons, drawing and where everything goes is handled here 
*/

//System includes
#include "stdlib.h" //for malloc

//Local includes
#include "menu.h"
#include "button.h"
#include "bitfont.h"
#include "conf.h"

//0 for win, -1 for fail
int setupMenu(Menu * menu, BitFont * font){
	if(menu == NULL){
		puts( "Menu pointer is null");
		return -1;
	}

	//setup the divider between menu and the work pane
	menu->divider = malloc(sizeof(SDL_Rect));
	if(menu->divider == NULL){
		puts("memory not allocated for divider");
		return -1;
	}
	menu->divider->x = 0;
	menu->divider->y = MENU_DIVIDER_Y;
	menu->divider->w = SCREENWIDTH;
	menu->divider->h = MENU_DIVIDER_WIDTH;
	menu->subMenuActive = 0;
	menu->font = font;

	//Decide on the color:
	menu->r = MENU_LINE_R;
	menu->g = MENU_LINE_G;
	menu->b = MENU_LINE_B;

	//Exit button details
	menu->buttons[EXIT_BUTTON_INDEX] = malloc(sizeof(ShadedButton));
    if(setupShadedButton(EXIT_BUTTON_LOCATION_X,EXIT_BUTTON_LOCATION_Y,  BUTTON_WIDTH, BUTTON_HEIGHT, 60, 40, 40, "Exit",menu->buttons[EXIT_BUTTON_INDEX],font,1) < 0){
        puts("Failed creating exit button");
        return -1;
    }

    //Brush button
    menu->buttons[BRUSH_BUTTON_INDEX] = malloc(sizeof(ShadedButton));
    if(setupShadedButton(BRUSH_BUTTON_LOCATION_X,BRUSH_BUTTON_LOCATION_Y,BUTTON_WIDTH,BUTTON_HEIGHT,60,60,60,"Brush",menu->buttons[BRUSH_BUTTON_INDEX],font,1) < 0){
    	puts("failed creating brush menu button");
    	return -1;
    }

    //Brush size button details
    menu->buttons[BRUSH_INCREASE_INDEX] = malloc(sizeof(ShadedButton));
    if(setupShadedButton(BRUSH_INCREASE_LOCATION_X, BRUSH_INCREASE_LOCATION_Y,BUTTON_WIDTH/3,BUTTON_HEIGHT, 60,40,40,"Brush+",menu->buttons[BRUSH_INCREASE_INDEX],font,0) < 0){
    	puts("failed creating brush+ button");
    	return -1;
    }

    menu->buttons[BRUSH_DECREASE_INDEX] = malloc(sizeof(ShadedButton));
    if(setupShadedButton(BRUSH_DECREASE_LOCATION_X,BRUSH_DECREASE_LOCATION_Y,BUTTON_WIDTH/3,BUTTON_HEIGHT,60,40,40,"Brush-",menu->buttons[BRUSH_DECREASE_INDEX],font,0) < 0){
    	puts("failed creating brush- button");
    	return -1;
    }


	return 0;
}

void freeMenu(Menu * menu){
	//Divider
	free(menu->divider);

	//Free the buttons
	int i;
	for(i=0; i < NUMBER_OF_BUTTONS; i++){
		freeButton(menu->buttons[i]);
	}
}

int withinMenu(Menu * menu, const int x, const int y){
	//This function now must also include pop-up menus
	if(menu->subMenuActive != 0){
		if((BRUSH_BUTTON_LOCATION_X - BUTTONBACKGROUNDOFFSET) < x  && x < (BRUSH_BUTTON_LOCATION_X - BUTTONBACKGROUNDOFFSET + BUTTON_WIDTH + BUTTONBACKGROUNDOFFSET*2)){
			if(BRUSH_SUBMENU_START < y  && y < BRUSH_SUBMENU_START + (BRUSH_SUBMENU_END - BRUSH_SUBMENU_START + 2*BUTTON_HEIGHT)+ BUTTON_VERTICAL_OFFSET ){
				return 0;
			}
		}
	}
	#ifdef MENU_ON_TOP
		if(y < MENU_Y_END){
			return 0;
		}
	#else
		if(y < MENU_Y_START){
			return 0;
		}
	#endif
	return -1;
}

int checkButtons(const Menu * menu, const int x, const int y){
	int b; 
	for(b=0; b < NUMBER_OF_BUTTONS; b++){
		if( within(menu->buttons[b], x,y) == 0){
			return b;
		}
	}
	return -1;
}

void drawMenu(SDL_Surface *screen, Menu * menu){
	//Draw the background color
	SDL_Rect background = {0,MENU_Y_START,SCREENWIDTH,MENU_WIDTH};
	Uint32 backColor = MENU_BACKCOLOR;
	if(SDL_FillRect(screen, &background, backColor) < 0){
		puts("failed to color menu background in");
	}

	//Draw the lining of the interface
	Uint32 divColor = SDL_MapRGB( screen->format, menu->r, menu->g, menu->b );
	if(SDL_FillRect(screen, menu->divider, divColor) < 0){
		puts("failed on the menu divider");
	}


	SDL_Rect submenu,submenuoutline;  
	if(menu->subMenuActive != 0){
		//Draw the appropriate submenu (check for the active state)
		switch(menu->subMenuActive){
			case BRUSH_BUTTON_INDEX:
				//draw the submenu for brush		
				submenu.x = BRUSH_BUTTON_LOCATION_X;
				submenu.y = BRUSH_SUBMENU_START;
				submenu.w = BUTTON_WIDTH;
				submenu.h = (BRUSH_SUBMENU_END - BRUSH_SUBMENU_START + 2*BUTTON_HEIGHT);
				submenuoutline.x = submenu.x - BUTTONBACKGROUNDOFFSET;
				submenuoutline.y = submenu.y;
				submenuoutline.w = BUTTON_WIDTH + BUTTONBACKGROUNDOFFSET*2;
				submenuoutline.h = submenu.h + BUTTON_VERTICAL_OFFSET;
				if(menu->buttons[BRUSH_BUTTON_INDEX]->clicked != 0){
					//This will clear it, but we'll lose the pixels underneath if they drew
					//There must be a way to do this... (maybe copy the old pixels to something?)
					divColor = 0xFFFFFF;
					backColor = 0xFFFFFF;
					menu->buttons[BRUSH_INCREASE_INDEX]->visible = 1;
                	menu->buttons[BRUSH_DECREASE_INDEX]->visible = 1;
				}
				if(SDL_FillRect(screen,&submenuoutline,divColor) < 0 ){
					puts("failed making submenu");
					}
				if(SDL_FillRect(screen,&submenu,backColor) <  0 ){
					puts("failed making submenu backcolor");
				}
				break;
			default:
				break;
		}
	}



	//Draw all the buttons
	int i;
	for(i=0; i < NUMBER_OF_BUTTONS; i++){
		drawShadedButton(menu->buttons[i],screen);
	}

}