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
#include "bitfont.h"

//Found on a forum http://www.cplusplus.com/forum/general/9063/ Thank you Disch.
SDL_Surface* createSurface(Uint32 flags,int width,int height,const SDL_Surface* display){
  // 'display' is the surface whose format you want to match
  const SDL_PixelFormat fmt = *(display->format);
  return SDL_CreateRGBSurface(flags,width,height,
                  fmt.BitsPerPixel,
                  fmt.Rmask,fmt.Gmask,fmt.Bmask,fmt.Amask );
}


int setupShadedButton(Sint16 x, Sint16 y, Uint16 w, Uint16 h, Uint8 r, Uint8 g, Uint8 b, char * text,ShadedButton * button, BitFont * font, const int visible, const SDL_Surface * display ){
	button->x = x;
	button->y = y;
	button->width = w + 2*BUTTONBACKGROUNDOFFSET;
	button->height = h + 2*BUTTONBACKGROUNDOFFSET;
	button->r = r;
	button->g = g;
	button->b = b;
	button->clicked = 0;
	button->hover = 0;
	button->visible = visible;
	button->text = malloc(sizeof(char)*strlen(text)+1);
	button->font = font;
	button->surface = createSurface(SDL_SWSURFACE, button->width,button->height,display);
	strcpy(button->text,text);
	return 0;
}


void freeButton(ShadedButton * button){
	//Free the text that was malloced
	SDL_FreeSurface(button->surface);
	free(button->text);
	//Note that this assumes the button's memory was malloced
	free(button);
}


int within(const ShadedButton * button, const int x, const int y){
	if(button == NULL){
		return -1;
	}
	if(x < button->x + button->width && x > button->x && y > button->y && y < button->y + button->height){
		return 0;
	}
	return -1;
}

void hideButton(ShadedButton * button, SDL_Surface * screen){
	SDL_Rect  backRect;
	backRect.x = button->x      - BUTTONBACKGROUNDOFFSET;
	backRect.y = button->y      - BUTTONBACKGROUNDOFFSET;
	backRect.w = button->width  + 2*BUTTONBACKGROUNDOFFSET;
	backRect.h = button->height + 2*BUTTONBACKGROUNDOFFSET;	

	Uint32 btnBackColor = SDL_MapRGB( screen->format, 255, 255, 255);

	SDL_FillRect(screen, &backRect, btnBackColor);

	button->visible = 0;

}

void drawShadedButton(ShadedButton * button, SDL_Surface *screen){
	if(button->visible != 1){
		return;
	}

	//We don't need to lock the screen for blitting
	SDL_Rect * backRect = malloc(sizeof(SDL_Rect));
	if(backRect == NULL){
		//Well, I'm out if we can't contain 4 16 bit ints in memory
		return;
	}
	backRect->x = button->x      - BUTTONBACKGROUNDOFFSET;
	backRect->y = button->y      - BUTTONBACKGROUNDOFFSET;
	backRect->w = button->width  + 2*BUTTONBACKGROUNDOFFSET;
	backRect->h = button->height + 2*BUTTONBACKGROUNDOFFSET;

	SDL_Rect foreRect;
	if(button->clicked==1){
		//Shift the 'top' part of the button down when we click it
		foreRect.x = BUTTONBACKGROUNDOFFSET ;
		foreRect.y = BUTTONBACKGROUNDOFFSET/2;
		foreRect.w = button->width;
		foreRect.h = button->height + BUTTONBACKGROUNDOFFSET/2;
	}else{ //Yes we could do a foreRect = {}... but if someone changes SDL's struct order thats screwed
		foreRect.x = BUTTONBACKGROUNDOFFSET;
		foreRect.y = BUTTONBACKGROUNDOFFSET;
		foreRect.w = button->width - 2*BUTTONBACKGROUNDOFFSET;
		foreRect.h = button->height - 2*BUTTONBACKGROUNDOFFSET;
	}
	
	//Both the foreground and the shadow of the button
	Uint32 btnForeColor;  
	Uint32 btnBackColor;

	//Create the actual Uint32 colors for the button
	if(button->clicked!=0){
		btnBackColor = SDL_MapRGB( screen->format, button->r, button->g, button->b );
	    btnForeColor = SDL_MapRGB( screen->format, button->r*2+1 % 256, button->g*2+1 % 256, button->b*2+1%256 );
	}else if(button->hover!=0){
		btnForeColor = SDL_MapRGB( screen->format, button->r, button->g, button->b );
		btnBackColor = SDL_MapRGB( screen->format, button->r*3+1 % 256, button->g*3+1 % 256, button->b*3+1%256 );
	}else if(button->visible!=0){
	    btnForeColor = SDL_MapRGB( screen->format, button->r, button->g, button->b );
	    btnBackColor = SDL_MapRGB( screen->format, button->r*2+1 % 256, button->g*2+1 % 256, button->b*2+1%256 );
	}else{
		//The button must be rendered invisible (never called right now due to early return)
		btnForeColor = SDL_MapRGB( screen->format, 255, 255, 255);
	    btnBackColor = SDL_MapRGB( screen->format, 255, 255, 255);
	}

    if(SDL_FillRect(button->surface, NULL, btnBackColor) <0 ){
    	perror("btnBackColor");
    }
    if(SDL_FillRect(button->surface, &foreRect, btnForeColor) <0 ){
    	perror("btnForeColor");
    }

    SDL_BlitSurface(button->surface, NULL, screen, backRect );

    free(backRect);

    //Text handling should go here since it should be drawn on top of the rest of the button
  	//Not implemented yet because I haven't decided how to handle fonts yet
    
}
