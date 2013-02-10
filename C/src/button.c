/*
*Ethan Eldridge
*February 9th 2013
*
*The purpose of this file is to provide a basic button. 
*Given an x,y it should return true if it's been clicked
*For niceness, changing color would be good as well when clicked (depressed)
*Text within the button
*/

//System Includes
#include <string.h>

//Local Includes
#include "button.h"


int setupShadedButton(Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint8 r, Uint8 g, Uint8 b, char * text,ShadedButton * button){
	button->x = x;
	button->y = y;
	button->width = w;
	button->height = h;
	button->r = r;
	button->g = g;
	button->b = b;
	button->clicked = 0;
	button->text = malloc(sizeof(char)*strlen(text)+1);
	strcpy(button->text,text);
	return 0;
}

void drawShadedButton(ShadedButton * button, SDL_Surface *screen){
	//We don't need to lock the screen for blitting
	SDL_Rect * backRect = malloc(sizeof(SDL_Rect));
	backRect->x = button->x - 5;
	backRect->y = button->y -5;
	backRect->w = button->width+5;
	backRect->h = button->height+5;

	SDL_Rect foreRect;
	if(button->clicked==1){
		//Shift the 'top' part of the button down when we click it
		foreRect.x = button->x;
		foreRect.y = button->y +5;
		foreRect.w = button->width;
		foreRect.h = button->height-5;
	}else{ //Yes we could do a foreRect = {}... but if someone changes SDL's struct order thats screwed
		foreRect.x = button->x;
		foreRect.y = button->y;
		foreRect.w = button->width;
		foreRect.h = button->height;
	}
	
	//Both the foreground and the shadow of the button
	Uint32 btnForeColor;  
	Uint32 btnBackColor;

	//Create the actual Uint32 colors for the button
    btnForeColor = SDL_MapRGB( screen->format, button->r, button->g, button->b );
    btnBackColor = SDL_MapRGB( screen->format, button->r*2+1 % 256, button->g*2+1 % 256, button->b*2+1%256 );

    if(SDL_FillRect(screen, backRect, btnBackColor) <0 ){
    	perror("btnBackColor");
    }
    if(SDL_FillRect(screen, &foreRect, btnForeColor) <0 ){
    	perror("btnForeColor");
    }

    SDL_BlitSurface(screen,&foreRect,screen,&foreRect);

    //Text handling should go here since it should be drawn on top of the rest of the button
  	//Not implemented yet because I haven't decided how to handle fonts yet
    
}
