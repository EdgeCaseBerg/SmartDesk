/*
*Ethan Eldridge
*January 30 2013
*
*The purpose of this file is to provide graphics to the program
*and to define how to read the memory mapped file
*
* To be honest I've gotta set up some type of protocol for reading from the
*thing. It's easy enough to sync the memory on each call, but how do I know
*if I'm reading 'too fast' and the networking isn't giving me anything? There
*needs to be some null value coming in from the file or something. Or a global
*count of how many bytes I've written so far. Then graphics reads up to that, if
*the reading count is higher than the writting count than the writer looped around
*the file and began writing at the beginning, so we need to reflect that...
*
* I could have the writer write the offset it's on at the 0th byte so the reader 
*could just msync, read that, and then continue on... 
*/

//System includes
#include <sys/mman.h>		//memory mapped files
#include <stdlib.h>			//malloc and free
#include <stdio.h>			//puts
#include <math.h>           //sqrt (compile with -lm flag)

//Local includes
#include "conf.h"
#include "graphics.h"
#include "menu.h"
#include "bitfont.h"

//Global variables
int mouseDown = 0;			           //True/False for if the mouse is 
int brushSize = 3;                     //Size of the brush
int static buffered[CLICKBUFFERSIZE];  //Buffer to hold x,y coordinates to draw
int static bufferPointer = 0;          //When to stop reading from the buffered

//Frees memory used by GraphicModule
void freeGraphicModule(GraphicModule * module){
    //Bit Font engine
    free(module->font);
    freeMenu(module->menu);
    free(module->menu);
}

//Returns -1 on failure, 0 on success, sets up the module
int setupGraphicModule(int fd, GraphicModule * module){

	//Zero the buffered array
	memset(buffered,0,sizeof(buffered));

	//Setup the void pointer for the mapped file
	module->memShareAddr = (void*)malloc(sizeof(void*));
	module->memShareFD = -1;
	if(module->memShareAddr == NULL){
		puts("Failed allocating memory for Graphics Module");
		return -1;
	}

	module->memShareAddr = mmap(NULL, MEMSHARESIZE, PROT_READ, MAP_SHARED, fd, 0);
	if(module->memShareAddr == MAP_FAILED){
		perror("memsharegraphic");
		puts("Failed to map memory share to network module");
		return -1;
	}
	module->memShareFD = fd;
	msync(module->memShareAddr,sizeof(int),MS_SYNC|MS_INVALIDATE);

	//Set up the graphics
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ){
		puts("Failed to initalize video");
		return -1;
	}

	module->screen = SDL_SetVideoMode(SCREENWIDTH, SCREENHEIGHT, SCREENDEPTH, SDL_HWSURFACE);
	if(module->screen == NULL){
		puts("Failed to set video mode");
		SDL_Quit();
		return -1;
	}
   
    //Load the bitmap engine within the module
    
    module->font = malloc(sizeof(BitFont*));
    if(setupBitFont(module->font) < 0){
        puts("Failed to create the bitmap engine");
        SDL_Quit();
        return -1;
    }

    if(loadUI(module) < 0){
        puts("Failed to load User Interface");
        SDL_Quit();
        return -1;
    }

    //Set the stop flag
    module->stopFlag = 0;

    SDL_WM_SetCaption( "Smart Desk | Interactive Learning Software", NULL);

	return 0;
}

void smoothPath(Uint16 x, Uint16 y, Sint16 xrel, Sint16 yrel){
    //The relative x,y tell us how far away from the previous x,y we were.
    //x-xrel gives us the previous coordinate
    //We can use this to define a path between the points and fill in holes
    float dist = sqrt((x-(x-xrel))*(x-(x-xrel)) + (y-(y-yrel))*(y-(y-yrel)));

    //Consider making these static or pre-declared for time saving
    float xSteps = xrel/dist;
    float ySteps = yrel/dist;

    int i = 1;
    //We need to guard against seg faults via the buffer size
    for(; i < dist && bufferPointer < CLICKBUFFERSIZE -2; i++){
        buffered[bufferPointer] = (int)(x - xSteps*i);
        buffered[bufferPointer+1] = (int)(y - ySteps*i);
        bufferPointer= bufferPointer+2;

    }
    

}

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 colour;  

	//Convert RGB to a color with the proper screen format 
    colour = SDL_MapRGB( screen->format, r, g, b );
  
  	//Set the pixels for 32 bit depth (if want different depth will have to mess)
    pixmem32 = (Uint32*) screen->pixels  + y + x;
    *pixmem32 = colour;
}

void drawBuffered(SDL_Surface *screen){
	if(SDL_MUSTLOCK(screen)) 
    {
        if(SDL_LockSurface(screen) < 0){
        	return;	
        } 
    }

    int i = 0;
    int x,y=0;
    int drawnY,drawnX;
    int ylimit = SCREENHEIGHT*screen->pitch/BITSPERPIXEL; //ylimit is307200
    for(; i < bufferPointer && i < CLICKBUFFERSIZE; i=i+2){
    	//Each odd number is an x, each even is a y
        for(x=0; x < brushSize; x++ ){
            for(y=0; y < brushSize; y++){
                drawnY=(buffered[i+1]+y)*screen->pitch/BITSPERPIXEL;
                drawnX=buffered[i]+x;
                if(drawnY < ylimit && drawnX < SCREENWIDTH)
                    setpixel(screen, drawnX,drawnY,0,0,0);        
            }
        }
    }
    //Done drawing reset:
    bufferPointer=0;

    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  
    SDL_Flip(screen); 
}

void drawUI(GraphicModule * module){
    if(SDL_MUSTLOCK(module->screen)) 
    {
        if(SDL_LockSurface(module->screen) < 0){
            return;
        }
    }

    //Ask the menu to draw itself please
    drawMenu(module->screen,module->menu);

    if(SDL_MUSTLOCK(module->screen)) SDL_UnlockSurface(module->screen);
  
    SDL_Flip(module->screen); 
}

void clearScreen(SDL_Surface* screen){ 
    if(SDL_MUSTLOCK(screen)) 
    {
        if(SDL_LockSurface(screen) < 0){
        	return;
        }
    }

    int x, y, ytimesw;
    
    SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
    bufferPointer = 0;
    
    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  
    SDL_Flip(screen); 
}

void runGraphics(GraphicModule * module){
    SDL_Event event;
  
    int keyQuit = 0;
    
    clearScreen(module->screen);
    
    //Main graphics event loop goes until an event causes keyquit != 0
    while(module->stopFlag == 0){
        //Loop until there are no more events to process
        while(SDL_PollEvent(&event)) {    
        	handleGraphicEvent(event, module);  
		}
        //Since smoothing is a preprocessor, if it's set to !1 then this call
        //should be optimized out by the compiler
        drawBuffered(module->screen);
        drawUI(module);   
    }

    SDL_FreeSurface(module->screen);

    SDL_Quit();
  

}

//-1 for fail 0 for good
int loadUI(GraphicModule * module){
    module->menu = (Menu*)malloc(sizeof(Menu));

    //Load the bitmap font and make the proper sprites and such

    if(module->menu == NULL){
        puts("failed allocating memory for menu");
        return -1;
    }

    if(setupMenu(module->menu,module->font)){
        puts("failed setting up menu");
        return -1;
    }

    return 0;

}

void handleGraphicEvent(SDL_Event  event, GraphicModule * module){
	//Giant Case to handle all events
	switch (event.type){
        case SDL_QUIT:
	       	//Halt the execution of the graphics
	       	module->stopFlag = 1;
	       	break;
	    case SDL_KEYDOWN:
	    	handleKeyEvent(event,module);
	    	break;
	    case SDL_MOUSEBUTTONDOWN:
	    	mouseDown = 1;
            handleMouseEvent(event,module);
	    	break;
	    case SDL_MOUSEBUTTONUP:
	    	mouseDown = 0;
            handleMouseEvent(event,module);
	    	break;
	    case SDL_MOUSEMOTION:
	    	handleMouseEvent(event,module);
	    	break;
        default:
            break;
	}
}

void handleKeyEvent(SDL_Event  event, GraphicModule * module){
	switch( event.key.keysym.sym ){
		case SDLK_ESCAPE:
	    	module->stopFlag = 1;
	    	break;
        case SDLK_F1:
            clearScreen(module->screen);
            break;
        default:
            break;
	}
}

void handleButtonClick(GraphicModule * module, int buttonID){
    //Handle clicking buttons and the like
    switch(buttonID){
        case EXIT_BUTTON_INDEX:
            module->stopFlag = 1;
            break;
        case BRUSH_BUTTON_INDEX:
            if(module->menu->subMenuActive != 0){
                module->menu->subMenuActive = 0;
                hideButton(module->menu->buttons[BRUSH_INCREASE_INDEX],module->screen);
                hideButton(module->menu->buttons[BRUSH_DECREASE_INDEX],module->screen);
            }else{
                //By assigning the button id, we can determine the
                //proper sub menu to pop out
                module->menu->subMenuActive = buttonID;    
                module->menu->buttons[BRUSH_INCREASE_INDEX]->visible = 1;
                module->menu->buttons[BRUSH_DECREASE_INDEX]->visible = 1;
            }
            break;
        case BRUSH_INCREASE_INDEX:
            brushSize++;
            break;
        case BRUSH_DECREASE_INDEX:
            brushSize = brushSize < 0 ? 1 : brushSize-1;
            break;
        default:
            break;
    }

}

void handleMouseEvent(SDL_Event event, GraphicModule * module){
    //Welcome to the bottleneck, we got fun and games
    //If lagging is your issue, here you must change!
    if(withinMenu(event.motion.y) != -1){
        int button = checkButtons(module->menu, event.motion.x,event.motion.y);
        if(button != -1){
            if(mouseDown){
                //Click
                module->menu->buttons[button]->clicked = 1;
                module->menu->buttons[button]->hover = 0;
            }else{
                //Hover or finished clicking
                if(module->menu->buttons[button]->clicked == 1){
                    //We just finished clicking and are about to change back to a hover state
                    handleButtonClick(module, button);

                }
                module->menu->buttons[button]->clicked = 0;
                module->menu->buttons[button]->hover = 1;
            }
        }else{
            //Not in any buttons, make sure all of them are in their default state
            int i;
            for(i=0; i < NUMBER_OF_BUTTONS; i++){
                module->menu->buttons[i]->clicked = 0;
                module->menu->buttons[i]->hover = 0;
            }
        }
    }else{
        //Drawing mode. (Once we are loading documents some more stuff will have to go here)
        if(mouseDown){
            if(event.motion.x < SCREENWIDTH && event.motion.y < SCREENHEIGHT){
                buffered[bufferPointer] = event.motion.x;
                buffered[bufferPointer+1] = event.motion.y;
                //If smoothing is off this will be optimized out by the compiler
                if(SMOOTHING==1){
                    smoothPath(event.motion.x,event.motion.y,event.motion.xrel,event.motion.yrel);
                }
                bufferPointer = bufferPointer+2;
            }    
            if(bufferPointer > CLICKBUFFERSIZE){
                puts("aw shit ");//what a good error message for now
                bufferPointer = 0;
            }
        }
    }
}
