/*--------------------------------------------------------------------
******************************************************************************
  * @file       communication.cpp
  * @author     Thiago Pedro Donadon Homem - ROBOFEI-HT - FEI
  * @version    V0.0.1
  * @created    19/06/2015
  * @Modified   19/06/2015
  * @e-mail     thiagohomem@gmail.com
  * @brief      Communication
  ****************************************************************************
/--------------------------------------------------------------------*/

#include <iostream>
#include "communication.h"
#include "blackboard.h"
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <fstream>

#include "threading.h"
#include "LinuxNetwork.h"
#include "libUDPclient.h"
#include "data_RoboCupGameControll.h"
//#include "RoboCupGameControlData.h"  //nao consegui usar a biblioteca da RoboCup


// =================== GENERAL SETTINGS =========================
// +> For Config
#define mSLEEP_COM              10
#define THREAD_PRIORITY_COM     30
// =============================================

// =================== GAMECONTROLLER CONFIG ====================
// +> For Config - Set IP address and of the Gamecontroller server
//#define SERVER      "192.168.1.121"
//#define PORT        3838
#define BUFLEN      512  //Max length of buffer

// +> For Use
void establishCommunication();
struct RoboCupGameControlReturnData dado;
// ==============================================================


// ====================== GAMECONTROLLER =====================
// +> For Config
// set CYAN or MAGENTA and the TEAM number and PLAYER number before the game
#define referreBox_broadcast_ip_port    "255.255.255.255:3838"
//#define TEAM_ROBOFEI   			TEAM_CYAN
//#define TEAM_OPPONENT      		TEAM_MAGENTA
//#define NO_TEAM_ROBOFEI                 3	//configurar numero da ROBOFEI que aparece no GameController
//#define NO_PLAYER_ROBOFEI		3      //configurar numero do robô. Lembre-se que cada robô deve ter seu número

#define AlfaStruct2String(x) ((unsigned char*)&x)

// +> For Use
unsigned short state, warnaGawang;
#define NOT_FOUND       404
// ===============================================

using namespace Robot;
using namespace std;

uint8 team_robofei;
struct RoboCupGameControlData referre;



#define referre_myteam	        referre.teams[team_robofei]
#define referre_TeamInfo(self)  referre.teams[team_robofei].players[self]
#define referre_myself          referre_TeamInfo(NO_PLAYER_ROBOFEI-1)


char *SERVER;
unsigned int PORT;
int TEAM_ROBOFEI;
int TEAM_OPPONENT;
int NO_TEAM_ROBOFEI;
int NO_PLAYER_ROBOFEI;


// pengaman
static bool globalCom_isActivated = false;
static bool kcomm_referee_active = false;


//utilize esta função para leitura de todos os arquivos de configuração!!
void leia()
{
	ifstream Arquivo;
	//estrutura do arquivo de leitura:
	string aux;
	string arqIP;
	int arqPORT;
	int arqTEAMColor;
	int arqOPPColor;
	int arqROBOFEINo;
	int arqROBOFEIPlayerNo;
	int pos_servo1;
	int pos_servo2;
	Arquivo.open("/home/fei/RoboFEI-HT/config.txt");
	if (Arquivo.is_open()){
	    Arquivo >> aux >> arqIP;
	    Arquivo >> aux >> arqPORT;
	    Arquivo >> aux >> arqTEAMColor;
	    Arquivo >> aux >> arqOPPColor;
	    Arquivo >> aux >> arqROBOFEINo;
	    Arquivo >> aux >> arqROBOFEIPlayerNo;

	    SERVER = strcpy((char*)malloc(arqIP.length()+1), arqIP.c_str());
	    PORT = arqPORT;
	    TEAM_ROBOFEI = arqTEAMColor;
	    TEAM_OPPONENT = arqOPPColor;
	    NO_TEAM_ROBOFEI = arqROBOFEINo;
	    NO_PLAYER_ROBOFEI = arqROBOFEIPlayerNo;
	    ROBOT_NUMBER = NO_PLAYER_ROBOFEI; //armazeno na memoria compartilhada o numero do robô!
	//cout<<"IP "<<SERVER<<" PORT "<<PORT<<endl;
	//cout<<"TEAM: "<<TEAM_ROBOFEI<<" Opponent: "<<TEAM_OPPONENT<<" Num FEI: "<<NO_TEAM_ROBOFEI<<" Num Robot: "<<NO_PLAYER_ROBOFEI<<endl;
	}
}


void error(const char *msg)
{
    perror(msg);
    exit(0);
}





void *recebendo(void* arg)
{
	UDPClient client_gcd;
    if(!kcomm_referee_active)
    {
        client_gcd.Initilaize(referreBox_broadcast_ip_port, "udp");
        kcomm_referee_active = true;
    }
    referre_myself.penalty = PENALTY_NONE;
    state = NOT_FOUND;

//~ exit(1);
	while(1)
	{
		//cout << "[=============kl========waiting==]";
		client_gcd.receive(Struct2String(referre), sizeof(referre));
		//cout << "================================\n" << referre.version;
		//exit(1);
		if(referre.version == GAMECONTROLLER_STRUCT_VERSION)
		{
			// cekking status pertandingan
			//cout<<"Referee teste: "<<referre.teams[TEAM_ROBOFEI].players[NO_PLAYER_ROBOFEI-1].penalty<<endl;
			if(referre_myself.penalty == PENALTY_HL_KID_REQUEST_FOR_PICKUP){
				state = 9;
				COM_REFEREE = 1; //stop
		             }
			else if(referre_myself.penalty == PENALTY_HL_KID_REQUEST_FOR_SERVICE){
				state = 10;
				COM_REFEREE = 1; //stop
		             }
			else if(referre_myself.penalty != PENALTY_NONE){
				state = 6;
				COM_REFEREE = 1; //stop
		             }
			else if(referre.state == STATE_INITIAL){
				state = 0;
				COM_REFEREE = 1; //stop
		             }
			else if(referre.state == STATE_READY){
				state = 1;
				COM_REFEREE = 1; //ready
			     }
			else if(referre.state == STATE_SET){
				state = 2;
				COM_REFEREE = 1; //stop
		             }
			else if(referre.kickOffTeam == TEAM_ROBOFEI && referre.state == STATE_PLAYING && (referre.secondaryState == STATE2_NORMAL || referre.secondaryState == STATE2_OVERTIME)){
				state = 3;
				COM_REFEREE = 2; //play  //kickOff RoboFEI
			     }
			else if(referre.kickOffTeam != TEAM_ROBOFEI && referre.state == STATE_PLAYING && (referre.secondaryState == STATE2_NORMAL|| referre.secondaryState == STATE2_OVERTIME)){
				state = 4;
				COM_REFEREE = 2; //play  //kickOff do adversario
			     }
			else if(referre.kickOffTeam != TEAM_OPPONENT && referre.secondaryState == STATE2_PENALTYSHOOT){
				state = 7;
				COM_REFEREE = 3; //Penalty para RoboFEI
		             }
			else if(referre.kickOffTeam == TEAM_OPPONENT && referre.secondaryState == STATE2_PENALTYSHOOT){
				state = 8;
				COM_REFEREE = 4; //Penalty para Adversário.
		             }
			else if(referre.state == STATE_FINISHED){
				state = 5;
				COM_REFEREE = 1; //finish
			     }
			//~ else
				//~ state = NOT_FOUND;
			cout<<"Referee: "<<COM_REFEREE<<endl;
			cout<<"State: "<<state<<" Penalty: "<<referre.teams[NO_TEAM_ROBOFEI-1].players[NO_PLAYER_ROBOFEI-1].penalty<<" Score: "<<unsigned(referre.teams[1].score)<<endl;
		} else {
			//state = NOT_FOUND;
		}
		usleep(100000);
	}
}


void *enviando(void *arg)
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];
    char message[BUFLEN];

    if ( (s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        error("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SERVER , &si_other.sin_addr) == 0)
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    while(1)
    {
        //send the message
        if (sendto(s, AlfaStruct2String(dado), 500 , 0 , (struct sockaddr *) &si_other, slen)==-1)
        {
            error("sendto()");
        }

        memset(buf,'\0', BUFLEN);
        usleep(mSLEEP_COM*1000);
        puts(buf);
    }

    close(s);

}


// =================== COMMUNICATION FUNCTION =======================
// +> Create the threads to send and receive packets
void establishCommunication()
{
    static pthread_t client_t, referreBox_t, clientbroad_t, serverbroad_t;

    //dado de resposta --> alterar apenas team e player no CONFIG no inicio do arquivo!
    strcpy(dado.header,GAMECONTROLLER_RETURN_STRUCT_HEADER);
    dado.version = GAMECONTROLLER_RETURN_STRUCT_VERSION;
    dado.team = NO_TEAM_ROBOFEI;
    dado.player = NO_PLAYER_ROBOFEI;
    dado.message = GAMECONTROLLER_RETURN_MSG_ALIVE;
    ROBOT_NUMBER = NO_PLAYER_ROBOFEI; //publico na memoria compartilhada quem eh o robô. Utilizo na comunicação entre robôs.
    if(!globalCom_isActivated)
    {
        threadInitialize(referreBox_t, recebendo, THREAD_PRIORITY_COM);
        threadInitialize(client_t, enviando, THREAD_PRIORITY_COM);
    }
}


int main(void)
{
	using_shared_memory();

	#ifdef DEBUG
	cout << "Communication" << endl;
	#endif
	
	leia();
	//cin.get();
	establishCommunication();
	while(1){
		//cout<<COM_REFEREE<<endl;
	}
    return 0;
}





/*
//SENT VARIABLES TO DECISION
COM_ACTION_ROBOT1 = 1;
COM_ACTION_ROBOT2 = 2;
COM_ACTION_ROBOT3 = 3;
COM_STATE_ROBOT1 = 10;//tempo
COM_STATE_ROBOT2 = 11;
COM_STATE_ROBOT3 = 12;
COM_POS_ROBOT1 = 20 ;
COM_POS_ROBOT2 = 21;
COM_POS_ROBOT3 = 22;
COM_POS_BALL_ROBOT1 = 30;
COM_POS_BALL_ROBOT2 = 31;
COM_POS_BALL_ROBOT3 = 32;
COM_POS_OPP_A_ROBOT1 = 40;
COM_POS_OPP_A_ROBOT2 = 41;
COM_POS_OPP_A_ROBOT3 = 42;
COM_POS_OPP_A_ROBOT4 = 43;
COM_POS_OPP_B_ROBOT1 = 50;
COM_POS_OPP_B_ROBOT2 = 51;
COM_POS_OPP_B_ROBOT3 = 52;
COM_POS_OPP_B_ROBOT4 = 53;
COM_POS_OPP_C_ROBOT1 = 60;
COM_POS_OPP_C_ROBOT2 = 61;
COM_POS_OPP_C_ROBOT3 = 62;
COM_POS_OPP_C_ROBOT4 = 63;
COM_REFEREE;

usleep(50);
//RECEIVED VARIABLES FROM DECISION
A = DECISION_STATE;
B = DECISION_POSITION_A;
C = DECISION_POSITION_B;
D = DECISION_POSITION_C;
E = DECISION_BALL_POS;
F = DECISION_OPP1_POS;
G = DECISION_OPP2_POS;
H = DECISION_OPP3_POS;
}

#ifdef DEBUG
cout << "communication received " << A <<" "<< B <<" "<< C <<" "<< D <<" "<<E <<" "<< F <<" "<< G<<" "<< H << endl;
#endif
*/

