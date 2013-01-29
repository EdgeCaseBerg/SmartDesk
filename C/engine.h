#ifndef __ENGINE_H__
	#define __ENGINE_H__


//Run the basic network accept loop and update the structure holding information
void processNewData();
//Update the graphics to represent the new information
void updateGraphics();
//check for mouse clicks and handle it
void processUserInput();
//Update if transmitting to another desk
void updateRemote();


#endif