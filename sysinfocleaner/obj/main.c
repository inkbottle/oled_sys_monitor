/*****************************************************************************
* | File      	:	main.c
* | Author      :   	Shawn Fang
* | Function    :	CPU info display to 0.9 inch OLED
* | Info        :	
*----------------
* |	This version:   V1.0
* | Date        :   2020-05-04
* | Info        :   Basic version
******************************************************************************/
#include <stdio.h>		//printf()
#include <stdlib.h>		//exit()
#include <time.h>


#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "OLED_Config.h"
#include "OLED_Driver.h"
#include "OLED_GUI.h"

int main(void)
{	
	//1.System Initialization
	if(System_Init())
		exit(0);
	
	//2.show	
	OLED_Init();
	GUI_Init(OLED_WIDTH, OLED_HEIGHT);
	
	GUI_Clear();
	GUI_Display();
	
	GUI_DrawRectangle(0, 0, 127, 31, WHITE, DRAW_EMPTY, DOT_PIXEL_DFT);
	GUI_DisString_EN(10, 10, "Now To shutdown", &Font12, FONT_BACKGROUND, WHITE);
	GUI_Display();
	
	Driver_Delay_ms(2000);
	GUI_Clear();
	GUI_Display();
	
	//3.System Exit
	System_Exit();
	return 0;
	
}

