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
	menu->divider->x = MENU_DIVIDER_X;
	menu->divider->y = 0;
	menu->divider->w = MENU_DIVIDER_WIDTH;
	menu->divider->h = SCREENHEIGHT;

	//setup the divider between the doc pane and the rest of the menu
	menu->docDivide = malloc(sizeof(SDL_Rect));
	if(menu->docDivide == NULL){
		puts("memory not allocated for doc divider");
		return -1;
	}
	menu->docDivide->x = MENU_X_START;
	menu->docDivide->y = DOCPANE_DIVIDER_Y_END;
	menu->docDivide->w = MENU_WIDTH;
	menu->docDivide->h = MENU_DIVIDER_WIDTH;
	menu->font = font;

	//Decide on the color:
	menu->r = MENU_LINE_R;
	menu->g = MENU_LINE_G;
	menu->b = MENU_LINE_B;

	//Exit button details
	menu->buttons[EXIT_BUTTON_INDEX] = malloc(sizeof(ShadedButton));
    if(setupShadedButton(MENU_X_START + BUTTON_HORIZONTAL_OFFSET, EXIT_BUTTON_LOCATION, BUTTON_WIDTH, BUTTON_HEIGHT, 60, 40, 40, "Exit",menu->buttons[EXIT_BUTTON_INDEX],font) < 0){
        puts("Failed creating exit button");
        return -1;
    }

    //Brush size button details
    menu->buttons[BRUSH_INCREASE_INDEX] = malloc(sizeof(ShadedButton));
    if(setupShadedButton(MENU_X_START + BUTTON_HORIZONTAL_OFFSET, BRUSH_INCREASE_LOCATION,BUTTON_WIDTH/3,BUTTON_HEIGHT, 60,40,40,"Brush+",menu->buttons[BRUSH_INCREASE_INDEX],font) < 0){
    	puts("failed creating brush+ button");
    	return -1;
    }

    menu->buttons[BRUSH_DECREASE_INDEX] = malloc(sizeof(ShadedButton));
    if(setupShadedButton(MENU_X_START + BUTTON_WIDTH/2 + 2*BUTTON_HORIZONTAL_OFFSET,BRUSH_DECREASE_LOCATION,BUTTON_WIDTH/3,BUTTON_HEIGHT,60,40,40,"Brush-",menu->buttons[BRUSH_DECREASE_INDEX],font) < 0){
    	puts("failed creating brush- button");
    	return -1;
    }

	return 0;
}

int withinMenu(const int x){
	#ifdef RIGHT_HANDED
		if(x < MENU_DIVIDER_X){
			return 0;
		}
	#else
		if(x > MENU_DIVIDER_X){
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
	SDL_Rect background = {MENU_X_START,0,MENU_WIDTH,SCREENHEIGHT};
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
	//Draw the scroll area for the document selecting pane (docPane
	if(SDL_FillRect(screen,menu->docDivide,divColor) < 0){
		puts("failed on doc divider");
	}else{
		SDL_BlitSurface(screen,menu->docDivide,screen,menu->docDivide);
	}

	SDL_Rect docScrollLeft = {DOC_SCROLL_X, 0, MENU_DIVIDER_WIDTH, DOCPANE_DIVIDER_Y_END};
	if(SDL_FillRect(screen,&docScrollLeft,divColor) < 0 ){
		puts("failed on doc pane scroll side");
	}else{
		SDL_BlitSurface(screen,&docScrollLeft,screen,&docScrollLeft);
	}


	//Draw all the buttons
	int i;
	for(i=0; i < NUMBER_OF_BUTTONS; i++){
		drawShadedButton(menu->buttons[i],screen);
	}

}