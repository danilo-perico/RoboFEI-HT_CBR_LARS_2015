/*
 * main.cpp
 *
 *  Created on: 2011. 1. 4.
 *      Author: robotis
 */

#include <unistd.h>
#include <string.h>
#include <libgen.h>

#include "Camera.h"
#include "Point.h"
#include "mjpg_streamer.h"
#include "minIni.h"
#include "LinuxCamera.h"
#include "ColorFinder.h"

#include "Action.h"
#include "Head.h"
#include "Walking.h"
#include "MX28.h"
#include "MotionManager.h"
#include "LinuxMotionTimer.h"
#include "LinuxCM730.h"
#include "LinuxActionScript.h"

#define INI_FILE_PATH       "../../../../Data/config.ini"

//#ifdef MX28_1024
#define MOTION_FILE_PATH    "../../../../Data/motion_1024.bin"
//#else
//#define MOTION_FILE_PATH    "../../../../Data/motion_4096.bin"
//#endif

void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
        chdir(dirname(exepath));
}

int main(int argc, char *argv[])
{
    printf( "\n===== Action script Tutorial for DARwIn =====\n\n");

    //change_current_dir();

    Action::GetInstance()->LoadFile(MOTION_FILE_PATH);

		minIni* ini;
		if(argc==2)
			ini = new minIni(argv[1]);
		else
			ini = new minIni(INI_FILE_PATH);


	printf("Pronto \n");
    getchar();

    //////////////////// Framework Initialize ////////////////////////////
    LinuxCM730 linux_cm730("/dev/ttyUSB1");
    CM730 cm730(&linux_cm730);

	printf("Pronto 1\n");
    getchar();

    if(MotionManager::GetInstance()->Initialize(&cm730) == false)
    {
        printf("Fail to initialize Motion Manager!\n");
            return 0;
    }

	MotionManager::GetInstance()->LoadINISettings(ini);

	printf("Pronto 2\n");
    getchar();

    MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());
    LinuxMotionTimer linuxMotionTimer;
		linuxMotionTimer.Initialize(MotionManager::GetInstance());
		linuxMotionTimer.Start();
    /////////////////////////////////////////////////////////////////////

	printf("Pronto 3\n");
    getchar();

    MotionManager::GetInstance()->SetEnable(true);

	printf("Pronto 4\n");
    getchar();

    Action::GetInstance()->Start(1);    /* Init(stand up) pose */
    while(Action::GetInstance()->IsRunning()) usleep(8*1000);

    printf("Press the ENTER key to begin!\n");
    getchar();

    LinuxActionScript::ScriptStart("script.asc");
    while(LinuxActionScript::m_is_running == 1) sleep(10);

    return 0;
}
