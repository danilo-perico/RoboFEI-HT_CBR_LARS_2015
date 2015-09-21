/*
 * main.cpp
 *
 *  Created on: 2011. 1. 4.
 *      Author: robotis  😛
 */

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
//#include <dynamixel.h>
#include <blackboard.h>

#ifdef MX28_1024
#define MOTION_FILE_PATH    "../../../../Data/motion_1024.bin"
#else
//#define MOTION_FILE_PATH    "../../../../Data/motion_4096.bin"
#define MOTION_FILE_PATH    "../../../../Data/webots_motion_4096.bin"
#endif

#define INI_FILE_PATH       "../../../../Data/config.ini"
//#define INI_FILE_PATH       "../../../../Data/webots_config.ini"
//#define INI_FILE_PATH       "../../../../Data/webots_config_1.ini"

#define DEBUG_PRINT true

using namespace Robot;
using namespace std;

#define KEY 123

int *mem ; //Variável que manipula memória compartilhada
float *memf ; //Variável que manipula memória compartilhada

//Depois de criado a memória compartilhada, para verificar se ela realmente foi criada
// e quantos processos estão utilizando, digite no terminal o comando $ipcs -m
// será a memoria criada ->   key = 0x0000007b    bytes = 2048
// nattch = number of attached processes

int using_shared_memory()
{
    // --- Variaveis usada para memoria compartilhada -----
    int shmid ; // identificador da memoria comum //
    const unsigned short int size = 2048; // tamanho da memória em Bytes
    int flag = 0;
    //-----------------------------------------------------

     // Recuperando ou criando uma memoria compartilhada-------------------
     //

     //shmget:para criar um segmento de memória compartilhada
     if (( shmid = shmget((key_t)KEY, size,0)) == -1)
     {
          perror("Erro no shmget") ;
          printf("\nMemória será criada\n");
         //return(1) ;
        if (( shmid = shmget((key_t)KEY, size, IPC_CREAT|IPC_EXCL|SHM_R|SHM_W)) == -1)
        {
            perror("Erro no shmget") ;
            return(1) ;
        }

     }
#ifdef DEBUG
     printf("Sou o processo com pid: %d \n",getpid()) ;
     printf("Identificador do segmento recuperado: %d \n",shmid) ;
     printf("Este segmento e associado a chave unica: %d\n",(key_t)KEY);
#endif
    //
    // acoplamento do processo a zona de memoria
    //recuperacao do pornteiro sobre a area de memoria comum
    //
    //shmat:retorna um pointeiro para o segmento de memória compartilhada
     if ((mem = (int*)shmat (shmid, 0, flag)) == (int*)-1){
          perror("acoplamento impossivel") ;
          return (2) ;
     }

     memf = (float*)(mem+125);
     //---------------------------------------------------------------------

            /* destruicao do segmento */
            //if ((shmctl(shmid, IPC_RMID, NULL)) == -1){
            // perror("Erro shmctl()");
             // return(1) ;
            //}
    return(0);
}




int kbhit(); //funcao do kbhit.cpp

void change_current_dir()
{
    char exepath[1024] = {0};
    if(readlink("/proc/self/exe", exepath, sizeof(exepath)) != -1)
        chdir(dirname(exepath));
}

int main(int argc, char *argv[])
{

    int * deviceIndex = new int; //alocado dinâmicamente
    *deviceIndex = 0; 		//endereça USB
    unsigned int tensaomedia = 0;

    using_shared_memory();

    bool stop_gait = 1;
    char string[50]; //String usada para definir prioridade do programa
    sprintf(string,"echo fei 123456| sudo -S renice -20 -p %d", getpid()); // prioridade maxima do codigo
    system(string);//prioridade

    printf( "\n===== ROBOFEI-HT Control Process | based on Jimmy Control=====\n\n");

    change_current_dir();
    getchar();
  	//Acopla ou cria a memoria compartilhada
//  	using_shared_memory();

    Action::GetInstance()->LoadFile(MOTION_FILE_PATH);

		minIni* ini;
		if(argc==2)
			ini = new minIni(argv[1]);
		else
			ini = new minIni(INI_FILE_PATH);

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
			        return 0;
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
				    return 0;
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
	

	std::cout<<"Start Action 9"<<std::endl;
    Action::GetInstance()->Start(9);   // Posicionando para andar
    while(Action::GetInstance()->IsRunning()) usleep(8*1000); 

	Action::GetInstance()->Stop();

//-------------iniciando o modulo de andar------------------------------------------

//while(1)
//{
//int key = kbhit();
//usleep(8*1000);
//	if(key != 115 && key != 0 && key != 107 && key != 107 && key != 100 && key != 101 && key != 102)
//		stop_gait = 1; // executa o playpage 9 uma vez antes de iniciar o gait

//        switch(key)
//		{
//            case 97: //a
//            cout << "Levantar quando as costas estão para cima" << endl;
//		Walking::GetInstance()->Stop();
//    		Walking::GetInstance()->m_Joint.SetEnableBody(false);
//    		Action::GetInstance()->m_Joint.SetEnableBody(true);
//    		MotionManager::GetInstance()->SetEnable(true);
//    		Action::GetInstance()->Start(11);    /* Init(stand up) pose */
//            break;

//            case 98: //b
//            cout << "Levantar quando o peito está para cima" << endl;
//			Walking::GetInstance()->Stop();
//    		Walking::GetInstance()->m_Joint.SetEnableBody(false);
//    		Action::GetInstance()->m_Joint.SetEnableBody(true);
//    		MotionManager::GetInstance()->SetEnable(true);
//    		Action::GetInstance()->Start(10);    /* Init(stand up) pose */
//            break;

//            case 99: //c
//            cout << "Chutar direito" << endl;
//		Walking::GetInstance()->Stop();
//    		Walking::GetInstance()->m_Joint.SetEnableBody(false);
//    		Action::GetInstance()->m_Joint.SetEnableBody(true);
//    		MotionManager::GetInstance()->SetEnable(true);
//		Action::GetInstance()->Start(1);    /* Init(stand up) pose */
//		while(Action::GetInstance()->IsRunning()) usleep(8*1000);
//    		Action::GetInstance()->Start(12);   
//            break;

//            case 103: //g
//            cout << "Chutar esquerdo" << endl;
//			Walking::GetInstance()->Stop();
//    		Walking::GetInstance()->m_Joint.SetEnableBody(false);
//   			Action::GetInstance()->m_Joint.SetEnableBody(true);
//    		MotionManager::GetInstance()->SetEnable(true);
//    		Action::GetInstance()->Start(13);    /* Init(stand up) pose */
//            break;

//            case 102: //f
//            cout << "Andar para frente" << endl;
//			Action::GetInstance()->Stop();
//	    	Walking::GetInstance()->m_Joint.SetEnableBody(true);
//   			Action::GetInstance()->m_Joint.SetEnableBody(false);
//			MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
//			Walking::GetInstance()->X_MOVE_AMPLITUDE = 20.0;
//			Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
//			Walking::GetInstance()->Start();
//            break;

//            case 100: //d
//            cout << "Vira para direita" << endl;
//			Action::GetInstance()->Stop();
//			Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
//			Walking::GetInstance()->A_MOVE_AMPLITUDE = -20.0;
//			Walking::GetInstance()->Start();
//	    	Walking::GetInstance()->m_Joint.SetEnableBody(true);
//   			Action::GetInstance()->m_Joint.SetEnableBody(false);
//			MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
//            break;

//            case 105: //i
//            cout << "Passe Direita" << endl;
//			Walking::GetInstance()->Stop();
//    		Walking::GetInstance()->m_Joint.SetEnableBody(false);
//    		Action::GetInstance()->m_Joint.SetEnableBody(true);
//    		MotionManager::GetInstance()->SetEnable(true);
//    		Action::GetInstance()->Start(70);    /* Init(stand up) pose */
//            break;

//            case 101: //e
//            cout << "Vira para esquerda" << endl;
//			Action::GetInstance()->Stop();
//	    	Walking::GetInstance()->m_Joint.SetEnableBody(true);
//   			Action::GetInstance()->m_Joint.SetEnableBody(false);
//			MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
//			Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
//			Walking::GetInstance()->A_MOVE_AMPLITUDE = 20.0;
//			Walking::GetInstance()->Start();
//            break;

//            case 106: //j
//            cout << "Passe Esquerda" << endl;
//		Walking::GetInstance()->Stop();
//    		Walking::GetInstance()->m_Joint.SetEnableBody(false);
//    		Action::GetInstance()->m_Joint.SetEnableBody(true);
//    		MotionManager::GetInstance()->SetEnable(true);
//    		Action::GetInstance()->Start(71);    /* Init(stand up) pose */
//            break;

//            case 107: //k
//            cout << "Andar curto para frente" << endl;
//			Action::GetInstance()->Stop();
//	    	Walking::GetInstance()->m_Joint.SetEnableBody(true);
//   			Action::GetInstance()->m_Joint.SetEnableBody(false);
//			MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
//			Walking::GetInstance()->X_MOVE_AMPLITUDE = 10.0;
//			Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
//			Walking::GetInstance()->Start();
//            break;

//            case 114: //r
//            cout << "Andar curto para traz" << endl;
//			Action::GetInstance()->Stop();
//	    	Walking::GetInstance()->m_Joint.SetEnableBody(true);
//   			Action::GetInstance()->m_Joint.SetEnableBody(false);
//			MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
//			Walking::GetInstance()->X_MOVE_AMPLITUDE = -20.0;
//			Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
//			Walking::GetInstance()->Start();
//            break;

//            case 118: //v
//            cout << "Andar rapido para traz" << endl;
//			Action::GetInstance()->Stop();
//	    	Walking::GetInstance()->m_Joint.SetEnableBody(true);
//   			Action::GetInstance()->m_Joint.SetEnableBody(false);
//			MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
//			Walking::GetInstance()->X_MOVE_AMPLITUDE = -35.0;
//			Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
//			Walking::GetInstance()->Start();
//            break;

//             case 115: //s
//		if(stop_gait == 1)
//		{
//            		cout << "Stop com gait" << endl;
//			Walking::GetInstance()->Stop();
//    			Walking::GetInstance()->m_Joint.SetEnableBody(false);
//    			Action::GetInstance()->m_Joint.SetEnableBody(true);
//    			MotionManager::GetInstance()->SetEnable(true);
//    			Action::GetInstance()->Start(9);
//			while(Action::GetInstance()->IsRunning()) usleep(8*1000);
//			stop_gait = 0;
//		}
//			Action::GetInstance()->Stop();
//	    		Walking::GetInstance()->m_Joint.SetEnableBody(true);
//   			Action::GetInstance()->m_Joint.SetEnableBody(false);
//			MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
//			Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
//			Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
//			Walking::GetInstance()->Start();
//            break;

//            case 116: //t
//            cout << "Stop" << endl;
//		Walking::GetInstance()->Stop();
//    		Walking::GetInstance()->m_Joint.SetEnableBody(false);
//		usleep(8*100000);
//    		Action::GetInstance()->m_Joint.SetEnableBody(true);
//    		MotionManager::GetInstance()->SetEnable(true);
//    		Action::GetInstance()->Start(1);    /* Init(stand up) pose */
//		while(Action::GetInstance()->IsRunning()) usleep(8*1000);
//		Action::GetInstance()->Stop();
//		Walking::GetInstance()->Stop();
//	    	Walking::GetInstance()->m_Joint.SetEnableBody(false);
//   		Action::GetInstance()->m_Joint.SetEnableBody(false);
//		MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
//            break;

//            case 104: //h
//            cout << "Greetings" << endl;
//		Walking::GetInstance()->Stop();
//    		Walking::GetInstance()->m_Joint.SetEnableBody(false);
//    		Action::GetInstance()->m_Joint.SetEnableBody(true);
//    		MotionManager::GetInstance()->SetEnable(true);
//    		Action::GetInstance()->Start(24);    /* Init(stand up) pose */
//            break;

//            case 27: //ESC (stop)
//            cout << "Stop and shutdown code" << endl;
//			return 0;
//            break;

//        }
//}

/*    Walking::GetInstance()->m_Joint.SetEnableBody(true);
    Action::GetInstance()->m_Joint.SetEnableBody(false);

			MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
			Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
			Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
			Walking::GetInstance()->Start();
			printf("Start Walking\n");

		while(Walking::GetInstance()->IsRunning())
		while(1)
		{
				usleep(8*1000); 
				printf("haha\n"); 
				sleep(3);

				Walking::GetInstance()->X_MOVE_AMPLITUDE = 35.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
				sleep(20);
			Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
			Walking::GetInstance()->A_MOVE_AMPLITUDE = 20.0;

		}
*/
		//-andando------------------------------------------------
		//while(Walking::GetInstance()->IsRunning())
		while(1)
		{
//				usleep(8*1000); 
//				printf("haha\n"); 
//				sleep(5);
//				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
//				Walking::GetInstance()->A_MOVE_AMPLITUDE = 25.0;
//				sleep(10);
//			Walking::GetInstance()->X_MOVE_AMPLITUDE = 30.8;
//			Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
//				sleep(10);

			std::cout<< "Action " << DECISION_ACTION_A;
			if(DECISION_ACTION_A == 0)
			{
				std::cout<<" | Nada a fazer"<<std::endl;
//				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
//				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->Stop();
			}
			else
			{
				Walking::GetInstance()->Start();
			}
			if(DECISION_ACTION_A == 1)
			{
				std::cout<<" | Andar"<<std::endl;
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 30.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
				usleep(500000);
			}
			if(DECISION_ACTION_A == 2)
			{
				std::cout<<" | Virar a esquerda"<<std::endl;
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 25.0;
				usleep(500000);
			}
			if(DECISION_ACTION_A == 3)
			{
				std::cout<<" | Virar a direita"<<std::endl;
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = -25.0;
				usleep(500000);
			}
			if(DECISION_ACTION_A == 4)
			{
				std::cout<<"| Chutar com pe direito"<<std::endl;
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
				//chute_direito();
				usleep(500000);
			}
			if(DECISION_ACTION_A == 5)
			{
				std::cout<<" | Chutar com pe esquerdo"<<std::endl;
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
				//chute_esquerdo();
				usleep(500000);
			}
			if(DECISION_ACTION_A == 6)
			{
				std::cout<<" | Andar de Lado esquerda"<<std::endl;
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 10.0;
				//andar_lateral_esquerda();
				usleep(500000);
			}
			if(DECISION_ACTION_A == 7)
			{
				std::cout<<" | Andar de Lado direita"<<std::endl;
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = -10.0;
				//andar_lateral_direita();
				usleep(500000);
			}
			if(DECISION_ACTION_A == 8)
			{
				std::cout<<" | Andar lento"<<std::endl;
				Walking::GetInstance()->X_MOVE_AMPLITUDE = 10.0;
				Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
				usleep(500000);
			}

		}
		//-------------------------------------------------------------



////	Robot::Action *action = new Robot::Action;
////	action->LoadFile(MOTION_FILE_PATH);
////	action->Initialize();
////	action->m_Joint.SetEnableBody(true, true);
////	action->Start(1);    /* Init(stand up) pose */
////	printf("haha\n"); sleep(2);
////    while(action->IsRunning()) 
////	{
////		//return 0;
////		action->Process();
////		usleep(8*1000); 
////		//printf("haha\n"); sleep(2);
////	}
//	Action::GetInstance()->Initialize();
//    Action::GetInstance()->m_Joint.SetEnableBody(true, true);
//    MotionManager::GetInstance()->SetEnable(false);
//    Action::GetInstance()->Start(1);    /* Init(stand up) pose */
//	std::cout<<"Start Action 1"<<std::endl;

//    while(Action::GetInstance()->IsRunning()) 
//	{
//		//return 0;
//		usleep(8*1000); 
//		//printf("haha\n"); sleep(4);
//	}

//	std::cout<<"Start Action 9"<<std::endl;
//    Action::GetInstance()->Start(9); 
//    while(Action::GetInstance()->IsRunning()) usleep(8*1000); 

//	Action::GetInstance()->Stop();
//	linuxMotionTimer.Stop();
//	MotionManager::GetInstance()->RemoveModule((MotionModule*)Action::GetInstance());

////-------------iniciando o modulo de andar------------------------------------------
//	sleep(2);
//    Walking::GetInstance()->LoadINISettings(ini); 
//    MotionManager::GetInstance()->AddModule((MotionModule*)Walking::GetInstance());
//		linuxMotionTimer.Initialize(MotionManager::GetInstance());
//		linuxMotionTimer.Start();

//			MotionStatus::m_CurrentJoints.SetEnableBodyWithoutHead(true);
//			Walking::GetInstance()->X_MOVE_AMPLITUDE = 30.8;
//			Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
//			Walking::GetInstance()->Start();
//			printf("Start Walking\n");

//			//-andando-----------------------------------------
//			while(Walking::GetInstance()->IsRunning())
//			{
//				usleep(8*1000); 
//				printf("haha\n"); 
//				sleep(5);
//				Walking::GetInstance()->X_MOVE_AMPLITUDE = 0.0;
//				Walking::GetInstance()->A_MOVE_AMPLITUDE = 25.0;
//				sleep(10);
//			Walking::GetInstance()->X_MOVE_AMPLITUDE = 30.8;
//			Walking::GetInstance()->A_MOVE_AMPLITUDE = 0.0;
//				sleep(10);
//			}





//    Action::GetInstance()->Start(24); 

//    while(Action::GetInstance()->IsRunning()) 
//	{
//		usleep(8*1000); 
//	}


    std::cout<<"Press the ENTER key to begin!\n"<<std::endl;
    getchar();

//    LinuxActionScript::ScriptStart("script.asc");
//    while(LinuxActionScript::m_is_running == 1) sleep(10);

    return 0;
}