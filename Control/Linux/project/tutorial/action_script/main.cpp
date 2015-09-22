/*--------------------------------------------------------------------

******************************************************************************
* @file control.cpp
* @author Isaac Jesus da Silva - ROBOFEI-HT - FEI 😛
* @version V1.0.5
* @created 20/01/2015
* @Modified 03/07/2015
* @e-mail isaac25silva@yahoo.com.br
* @brief control 😛
****************************************************************************
**************************************************************************** 
Arquivo fonte contendo o programa que controla os servos do corpo do robô
/--------------------------------------------------------------------------*/

#include <unistd.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdlib>

#include <stdio.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>


//#include "Camera.h"
//#include "Point.h"
//#include "mjpg_streamer.h"
#include "minIni.h"
//#include "LinuxCamera.h"
//#include "ColorFinder.h"
#include <string>

#include "Action.h"
#include "Head.h"
#include "Walking.h"
#include "MX28.h"
#include "MotionManager.h"
#include "LinuxMotionTimer.h"
#include "LinuxCM730.h"
#include "LinuxActionScript.h"
#include <blackboard.h>
#include <boost/program_options.hpp> //tratamento de argumentos linha de comando

#ifdef MX28_1024
#define MOTION_FILE_PATH    "../../Control/Data/motion_1024.bin"
#else
//#define MOTION_FILE_PATH    "../../Control/Data/motion_4096.bin"
#define MOTION_FILE_PATH    "../../../Control/Data/webots_motion_4096.bin"
#endif

#define INI_FILE_PATH       "../../Control/Data/config.ini"
//#define INI_FILE_PATH       "../../Control/Data/webots_config.ini"
//#define INI_FILE_PATH       "../../Control/Data/webots_config_1.ini"

#define DEBUG_PRINT true

using namespace Robot;
using namespace std;

int kbhit(); //funcao do kbhit.cpp

char Initialize_servo();

void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
        chdir(dirname(exepath));
}

char string1[50]; //String usada durante o programa

int main(int argc, char **argv)
{

    unsigned int tensaomedia = 0;

    using_shared_memory();

    bool stop_gait = 1;
	char *Servoport;
    sprintf(string1,"echo fei 123456| sudo -S renice -20 -p %d", getpid()); // prioridade maxima do codigo
    system(string1);//prioridade

    printf( "\n===== ROBOFEI-HT Control Process | based on Jimmy Control=====\n\n");

    change_current_dir();
    //getchar();
  	//Acopla ou cria a memoria compartilhada
//  	using_shared_memory();

    Action::GetInstance()->LoadFile(MOTION_FILE_PATH);

		minIni* ini;
		//if(argc==2)
		//	ini = new minIni(argv[1]);
		//else
			ini = new minIni(INI_FILE_PATH);

	//**************************************************************************
	//-------------para entrada de argumentos-----------------------------------
	namespace po=boost::program_options;

	po::options_description desc("options");
	desc.add_options()
    ("help", "produce help message")
    ("keyboard", "Inicia com o controle do robô pelo teclado")
	;
  
	po::variables_map variables;
	po::store(po::parse_command_line(argc, argv, desc), variables);
	po::notify(variables); 
	//--------------------------------------------------------------------------

//////////////////// Framework Initialize ////////////////////////////
// ---- Open USBDynamixel -----------------------------------------------{
	bool servoComunica = false;
	bool servoConectado = false;
	int * deviceIndex = new int;
	int idServo;
	*deviceIndex = 0; 		//endereça o Servo
	while(*deviceIndex<3)// laço que percorre o servo 0, 1 e 2.
	{
		sprintf(string1,"/dev/robot/servo%d", *deviceIndex);
		LinuxCM730* linux_cm730;
    	linux_cm730 = new LinuxCM730(string1);
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
			        return 0;
			}
			*deviceIndex = *deviceIndex + 1;      // Não conecta na placa do servo e tenta a proxima porta.
		}
		else
		{
			servoComunica = true;
			printf( "Succeed to open Servo%d!\n", *deviceIndex );
			cm730->ReadByte(1, 3, &idServo, 0);
			servoConectado = idServo == 1;
			usleep(1000);
			cm730->ReadByte(1, 3, &idServo, 0);//Tenta novamente caso falhe a comunicação
			servoConectado = idServo == 1;
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
				    return 0;
				}
				*deviceIndex = *deviceIndex + 1;
			}
		}
	}
	delete deviceIndex; //desalocando da memória
	//-----------------------------------------------------------------------------}
    //////////////////// Framework Initialize ////////////////////////////
    LinuxCM730 linux_cm730(string1);
    CM730 cm730(&linux_cm730);
    if(MotionManager::GetInstance()->Initialize(&cm730) == false)
    {
        printf("Fail to initialize Motion Manager!\n");
            return 0;
    }
//================================================================================== 


	MotionManager::GetInstance()->LoadINISettings(ini);

//	printf("Pronto 2\n");
//    getchar();

    Walking::GetInstance()->LoadINISettings(ini); 
    MotionManager::GetInstance()->AddModule((MotionModule*)Action::GetInstance());
    MotionManager::GetInstance()->AddModule((MotionModule*)Walking::GetInstance());
    LinuxMotionTimer linuxMotionTimer;
		linuxMotionTimer.Initialize(MotionManager::GetInstance());
		linuxMotionTimer.Start();
    /////////////////////////////////////////////////////////////////////
  

//	printf("Pronto 3\n");
//    getchar();

	Action::GetInstance()->Initialize();
    Walking::GetInstance()->m_Joint.SetEnableBody(false);
    Action::GetInstance()->m_Joint.SetEnableBody(true);
    MotionManager::GetInstance()->SetEnable(true);

	std::cout<<"Start Action 1"<<std::endl;
    Action::GetInstance()->Start(1);    /* Init(stand up) pose */
    while(Action::GetInstance()->IsRunning()) usleep(8*1000); 

//	getchar();	

	std::cout<<"Start Action 9"<<std::endl;
    Action::GetInstance()->Start(9);   // Posicionando para andar
    while(Action::GetInstance()->IsRunning()) usleep(8*1000); 

	Action::GetInstance()->Stop();
int erro;

	//***********************************************************************************************
	if (variables.count("keyboard")) //verifica se foi chadao o argumento de controle pelo teclado
	{
	//-------------iniciando o modulo de andar pelo teclado------------------------------------------

		while(1)
		{
			int key = kbhit();
			usleep(8*1000);
			//Neste if está incluso todos os movimentos de gait - excluindo os actions----------------
			if(key != 115 && key != 0 && key != 107 && key != 100 && key != 101 && key != 102&& key != 109 && key != 110 && key != 111)
				stop_gait = 1; // executa o playpage 9 uma vez antes de iniciar o gait

		    switch(key)
			{
		        case 97: //a
				    cout << "Levantar quando as costas estão para cima" << endl;
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(11);  
		        break;

		        case 98: //b
				    cout << "Levantar quando o peito está para cima" << endl;
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(10);  
		        break;

		        case 99: //c
				    cout << "Chutar direito" << endl;
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(1);    /* Init(stand up) pose */
					while(Action::GetInstance()->IsRunning()) usleep(8*1000);
					Action::GetInstance()->Start(12);
					while(Action::GetInstance()->IsRunning()) usleep(8*1000);
					Action::GetInstance()->Stop();
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionManager::GetInstance()->SetEnable(false);
					cm730.WriteWord(13, 32, 1023, &erro);
					cm730.WriteWord(13, 30, MotionManager::GetInstance()->m_Offset[13]+520, &erro);
					cm730.WriteWord(15, 32, 1023, &erro);
					cm730.WriteWord(15, 30, MotionManager::GetInstance()->m_Offset[15]+400, &erro);
					usleep(1000000);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(78);
		        break;

		        case 103: //g
				    cout << "Chutar esquerdo" << endl;
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
		   			Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(1);    /* Init(stand up) pose */
					while(Action::GetInstance()->IsRunning()) usleep(8*1000);
					Action::GetInstance()->Start(13);
					while(Action::GetInstance()->IsRunning()) usleep(8*1000);
					Action::GetInstance()->Stop();
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionManager::GetInstance()->SetEnable(false);
					cm730.WriteWord(14, 32, 1023, &erro);
					cm730.WriteWord(14, 30, MotionManager::GetInstance()->m_Offset[14]+520, &erro);
					cm730.WriteWord(16, 32, 1023, &erro);
					cm730.WriteWord(16, 30, MotionManager::GetInstance()->m_Offset[16]+640, &erro);
					usleep(1000000);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(79);
   
		        break;

		        case 102: //f
				    cout << "Andar para frente" << endl;
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 20.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Start();
		        break;

		        case 100: //d
				    cout << "Vira para direita" << endl;
					Action::GetInstance()->Stop();
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = -20.0;
					Walking::GetInstance()->Start();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
		        break;

		        case 105: //i
				    cout << "Passe Direita" << endl;
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(70);  
		        break;

		        case 101: //e
				    cout << "Vira para esquerda" << endl;
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 20.0;
					Walking::GetInstance()->Start();
		        break;

		        case 106: //j
				    cout << "Passe Esquerda" << endl;
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(71);   
		        break;

		        case 109: //m
				    cout << "Andar de lado esquerda" << endl;
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 10.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Start();
		        break;

		        case 110: //n
				    cout << "Andar de lado direita" << endl;
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = -10.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Start();
		        break;

		        case 111: //o
				    cout << "Rotacionar a esquerda em volta da bola" << endl;
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 23.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = -10.0;
					Walking::GetInstance()->Start();
		        break;

		        case 107: //k
				    cout << "Andar curto para frente" << endl;
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 10.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Start();
		        break;

		        case 114: //r
				    cout << "Andar curto para traz" << endl;
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = -10.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Start();
		        break;

		        case 118: //v
				    cout << "Andar rapido para traz" << endl;
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = -20.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Start();
		        break;

		        case 115: //s
					if(stop_gait == 1)
					{
						cout << "Stop com gait" << endl;
						Walking::GetInstance()->Stop();
						Walking::GetInstance()->m_Joint.SetEnableBody(false);
						Action::GetInstance()->m_Joint.SetEnableBody(true);
						MotionManager::GetInstance()->SetEnable(true);
						Action::GetInstance()->Start(9);
						while(Action::GetInstance()->IsRunning()) usleep(8*1000);
						stop_gait = 0;
					}
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Start();
		        break;

		        case 116: //t
				    cout << "Stop" << endl;
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					usleep(8*100000);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(1);    /* Init(stand up) pose */
					while(Action::GetInstance()->IsRunning()) usleep(8*1000);
					Action::GetInstance()->Stop();
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
			   		Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
		        break;

		        case 104: //h
				    cout << "Greetings" << endl;
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(24);  
		        break;

		        case 27: //ESC (stop)
				    cout << "Stop and shutdown code" << endl;
					return 0;
		        break;

		    }
		}

	}
	//==========================================================================================


	//***************************************************************************************
	//-------------------------Controle pela decisão-----------------------------------------
	while(1)
	{

			std::cout<< "Action " << DECISION_ACTION_A; // Mostra o valor da ação

			if (IMU_STATE){ // Ve se esta caido
				if(IMU_STATE==1){  //Levanta se caido de frente
					std::cout<<" | Levantar de frente";
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(11);
				}
				else{  //Levanta se caido de costa
					std::cout<<" | Levantar de costa";
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					Action::GetInstance()->Start(10);
				}
				stop_gait = 1;
				if(stop_gait == 1)
					{
						//cout << "Stop com gait" << endl;
						Walking::GetInstance()->Stop();
						Walking::GetInstance()->m_Joint.SetEnableBody(false);
						Action::GetInstance()->m_Joint.SetEnableBody(true);
						MotionManager::GetInstance()->SetEnable(true);
						Action::GetInstance()->Start(9);
						while(Action::GetInstance()->IsRunning()) usleep(8*1000);
						stop_gait = 0;
					}
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Start();
				sleep(5);
			}




			//Neste if está incluso todos os movimentos de gait - excluindo os actions----------------
			if(DECISION_ACTION_A != 1 && DECISION_ACTION_A != 2 && DECISION_ACTION_A != 3 && DECISION_ACTION_A != 6 && DECISION_ACTION_A != 7 && DECISION_ACTION_A != 8 && DECISION_ACTION_A != 9 )
				stop_gait = 1; // executa o playpage 9 uma vez antes de iniciar o gait

			if(DECISION_ACTION_A == 0)
			{
				std::cout<<" | Nada a fazer"<<std::endl;
					if(stop_gait == 1)
					{
						//cout << "Stop com gait" << endl;
						Walking::GetInstance()->Stop();
						Walking::GetInstance()->m_Joint.SetEnableBody(false);
						Action::GetInstance()->m_Joint.SetEnableBody(true);
						MotionManager::GetInstance()->SetEnable(true);
						Action::GetInstance()->Start(9);
						while(Action::GetInstance()->IsRunning()) usleep(8*1000);
						stop_gait = 0;
					}
					Action::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   			Action::GetInstance()->m_Joint.SetEnableBody(false);
					MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
					Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
					Walking::GetInstance()->Start();
			}

			if(DECISION_ACTION_A == 1)
			{
				std::cout<<" | Andar para frente"<<std::endl;
				Action::GetInstance()->Stop();
				Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   		Action::GetInstance()->m_Joint.SetEnableBody(false);
				MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 20.0;
				Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->Start();
				usleep(500000);
			}
			if(DECISION_ACTION_A == 2)
			{
				std::cout<<" | Virar a esquerda"<<std::endl;
				Action::GetInstance()->Stop();
				Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   		Action::GetInstance()->m_Joint.SetEnableBody(false);
				MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 20.0;
				Walking::GetInstance()->Start();
				usleep(500000);
			}
			if(DECISION_ACTION_A == 3)
			{
				std::cout<<" | Virar a direita"<<std::endl;
				Action::GetInstance()->Stop();
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = -20.0;
				Walking::GetInstance()->Start();
				Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   		Action::GetInstance()->m_Joint.SetEnableBody(false);
				MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
				usleep(500000);
			}
			if(DECISION_ACTION_A == 4)
			{
				std::cout<<"| Chutar com pe direito"<<std::endl;
				Walking::GetInstance()->Stop();
				Walking::GetInstance()->m_Joint.SetEnableBody(false);
				Action::GetInstance()->m_Joint.SetEnableBody(true);
				MotionManager::GetInstance()->SetEnable(true);
				Action::GetInstance()->Start(1);    /* Init(stand up) pose */
				while(Action::GetInstance()->IsRunning()) usleep(8*1000);
				Action::GetInstance()->Start(12);
				while(Action::GetInstance()->IsRunning()) usleep(8*1000);
				Action::GetInstance()->Stop();
		   		Action::GetInstance()->m_Joint.SetEnableBody(false);
				MotionManager::GetInstance()->SetEnable(false);
				cm730.WriteWord(13, 32, 1023, &erro);
				cm730.WriteWord(13, 30, MotionManager::GetInstance()->m_Offset[13]+520, &erro);
				cm730.WriteWord(15, 32, 1023, &erro);
				cm730.WriteWord(15, 30, MotionManager::GetInstance()->m_Offset[15]+400, &erro);
				usleep(1000000);
				Action::GetInstance()->m_Joint.SetEnableBody(true);
				MotionManager::GetInstance()->SetEnable(true);
				Action::GetInstance()->Start(78);  
				usleep(500000);
			}
			if(DECISION_ACTION_A == 5)
			{
				std::cout<<" | Chutar com pe esquerdo"<<std::endl;
				Walking::GetInstance()->Stop();
				Walking::GetInstance()->m_Joint.SetEnableBody(false);
		   		Action::GetInstance()->m_Joint.SetEnableBody(true);
				MotionManager::GetInstance()->SetEnable(true);
				Action::GetInstance()->Start(1);    /* Init(stand up) pose */
				while(Action::GetInstance()->IsRunning()) usleep(8*1000);
				Action::GetInstance()->Start(13);
				while(Action::GetInstance()->IsRunning()) usleep(8*1000);
				Action::GetInstance()->Stop();
		   		Action::GetInstance()->m_Joint.SetEnableBody(false);
				MotionManager::GetInstance()->SetEnable(false);
				cm730.WriteWord(14, 32, 1023, &erro);
				cm730.WriteWord(14, 30, MotionManager::GetInstance()->m_Offset[14]+520, &erro);
				cm730.WriteWord(16, 32, 1023, &erro);
				cm730.WriteWord(16, 30, MotionManager::GetInstance()->m_Offset[16]+640, &erro);
				usleep(1000000);
				Action::GetInstance()->m_Joint.SetEnableBody(true);
				MotionManager::GetInstance()->SetEnable(true);
				Action::GetInstance()->Start(79);
				usleep(500000);
			}
			if(DECISION_ACTION_A == 6)
			{
				std::cout<<" | Andar de Lado esquerda"<<std::endl;
				Action::GetInstance()->Stop();
				Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   		Action::GetInstance()->m_Joint.SetEnableBody(false);
				MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->Y_MOVE_AMPLITUDE = 10.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->Start();
				usleep(500000);
			}
			if(DECISION_ACTION_A == 7)
			{
				std::cout<<" | Andar de Lado direita"<<std::endl;
				Action::GetInstance()->Stop();
				Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   		Action::GetInstance()->m_Joint.SetEnableBody(false);
				MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->Y_MOVE_AMPLITUDE = -10.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->Start();
				usleep(500000);
			}
			if(DECISION_ACTION_A == 8)
			{
				std::cout<<" | Andar lento para frente"<<std::endl;
				Action::GetInstance()->Stop();
				Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   		Action::GetInstance()->m_Joint.SetEnableBody(false);
				MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 10.0;
				Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->Start();
				usleep(500000);
			}
			if(DECISION_ACTION_A == 9)
			{
				std::cout<<" | Girar em torno da bola"<<std::endl;
				Action::GetInstance()->Stop();
				Walking::GetInstance()->m_Joint.SetEnableBody(true);
		   		Action::GetInstance()->m_Joint.SetEnableBody(false);
				MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->Y_MOVE_AMPLITUDE = 23.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = -10.0;
				Walking::GetInstance()->Start();
				usleep(500000);
			}
			if(DECISION_ACTION_A == 10)
			{							// colocar o action-script para cair e defender!!!
				std::cout<<" | Defender a bola"<<std::endl;  //---------------------------------------------------------TODO
				Walking::GetInstance()->Stop();
				Walking::GetInstance()->m_Joint.SetEnableBody(false);
		   		Action::GetInstance()->m_Joint.SetEnableBody(true);
				MotionManager::GetInstance()->SetEnable(true);
				Action::GetInstance()->Start(1);    /* Init(stand up) pose */
				while(Action::GetInstance()->IsRunning()) usleep(8*1000);
				Action::GetInstance()->Start(20);    // colocar o action-script para cair e defender!!!
				usleep(500000);
			}

	}
	//--------------------------------------------------------------------------------------------------
	//==================================================================


    std::cout<<"Press the ENTER key to begin!\n"<<std::endl;
    getchar();

//    LinuxActionScript::ScriptStart("script.asc");
//    while(LinuxActionScript::m_is_running == 1) sleep(10);

    return 0;
}

//////////////////// Framework Initialize ////////////////////////////
// ---- Open USBDynamixel -----------------------------------------------{
char Initialize_servo()
{
	bool servoComunica = false;
	bool servoConectado = false;
	int * deviceIndex = new int;
	int idServo;
	*deviceIndex = 0; 		//endereça o Servo
	while(*deviceIndex<3)// laço que percorre o servo 0, 1 e 2.
	{
		sprintf(string1,"/dev/robot/servo%d", *deviceIndex);
		LinuxCM730* linux_cm730;
    	linux_cm730 = new LinuxCM730(string1);
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
			        return 0;
			}
			*deviceIndex = *deviceIndex + 1;      // Não conecta na placa do servo e tenta a proxima porta.
		}
		else
		{
			servoComunica = true;
			printf( "Succeed to open Servo%d!\n", *deviceIndex );
			cm730->ReadByte(1, 3, &idServo, 0);
			servoConectado = idServo == 1;
			usleep(1000);
			cm730->ReadByte(1, 3, &idServo, 0);//Tenta novamente caso falhe a comunicação
			servoConectado = idServo == 1;
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
				    return 0;
				}
				*deviceIndex = *deviceIndex + 1;
			}
		}
	}
	delete deviceIndex; //desalocando da memória
}

