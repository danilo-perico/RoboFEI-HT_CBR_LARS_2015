/*--------------------------------------------------------------------

******************************************************************************
* @file control.cpp
* @author Isaac Jesus da Silva - ROBOFEI-HT - FEI 😛
* @version V1.1.3
* @created 20/01/2015
* @Modified 21/10/2015
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
#include "mov.hpp"

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

int Initialize_servo();

void Gait_in_place(bool &stop_gait);

void move_action(int move_number, bool interrupt, bool &stop_gait); // realiza o movimento de ações

void move_gait(float X_amplitude, float Y_amplitude, float A_amplitude, bool &stop_gait); // realiza o gait

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
    float turn_angle = 20;
    float walk_foward= 15;
    int value;
	int erro;

    printf( "\n===== ROBOFEI-HT Control Process | based on Jimmy Control =====\n\n");

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

    //Carregando valores do config.ini -----------------------------------------
	if((turn_angle=ini->getd("Walking Config","turn_angle",-1024))==-1024){
		cout<<"Erro na leitura do conf.ini";
		turn_angle=20;
	}
	else if(turn_angle>30 || turn_angle<-30)
	    turn_angle=30;
	
	if((walk_foward=ini->getd("Walking Config","walk_foward",-1024))==-1024){
		cout<<"Erro na leitura do conf.ini";
		walk_foward=15;
	}
	else if(walk_foward>25)
	    walk_foward=25;
	
    TurnBall turnball(ini);

	//**************************************************************************
	//-------------para entrada de argumentos-----------------------------------
	namespace po=boost::program_options;

	po::options_description desc("options");
	desc.add_options()
    ("help", "produce help message")
    ("k", "Inicia com o controle do robô pelo teclado")
    ("v", "Verifica a tensao nos servos do corpo")
	;
  
	po::variables_map variables;
	po::store(po::parse_command_line(argc, argv, desc), variables);
	po::notify(variables); 
	//--------------------------------------------------------------------------

//////////////////// Framework Initialize ////////////////////////////
// ---- Open USBDynamixel -----------------------------------------------{
	if(Initialize_servo()==1) // chama a função que encontra o endereço de comunicação com o servo
		return 0;
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
  
    if (variables.count("v")) //verifica se foi chamado o argumento de controle pelo teclado
	{
	    if(cm730.ReadByte(12, 42, &value, 0) != CM730::SUCCESS)
            std::cout<<"Erro na leitura da tensao"<<std::endl;
	    std::cout<<"Tensao = "<<float(value)/10<<"V"<<std::endl;
	    return 0;
	}

//	printf("Pronto 3\n");
//    getchar();

	Action::GetInstance()->Initialize();
    Walking::GetInstance()->m_Joint.SetEnableBody(false);
    Action::GetInstance()->m_Joint.SetEnableBody(true);
    MotionManager::GetInstance()->SetEnable(true);

	std::cout<<"Start Action 1"<<std::endl;
    Action::GetInstance()->Start(1);    /* Init(stand up) pose */
    while(Action::GetInstance()->IsRunning()) usleep(8*1000); 
	Action* a=Action::GetInstance();

//	getchar();	

	std::cout<<"Start Action 9"<<std::endl;
    Action::GetInstance()->Start(9);   // Posicionando para andar
    while(Action::GetInstance()->IsRunning()) usleep(8*1000); 

	Action::GetInstance()->Stop();


	//***********************************************************************************************
	if (variables.count("k")) //verifica se foi chamado o argumento de controle pelo teclado
	{
	//-------------iniciando o modulo de andar pelo teclado------------------------------------------

		while(1)
		{
			int key = kbhit();
			usleep(4*1000);

		    switch(key)
			{
		        case 97: //a
				    cout << "Levantar quando as costas está para cima" << endl;
					move_action(11, 0, stop_gait);
		        break;

		        case 98: //b
				    cout << "Levantar quando o peito está para cima" << endl;
					move_action(10, 0, stop_gait);
		        break;
		        
		        case 112: //p
				    cout << "Chutar direito bola branca" << endl;
					move_action(1, 0, stop_gait);
					while(Action::GetInstance()->IsRunning()) usleep(8*1000);
					Action::GetInstance()->Start(20);
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
					while(Action::GetInstance()->IsRunning()) usleep(8*1000);
		        break;

		        case 108: //l
				    cout << "Chutar esquerdo bola branca" << endl;
					move_action(1, 0, stop_gait);
					Action::GetInstance()->Start(21);
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
					while(Action::GetInstance()->IsRunning()) usleep(8*1000);
		        break;

		        case 99: //c
				    cout << "Chutar direito bola laranja" << endl;
					move_action(12, 0, stop_gait);
		        break;

		        case 103: //g
				    cout << "Chutar esquerdo bola laranja" << endl;
					move_action(13, 0, stop_gait);
		        break;

		        case 102: //f
				    cout << "Andar para frente" << endl;
				    turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
					move_gait(walk_foward, 0.0, 0.0, stop_gait);
		        break;

		        case 100: //d
				    cout << "Vira para direita" << endl;
				    turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
					move_gait(0.0, 0.0, -turn_angle, stop_gait);
		        break;

		        case 105: //i
				    cout << "Passe Esquerda" << endl;
					move_action(70, 0, stop_gait);
		        break;

		        case 101: //e
				    cout << "Vira para esquerda" << endl;
				    turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
					move_gait(0.0, 0.0, turn_angle, stop_gait);
		        break;

		        case 106: //j
				    cout << "Passe Direita" << endl;
					move_action(71, 0, stop_gait);
		        break;

		        case 109: //m
				    cout << "Andar de lado esquerda" << endl;
				    turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
					move_gait(0.0, 10.0, 0.0, stop_gait);
		        break;

		        case 110: //n
				    cout << "Andar de lado direita" << endl;
				    turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
					move_gait(0.0, -10.0, 0.0, stop_gait);
		        break;

		        case 111: //o
				    cout << "Rotacionar a esquerda em volta da bola" << endl;
				    turnball.updateTurnValue(Walking::GetInstance()); //atualiza para os parametros do turn
					move_gait(turnball.andar_X, turnball.andar_lateral, turnball.turn_angle, stop_gait);
		        break;

		        case 107: //k
				    cout << "Andar curto para frente" << endl;
				    turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
					move_gait(10.0, 0.0, 0.0, stop_gait);
		        break;

		        case 114: //r
				    cout << "Andar curto para traz" << endl;
				    turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
					move_gait(-10.0, 0.0, 0.0, stop_gait);
		        break;

		        case 118: //v
				    cout << "Andar rapido para traz" << endl;
				    turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
					move_gait(-20.0, 0.0, 0.0, stop_gait);
		        break;

		        case 115: //s
					cout << "Stop com gait" << endl;
					turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
					Gait_in_place(stop_gait);
		        break;

		        case 116: //t
				    cout << "Stop" << endl;
					while(Walking::GetInstance()->GetCurrentPhase()!=0 && Walking::GetInstance()->IsRunning()!=0)  usleep(8*1000);
					Walking::GetInstance()->Stop();
					Walking::GetInstance()->m_Joint.SetEnableBody(false);
					Action::GetInstance()->m_Joint.SetEnableBody(true);
					MotionManager::GetInstance()->SetEnable(true);
					usleep(500000); //Aguarda meio segundo
					Action::GetInstance()->Start(1); // Realiza a ação do numero contido no move_number
					while(Action::GetInstance()->IsRunning()) usleep(8*1000);
					stop_gait = 1;
		        break;

		        case 104: //h
				    cout << "Greetings" << endl;
					move_action(24, 0, stop_gait);
		        break;

		        case 27: //ESC (stop)
				    cout << "Stop and shutdown code" << endl;
					return 0;
		        break;

				default:
					if(key!=0)
						cout<< "Tecla incorreta - verifique quais teclas controlam o robo"<<endl;
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
				if(IMU_ACCEL_X>0){  //Levanta se caido de frente
					std::cout<<" | Levantar de frente";
					move_action(10, 0, stop_gait);
				}
				else{  //Levanta se caido de costa
					std::cout<<" | Levantar de costa";
					move_action(11, 0, stop_gait);
				}
				stop_gait = 1;
				sleep(1);
			}


			if(DECISION_ACTION_A == 0)
			{
				std::cout<<" | Nada a fazer"<<std::endl;
				while(Walking::GetInstance()->GetCurrentPhase()!=0 && Walking::GetInstance()->IsRunning()!=0)  usleep(8*1000);
				Walking::GetInstance()->Stop();
				Walking::GetInstance()->m_Joint.SetEnableBody(false);
				Action::GetInstance()->m_Joint.SetEnableBody(true);
				MotionManager::GetInstance()->SetEnable(true);
				usleep(500000); //Aguarda meio segundo
				Action::GetInstance()->Start(1); // Realiza a ação do numero contido no move_number
				while(Action::GetInstance()->IsRunning()) usleep(8*1000);
				stop_gait = 1;
			}

			if(DECISION_ACTION_A == 1)
			{
				std::cout<<" | Andar para frente"<<std::endl;
				turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
				move_gait(walk_foward, 0.0, 0.0, stop_gait);
				usleep(500000);
			}
			if(DECISION_ACTION_A == 2)
			{
				std::cout<<" | Virar a esquerda"<<std::endl;
				turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
				move_gait(0.0, 0.0, turn_angle, stop_gait);
				usleep(500000);
			}
			if(DECISION_ACTION_A == 3)
			{
				std::cout<<" | Virar a direita"<<std::endl;
				turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
				move_gait(0.0, 0.0, -turn_angle, stop_gait);
				usleep(500000);
			}
			if(DECISION_ACTION_A == 4)
			{
				std::cout<<"| Chutar com pe direito"<<std::endl;
				move_action(12, 0, stop_gait);
				usleep(500000);
			}
			if(DECISION_ACTION_A == 5)
			{
				std::cout<<" | Chutar com pe esquerdo"<<std::endl;
				move_action(13, 0, stop_gait);
				usleep(500000);
			}
			if(DECISION_ACTION_A == 6)
			{
				std::cout<<" | Andar de Lado esquerda"<<std::endl;
				turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
				move_gait(0.0, 10.0, 0.0, stop_gait);
				usleep(500000);
			}
			if(DECISION_ACTION_A == 7)
			{
				std::cout<<" | Andar de Lado direita"<<std::endl;
				turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
				move_gait(0.0, -10.0, 0.0, stop_gait);
				usleep(500000);
			}
			if(DECISION_ACTION_A == 8)
			{
				std::cout<<" | Andar lento para frente"<<std::endl;
				turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
				if(float(DECISION_ACTION_B)<walk_foward)
				    move_gait(float(DECISION_ACTION_B), 0.0, 0.0, stop_gait);
				else
				    move_gait(walk_foward, 0.0, 0.0, stop_gait);
				usleep(500000);
			}
			if(DECISION_ACTION_A == 9)
			{
				std::cout<<" | Girar em torno da bola"<<std::endl;
				turnball.updateTurnValue(Walking::GetInstance()); //atualiza para os parametros do turn
				move_gait(turnball.andar_X, turnball.andar_lateral, turnball.turn_angle, stop_gait);
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
			if(DECISION_ACTION_A == 11)
			{
				std::cout<<" | Stop com gait"<<std::endl;
				turnball.updateConfig(Walking::GetInstance()); //volta para os parametros padrao do gait
				Gait_in_place(stop_gait);
			}
			if(DECISION_ACTION_A == 12)
			{			
					cout << "Passe Esquerda" << endl;
					move_action(70, 0, stop_gait);
            }
            if(DECISION_ACTION_A == 13)
			{			
					cout << "Passe Direito" << endl;
					move_action(71, 0, stop_gait);
            }
	}
	//--------------------------------------------------------------------------------------------------
	//==================================================================


    std::cout<<"Press some key to end!\n"<<std::endl;
    getchar();

//    LinuxActionScript::ScriptStart("script.asc");
//    while(LinuxActionScript::m_is_running == 1) sleep(10);

    return 0;
}

//========================================================================
//Execute the move action-------------------------------------------------
void move_action(int move_number, bool interrupt, bool &stop_gait )
{
	while(Walking::GetInstance()->GetCurrentPhase()!=0 && Walking::GetInstance()->IsRunning()!=0)  usleep(8*1000);
	Walking::GetInstance()->Stop();
	Walking::GetInstance()->m_Joint.SetEnableBody(false);
	Action::GetInstance()->m_Joint.SetEnableBody(true);
	MotionManager::GetInstance()->SetEnable(true);
	Action::GetInstance()->Start(move_number); // Realiza a ação do numero contido no move_number
	while(Action::GetInstance()->IsRunning() && ~interrupt) usleep(8*1000); // Aguarda finalizar a ação ou para por interrupção
	stop_gait = 1;
}

//========================================================================
//Execute the gait generation---------------------------------------------
void move_gait(float X_amplitude, float Y_amplitude, float A_amplitude, bool &stop_gait)
{
	if(Walking::GetInstance()->IsRunning()==0)
	{
		//Gait_in_place(stop_gait); // Necessita realizar o Gait antes de qualquer outra
		if(stop_gait == 1)
		{
			move_action(9, 0, stop_gait);
			stop_gait = 0;
		}
		cout << "Stop com gait" << endl;
		Action::GetInstance()->Stop();
		Walking::GetInstance()->m_Joint.SetEnableBody(true);
		Action::GetInstance()->m_Joint.SetEnableBody(false);
		MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
		Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
		Walking::GetInstance()->Y_MOVE_AMPLITUDE = 0.0;
		Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
		Walking::GetInstance()->Start();
		sleep(1);
	}
	Action::GetInstance()->Stop();
	Walking::GetInstance()->m_Joint.SetEnableBody(true);
	Action::GetInstance()->m_Joint.SetEnableBody(false);
	MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
	Walking::GetInstance()->X_MOVE_AMPLITUDE = X_amplitude;
	Walking::GetInstance()->Y_MOVE_AMPLITUDE = Y_amplitude;
	Walking::GetInstance()->A_MOVE_AMPLITUDE = A_amplitude;
	Walking::GetInstance()->Start();
}

//========================================================================
//Do the gait staing int the place----------------------------------------
void Gait_in_place(bool &stop_gait)
{
	if(stop_gait == 1)
	{
		cout << "Action 9" << endl;
		move_action(9, 0, stop_gait);
		stop_gait = 0;
	}
	move_gait(0.0, 0.0, 0.0, stop_gait);
}

//////////////////// Framework Initialize ////////////////////////////
// ---- Open USBDynamixel -----------------------------------------------{
int Initialize_servo()
{
	bool servoComunica = false;
	bool servoConectado = false;
	bool connectedRS = false;
	int * deviceIndex = new int;
	int idServo;
	char string_buffer[100]="";
	*deviceIndex = 0; 		//endereça o Servo
	while(*deviceIndex<3)// laço que percorre o servo 0, 1 e 2.
	{
		sprintf(string1,"/dev/robot/servo%d", *deviceIndex);
		LinuxCM730* linux_cm730;
    	linux_cm730 = new LinuxCM730(string1);
		CM730* cm730;
    	cm730 = new CM730(linux_cm730);

		if( MotionManager::GetInstance()->Initialize(cm730) == 0)
		{ // not connect with board rs485
			
		}
		else
		{
			cm730->ReadByte(1, 3, &idServo, 0); // Read the servo id of servo 1
			servoConectado = idServo == 1;
			usleep(1000);
			cm730->ReadByte(1, 3, &idServo, 0);//Try again because of fail
			servoConectado = idServo == 1;
    		if(servoConectado)
			{
       			cout<<"Connected and communicating with the body of the robot!\n";
			 	return 0;
			}
    		else
    		{// connected with board rs485 but it's not communicating
				sprintf(string_buffer,"%s/dev/robot/servo%d\n", string_buffer, *deviceIndex);
				connectedRS = true;
			}			
		}
		*deviceIndex = *deviceIndex + 1;
		delete cm730;
		delete linux_cm730;
	}
	delete deviceIndex; //desalocando da memória
	
	if(connectedRS == true)
	{
		printf("\e[0;31mConectou-se a placa USB/RS-485 mas não conseguiu se comunicar com o servo.\e[0m\n");
		cout<<"Endereços encontrado:"<<endl;
		cout<<string_buffer<<endl;
		cout<<"\e[0;36mVerifique se a chave que liga os servos motores está na posição ligada.\n\n\e[0m"<<endl;
	}
	else
	{
		cout<<"\e[1;31mNão há nenhuma placa USB/RS-485 conectada no computador.\n\n\e[0m"<<endl;
	}
	return 1;

}

