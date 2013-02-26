/*
*Ethan Eldridge
*February 12th 2013
*
*
*
*/

//Local includes
#include "conf.h"
#include "bitfont.h"


int setupBitFont(BitFont * font){
	//Load the image
	SDL_Surface * temp;
	temp = SDL_LoadBMP(BITMAPFILE);
	if(temp == NULL){
		perror("Failed loading BMP files");
		return -1;
	}else{
		//Optimize the image for fast blitting
		if(SDL_SetColorKey(temp, SDL_SRCCOLORKEY | SDL_RLEACCEL, 0x000000) <0){
			puts("Failed setting the color key");
			return -1;
		}
		font->bitmap = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
	}

	buildFontFromImage(font);

	return 0;
}

void buildFontFromImage(BitFont * font){

}

void freeBitFont(BitFont * font){
	
}