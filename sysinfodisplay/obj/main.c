/*****************************************************************************
* | File      	:	main.c
* | Author      :   Shawn Fang
* | Function    :	CPU temperature display to 0.9 inch OLED
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


// cpu usage

typedef struct cpu_occupy_          //定义一个cpu occupy的结构体
{
    char name[20];                  //定义一个char类型的数组名name有20个元素
    unsigned int user;              //定义一个无符号的int类型的user
    unsigned int nice;              //定义一个无符号的int类型的nice
    unsigned int system;            //定义一个无符号的int类型的system
    unsigned int idle;              //定义一个无符号的int类型的idle
    unsigned int iowait;
    unsigned int irq;
    unsigned int softirq;
}cpu_occupy_t;

double cal_cpuoccupy (cpu_occupy_t *o, cpu_occupy_t *n)
{
    double od, nd;
    double id, sd;
    double cpu_use ;

    od = (double) (o->user + o->nice + o->system +o->idle+o->softirq+o->iowait+o->irq);//第一次(用户+优先级+系统+空闲)的时间再赋给od
    nd = (double) (n->user + n->nice + n->system +n->idle+n->softirq+n->iowait+n->irq);//第二次(用户+优先级+系统+空闲)的时间再赋给od

    id = (double) (n->idle);    //用户第一次和第二次的时间之差再赋给id
    sd = (double) (o->idle) ;    //系统第一次和第二次的时间之差再赋给sd
    if((nd-od) != 0)
        cpu_use =100.0 - ((id-sd))/(nd-od)*100.00; //((用户+系统)乖100)除(第一次和第二次的时间差)再赋给g_cpu_used
    else 
        cpu_use = 0;
    return cpu_use;
}

void get_cpuoccupy (cpu_occupy_t *cpust)
{
    FILE *fd;
    char buff[256];
    cpu_occupy_t *cpu_occupy;
    cpu_occupy=cpust;

    fd = fopen ("/proc/stat", "r");
    if(fd == NULL)
    {
            perror("fopen:");
            exit (0);
    }
    fgets (buff, sizeof(buff), fd);

    sscanf (buff, "%s %u %u %u %u %u %u %u", cpu_occupy->name, &cpu_occupy->user, &cpu_occupy->nice,&cpu_occupy->system, &cpu_occupy->idle ,&cpu_occupy->iowait,&cpu_occupy->irq,&cpu_occupy->softirq);

    fclose(fd);
}

const std::string get_sys_cpu_usage()
{
    cpu_occupy_t cpu_stat1;
    cpu_occupy_t cpu_stat2;
    double rate;
    get_cpuoccupy((cpu_occupy_t *)&cpu_stat1);
    sleep(1);

    //第二次获取cpu使用情况
    get_cpuoccupy((cpu_occupy_t *)&cpu_stat2);

    //计算cpu使用率
    rate = cal_cpuoccupy ((cpu_occupy_t *)&cpu_stat1, (cpu_occupy_t *)&cpu_stat2);


	char cpu_usage[16] = {'\0'};
    sprintf(cpu_usage, "%.2f", rate);
	std::string usage = cpu_usage;
	usage = "CPU USGE: " + usage + "%";
    return usage;
}


// cpu temperature
const std::string get_cpu_temperature() {
    #define TEMP_PATH "/sys/class/thermal/thermal_zone0/temp"
    #define MAX_SIZE 32
    int fd;
    double temp = 0;
    char buf[MAX_SIZE];

    fd = open(TEMP_PATH, O_RDONLY);
    if (fd < 0) {
        fprintf(stderr, "failed to open thermal_zone0/temp\n");
        return "-1";
    }

    if (read(fd, buf, MAX_SIZE) < 0) {
        fprintf(stderr, "failed to read temp\n");
        return "-1";
    }

    close(fd);
	
    temp = atoi(buf) / 1000.0;
    char temperature[16] = {'\0'};
    sprintf(temperature, "%.2f", temp);

    std::string ret = temperature;
	ret.pop_back();
    ret = "CPU TEMP: " + ret + "'C";
    return ret; 
}

void GUI_Construct_Display_Info() {
	GUI_DisString_EN(15 , 0, "[ Monitoring ]", &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN(7 , 10, get_cpu_temperature().c_str(), &Font12, FONT_BACKGROUND, WHITE);
	GUI_DisString_EN(7 , 20, get_sys_cpu_usage().c_str(), &Font12, FONT_BACKGROUND, WHITE);
}

int main(void)
{	
	//1.System Initialization
	if(System_Init())
		exit(0);
	
	//2.show	
	OLED_Init();
	GUI_Init(OLED_WIDTH, OLED_HEIGHT);
	
	GUI_DrawRectangle(0, 0, 127, 31, WHITE, DRAW_EMPTY, DOT_PIXEL_DFT);
	GUI_DisString_EN(10, 10, "Powered By Shawn", &Font12, FONT_BACKGROUND, WHITE);
	GUI_Display();
	
	Driver_Delay_ms(2000);
	GUI_Clear();
	GUI_Display();

	while(true) {
		GUI_Clear();
		GUI_Construct_Display_Info();
		GUI_Display();
		Driver_Delay_ms(1000);
	}
	//3.System Exit
	System_Exit();
	return 0;
	
}

