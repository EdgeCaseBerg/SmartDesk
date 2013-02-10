

#include "menu.h"
#include "button.h"

//0 for win, -1 for fail
int setupMenu(Menu * menu){
	if(menu == NULL){
		puts( "Menu pointer is null");
		return -1;
	}

	menu->buttons[EXITBUTTONINDEX] = malloc(sizeof(ShadedButton));
    if(setupShadedButton(5, 5, 50, 25, 25, 25, 25, "poop",menu->buttons[EXITBUTTONINDEX]) < 0){
        puts("Failed creating exit button");
        return -1;
    }

	return 0;
}
