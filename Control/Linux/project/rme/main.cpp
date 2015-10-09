/*--------------------------------------------------------------------------

****************************************************************************
* @file main.cpp
* @author Isaac Jesus da Silva - ROBOFEI-HT - FEI
* @version V0.0.1
* @created 06/10/2015
* @Modified 06/10/2015
* @e-mail isaac25silva@yahoo.com.br
* @brief Action Editor (rme)
****************************************************************************

Arquivo fonte contendo o programa que grava pontos de ações do robô

/-------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <term.h>
#include <ncurses.h>
#include <libgen.h>
#include <signal.h>
#include "cmd_process.h"
#include "PS3Controller.h"
#include "blackboard.h"

#ifdef MX28_1024
#define MOTION_FILE_PATH    "../../../Data/motion_1024.bin"
#else
#define MOTION_FILE_PATH    "../../../Data/motion_4096.bin"
#endif

using namespace Robot;

LinuxMotionTimer linuxMotionTimer;

void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
        chdir(dirname(exepath));
}

void sighandler(int sig)
{
    struct termios term;
    tcgetattr( STDIN_FILENO, &term );
    term.c_lflag |= ICANON | ECHO;
		tcsetattr( STDIN_FILENO, TCSANOW, &term );

    exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGABRT, &sighandler);
    signal(SIGTERM, &sighandler);
    signal(SIGQUIT, &sighandler);
    signal(SIGINT, &sighandler);

    int ch;
    char filename[128];
    char string[50];

	using_shared_memory();

    change_current_dir();
    if(argc < 2)
        strcpy(filename, MOTION_FILE_PATH); // Set default motion file path
    else
        strcpy(filename, argv[1]);

    /////////////// Load/Create Action File //////////////////
    if(Action::GetInstance()->LoadFile(filename) == false)
    {
        printf("Can not open \e[0;31m%s\e[0m\n", filename);
        printf("Do you want to make a new action file? (y/n) ");
        ch = _getch();
        if(ch != 'y')
        {
            printf("\n");
            return 0;
        }

        if(Action::GetInstance()->CreateFile(filename) == false)
        {
            printf("\e[1;31mFail to create %s\e[0m\n", filename);
            return 0;
        }
    }
    ////////////////////////////////////////////////////////////

  	PS3Controller_Start();

//////////////////// Framework Initialize ////////////////////////////
// ---- Open USBDynamixel -----------------------------------------------{
	bool servoComunica = false;
	bool servoConectado = false;
	int * deviceIndex = new int;
	int idServo;
	*deviceIndex = 0; 		//endereça o Servo
	while(*deviceIndex<3)// laço que percorre o servo 0, 1 e 2.
	{
		sprintf(string,"/dev/robot/servo%d", *deviceIndex);
		LinuxCM730* linux_cm730;
    	linux_cm730 = new LinuxCM730(string);
		CM730* cm730;
    	cm730 = new CM730(linux_cm730);
		if( MotionManager::GetInstance()->Initialize(cm730) == 0)
		{
			printf( "Failed to open servo%d!\n", *deviceIndex );
			if(*deviceIndex==2)  // Não encontrou nenhum
			{
				if(servoComunica)
				    printf("Conectou-se a uma placa mas não conseguiu se comunicar com o servo\n");
				else
				    printf("Não encontrou nenhuma placa do servo conectada a porta USB\n");
			        break;
			}
			*deviceIndex = *deviceIndex + 1;      // Não conecta na placa do servo e tenta a proxima porta.
		}
		else
		{
			servoComunica = true;
			printf( "Succeed to open Servo%d!\n", *deviceIndex );
			cm730->ReadByte(15, 3, &idServo, 0);
			servoConectado = idServo == 15;
			usleep(1000);
			cm730->ReadByte(15, 3, &idServo, 0);//Tenta novamente caso falhe a comunicação
			servoConectado = idServo == 15;
    		if(servoConectado)
			{
       			 	printf("Servo%d okay - Connected and communicated!\n", *deviceIndex);
			 	break;
			}
    		else
    		{
			printf("Servo wrong or not communicated!\n");
				if(*deviceIndex==2)
				{
				    printf("Conectou-se a uma placa mas não conseguiu se comunicar com o servo\n");
				    break;
				}
				*deviceIndex = *deviceIndex + 1;
			}
		}
	}
	delete deviceIndex; //desalocando da memória
	//-----------------------------------------------------------------------------}
    //////////////////// Framework Initialize ////////////////////////////
    LinuxCM730 linux_cm730(string);
    CM730 cm730(&linux_cm730);
    if(MotionManager::GetInstance()->Initialize(&cm730) == false)
    {
        printf("Fail to initialize Motion Manager!\n");
    }
	sleep(1);
//================================================================================== 


    MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());	
    linuxMotionTimer.Initialize(MotionManager::GetInstance());
	linuxMotionTimer.Stop();
	//MotionManager::GetInstance()->StopThread();
    /////////////////////////////////////////////////////////////////////
	
    DrawIntro(&cm730);

    char input[128] = {0,};
    char *token;
    int input_len;
    char cmd[80];
    int num_param;
    int iparam[30];
    char iparams[30][10];
		char s[60];
    int idx = 0;
		int apState[6]={0,0,0,0,0,0};

    while(1)
    {
        while(!kbhit(true)) ProcessPS3(&cm730,apState);
				ch = _getch();

        if(ch == 0x1b)
        {
            ch = _getch();
            if(ch == 0x5b)
            {
                ch = _getch();
                if(ch == 0x41)      // Up arrow key
                    MoveUpCursor();
                else if(ch == 0x42) // Down arrow key
                    MoveDownCursor();
                else if(ch == 0x44) // Left arrow key
                    MoveLeftCursor();
                else if(ch == 0x43) // Right arrow key
                    MoveRightCursor();
            }
        }
        else if( ch == '[' )
            UpDownValue(&cm730, -1);
        else if( ch == ']' )
            UpDownValue(&cm730, 1);
        else if( ch == '{' )
            UpDownValue(&cm730, -10);
        else if( ch == '}' )
            UpDownValue(&cm730, 10);
        else if( ch == ' ' )
            ToggleTorque(&cm730);
        else if((ch >= 'a' && ch <= 'z' ) || ( ch >= '0' && ch <= '9') )
        {

            BeginCommandMode();

            printf("%c", ch);
            if(idx < 127) input[idx++] = (char)ch;

          while(1)
            {
						while(!kbhit(true))	ProcessPS3(&cm730,apState);
            ch = _getch();
            if( ch == 0x0A ) // newline
              break;
            else if( ch == 0x7F || ch == 0x08) // delete key or bksp
              {
              if(idx > 0)
                {
                ch = 0x08; // bksp
                printf("%c", ch);
                ch = ' ';
                printf("%c", ch);
                ch = 0x08;// bksp
                printf("%c", ch);
                input[--idx] = 0;
                }
              }
            else if( ch == ' ' || ch == '-' || (ch >= 'a' && ch <= 'z' ) || ( ch >= '0' && ch <= '9') )
              {
              if(idx < 127) 
                {
                printf("%c", ch);
                input[idx++] = (char)ch;
                }
             }
           }

            fflush(stdin);
            input_len = strlen(input);
            if(input_len > 0)
            {
                token = strtok( input, " " );
                if(token != 0)
                {
                    strcpy( cmd, token );
                    token = strtok( 0, " " );
                    num_param = 0;
                    while(token != 0)
                    {
                        iparam[num_param] = atoi(token);
                        strncpy(iparams[num_param++],token,10);
												token = strtok( 0, " " );
                    }

                    if(strcmp(cmd, "exit") == 0)
                    {
                        if(AskSave() == false)
                            break;
                    }
                    else if(strcmp(cmd, "re") == 0)
                        DrawPage();
                    else if(strcmp(cmd, "help") == 0)
                        HelpCmd();
                    else if(strcmp(cmd, "n") == 0)
                        NextCmd();
                    else if(strcmp(cmd, "b") == 0)
                        PrevCmd();						
                    else if(strcmp(cmd, "time") == 0)
                        TimeCmd();
                    else if(strcmp(cmd, "speed") == 0)
                        SpeedCmd();
                    else if(strcmp(cmd, "mon") == 0)
                        {
												linuxMotionTimer.Start();
												cm730.m_bIncludeTempData = true;
												cm730.MakeBulkReadPacket();// force packet to get rebuilt
												while(!kbhit(true))
													{
													MonitorServos(&cm730);
													usleep(10000);
													}
												linuxMotionTimer.Stop();
												cm730.m_bIncludeTempData = false;
												cm730.MakeBulkReadPacket();// force packet to be rebuilt
												GoToCursor(CMD_COL, CMD_ROW);
												}
                    else if(strcmp(cmd, "page") == 0)
                    {
                        if(num_param > 0)
                            PageCmd(iparam[0]);
                        else
                            PrintCmd("Need parameter");
                    }
                    else if(strcmp(cmd, "play") == 0)
                    {
                        if(num_param > 0)
                          PlayCmd(&cm730, iparam[0]);
                        else
													PlayCmd(&cm730,IndexPage());
                    }
                    else if(strcmp(cmd, "set") == 0)
                    {
                        if(num_param > 0)
                            SetValue(&cm730, iparam[0]);
                        else
                            PrintCmd("Need parameter");
                    }
                    else if(strcmp(cmd, "list") == 0)
                        ListCmd();
                    else if(strcmp(cmd, "on") == 0)
                        OnOffCmd(&cm730, true, num_param, iparam,iparams);
                    else if(strcmp(cmd, "off") == 0)
                        OnOffCmd(&cm730, false, num_param, iparam,iparams);
                    else if(strcmp(cmd, "w") == 0)
                    {
                        if(num_param > 0)
                            WriteStepCmd(iparam[0]);
                        else
                            PrintCmd("Need parameter");
                    }
                    else if(strcmp(cmd, "d") == 0)
                    {
                        if(num_param > 0)
                            DeleteStepCmd(iparam[0]);
                        else
                            PrintCmd("Need parameter");
                    }
                    else if(strcmp(cmd, "i") == 0)
                    {
                        if(num_param == 0)
                            InsertStepCmd(0);
                        else
                            InsertStepCmd(iparam[0]);
                    }
                    else if(strcmp(cmd, "m") == 0)
                    {
                        if(num_param > 1)
                            MoveStepCmd(iparam[0], iparam[1]);
                        else
                            PrintCmd("Need parameter");
                    }
                    else if(strcmp(cmd, "copy") == 0)
                    {
                        if(num_param > 0)
                            CopyCmd(iparam[0]);
                        else
                            PrintCmd("Need parameter");
                    }
                    else if(strcmp(cmd, "new") == 0)
                        NewCmd();
                    else if(strcmp(cmd, "g") == 0)
                    {
                        if(num_param > 0)
                            GoCmd(&cm730, iparam[0]);
                        else
                            PrintCmd("Need parameter");
                    }
                    else if(strcmp(cmd, "poweroff") == 0)
											{
											cm730.DXLPowerOn(false);
											}
                    else if(strcmp(cmd, "poweron") == 0)
											{
											cm730.DXLPowerOn(true);
											}
										else if(strcmp(cmd, "save") == 0)
												{
												if(num_param > 0)
                          SaveCmd(iparam[0]);
                        else
													SaveCmd(IndexPage());
												}
                    else if(strcmp(cmd, "name") == 0)
                        NameCmd();
                    else
                        {
												sprintf(s,"Bad command (%s:%d)! please input 'help'",cmd,strlen(cmd));
												PrintCmd(s);
												}
                }
            }
						idx = 0;
						memset(input,0,128);
            EndCommandMode();
        }
    }

    DrawEnding();

    return 0;
}
