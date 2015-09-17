#ifndef ALFA_COMMUNICATION_H
#define ALFA_COMMUNICATION_H

#include <iostream>
#include "string.h"
#include "threading.h"
#include "LinuxNetwork.h"
#include "libUDPclient.h"
#include "data_AlfarobiCommunication.h"
#include "data_RoboCupGameControll.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<sys/socket.h>


// =================== GENERAL SETTING =========================
// +> For Config
#define mSLEEP_COM              10
#define THREAD_PRIORITY_COM     30
// =============================================

// =================== KOMUNIKASI STRATEGI =========================
// +> For Config
#define PORT        3838
#define HOST        "127.0.0.1"

#define SERVER "127.0.0.1"
#define BUFLEN 512  //Max length of buffer


// +> For Use
void establishCommunication();
struct Alfarobi::Player recv_data;
struct Alfarobi::Player send_data;
struct RoboCupGameControlReturnData dado;
// ==========================================

// ====================== GAMECONTROLLER =====================
// +> For Config
#define referreBox_broadcast_ip_port    "255.255.255.255:3838"
#define TEAM_ALFAROBI   				TEAM_CYAN
#define TEAM_MUSUH      				TEAM_MAGENTA
#define NO_TEAM_ALFAROBI                2
#define NO_PLAYER_ALFAROBI				1

// +> For Use
unsigned short state, warnaGawang;
#define NOT_FOUND       404
// ===============================================

using namespace Robot;
using namespace std;

#define juri_myteam			juri.teams[warna_team_alfarobi]
#define juri_sebagai(self)  juri.teams[warna_team_alfarobi].players[self]
#define juri_myself         juri_sebagai(NO_PLAYER_ALFAROBI-1)

struct RoboCupGameControlData juri;
uint8 warna_team_alfarobi;

// pengaman
static bool globalCom_isActivated = false;
static bool alfaCom_isActivated = false;
static bool komunikasiJuri_aktive = false;
static bool cariWarnaTeam_aktive = false;


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void *recebendo(void* arg)
{
	UDPClient client_gcd;
    if(!komunikasiJuri_aktive)
    {
        client_gcd.Initilaize(referreBox_broadcast_ip_port, "udp");
        komunikasiJuri_aktive = true;
    }
    juri_myself.penalty = PENALTY_NONE;
    state = NOT_FOUND;

//~ exit(1);
	while(1)
	{
		//cout << "[=============kl========waiting==]";
		client_gcd.receive(Struct2String(juri), sizeof(juri));
		//cout << "================================\n" << juri.version;
		//~ exit(1);
		if(juri.version == GAMECONTROLLER_STRUCT_VERSION)
		{
			// cekking status pertandingan
			if(juri_myself.penalty == PENALTY_HL_KID_REQUEST_FOR_PICKUP)
				state = 9;
			else if(juri_myself.penalty == PENALTY_HL_KID_REQUEST_FOR_SERVICE)
				state = 10;
			else if(juri_myself.penalty != PENALTY_NONE)
				state = 6;
			else if(juri.state == STATE_INITIAL)
				state = 0;
			else if(juri.state == STATE_READY)
				state = 1;
			else if(juri.state == STATE_SET)
				state = 2;
			else if(juri.kickOffTeam == TEAM_ALFAROBI && juri.state == STATE_PLAYING && (juri.secondaryState == STATE2_NORMAL || juri.secondaryState == STATE2_OVERTIME))
				state = 3;
			else if(juri.kickOffTeam != TEAM_ALFAROBI && juri.state == STATE_PLAYING && (juri.secondaryState == STATE2_NORMAL|| juri.secondaryState == STATE2_OVERTIME))
				state = 4;
			else if(juri.kickOffTeam != TEAM_MUSUH && juri.secondaryState == STATE2_PENALTYSHOOT)
				state = 7;
			else if(juri.kickOffTeam == TEAM_MUSUH && juri.secondaryState == STATE2_PENALTYSHOOT)
				state = 8;
			else if(juri.state == STATE_FINISHED)
				state = 5;
			//~ else
				//~ state = NOT_FOUND;

			// cek gawang
			if(juri.teams[TEAM_ALFAROBI].goalColour == GOAL_BLUE)
				warnaGawang = 11;
			if(juri.teams[TEAM_ALFAROBI].goalColour == GOAL_YELLOW)
				warnaGawang = 12;
		} else {
			state = NOT_FOUND;
		}
		cout<<state<<endl;
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




uint8 whatColorIsMyTeam()
{
    for (int var = 0; var <= 2; ++var) {
        if (juri.teams[var].teamNumber == NO_TEAM_ALFAROBI)
            return juri.teams[var].teamColour;
    }
	state = 190;
    return 190;
}

void *referreBox_communication(void*arg)
{
    globalCom_isActivated = true;
    /// CLIENT UDP GAMECONTROLLER
    UDPClient client_gcd;
	state = 404;
    while(1)
    {
///    / INIT :  CLIENT UDP ==============================
        if(!komunikasiJuri_aktive)
        {
            cout << "\n---------------[Waiting for GAMECONTROLLER to active]---------------\n";
            komunikasiJuri_aktive = client_gcd.Initilaize(referreBox_broadcast_ip_port, "udp");
            cout << "-----------------------[GAMECONTROLLER Connected]---------------------------";
			juri_myself.penalty = PENALTY_NONE;
        }
///    ============================================================

///        / LOOP :  CLIENT UDP ==============================
        if(komunikasiJuri_aktive)
        {
            client_gcd.receive(Struct2String(juri), sizeof(juri));

            if(!cariWarnaTeam_aktive)
            {
                warna_team_alfarobi = whatColorIsMyTeam();
                cariWarnaTeam_aktive = true;
            }

            if(juri.version == GAMECONTROLLER_STRUCT_VERSION)
            {
                // cekking status pertandingan
                if(juri_myself.penalty == PENALTY_HL_KID_REQUEST_FOR_PICKUP)
                    state = 9;
                else if(juri_myself.penalty == PENALTY_HL_KID_REQUEST_FOR_SERVICE)
                    state = 10;
                else if(juri_myself.penalty != PENALTY_NONE)
                    state = 6;
                else if(juri.dropInTeam == warna_team_alfarobi && juri.dropInTime < 10 && juri.dropInTime > 0)
                    state = 12; // 10 detik setelah OUT
                else if(juri.state == STATE_INITIAL)
                    state = 0;
                else if(juri.state == STATE_READY)
                    state = 1;
                else if(juri.state == STATE_SET)
                    state = 2;
                else if(juri.kickOffTeam == DROPBALL && juri.state == STATE_PLAYING && (juri.secondaryState == STATE2_NORMAL || juri.secondaryState == STATE2_OVERTIME))
                    state = 11; // kickoff siapa cepat dia dapat (langsung tendang gawang coy)
                else if(juri.kickOffTeam == warna_team_alfarobi && juri.state == STATE_PLAYING && (juri.secondaryState == STATE2_NORMAL || juri.secondaryState == STATE2_OVERTIME))
                    state = 3; // alfarobi kickoff
                else if(juri.kickOffTeam != warna_team_alfarobi && juri.state == STATE_PLAYING && (juri.secondaryState == STATE2_NORMAL|| juri.secondaryState == STATE2_OVERTIME))
                    state = 4; // musuh kickoff
                else if(juri.kickOffTeam == warna_team_alfarobi && juri.secondaryState == STATE2_PENALTYSHOOT)
                    state = 7;
                else if(juri.kickOffTeam != warna_team_alfarobi && juri.secondaryState == STATE2_PENALTYSHOOT)
                    state = 8;
                else if(juri.state == STATE_FINISHED)
                    state = 5;
                else
                    state = NOT_FOUND;

                // cek gawang
                if(juri.teams[warna_team_alfarobi].goalColour == GOAL_BLUE)
                    warnaGawang = 11;
                else if(juri.teams[warna_team_alfarobi].goalColour == GOAL_YELLOW)
                    warnaGawang = 12;
            }
            else
            {
                state = NOT_FOUND;
            }
        }
///        ============================================================
        usleep(mSLEEP_COM*1000);
    }
    komunikasiJuri_aktive = false;
    globalCom_isActivated = false;
    cariWarnaTeam_aktive = false;
}

void *client_communication(void *arg)
{
    globalCom_isActivated = true;
    /// CLIENT TCP KIPER
    LinuxClient *socket;

    while (1)
    {
///    / INIT : CLIENT TCP ==============================
        if(!alfaCom_isActivated)
        {
            try
            {
				socket = new LinuxClient(PORT);
                cout << "\n-----------------[Waiting for kiper connection]-----------------\n";
                socket->connect(HOST, PORT);
                cout << "\n-----------------[kiper connected]-----------------\n";
                alfaCom_isActivated = true;
            } catch(LinuxSocketException e)
            {
                cerr << "\n=====================Erro kiper===+>" << e.description() << endl;
                alfaCom_isActivated = false;
            }
        }
///    ============================================================

///        / LOOP : CLIENT TCP ==============================
        if(alfaCom_isActivated)
        {
            try
            {
                socket->send(AlfaStruct2String(dado), sizeof(dado));
                socket->recv(AlfaStruct2String(dado), sizeof(dado));
               // cout<<" Enviado ...."<<endl;
               //cout<<"Dado: "<<dado.message<<endl;
            } catch(LinuxSocketException e)
            {
                cerr << "=========================Erro +>" << e.description() << endl;
                alfaCom_isActivated = false;
            }
        }
///        ============================================================

        usleep(mSLEEP_COM*1000);
    }
    alfaCom_isActivated = false;
    globalCom_isActivated = false;
}

void establishCommunication()
{
    static pthread_t client_t, referreBox_t;

    strcpy(send_data.header, ALFAROBI_STRUCT_HEADER);

    //dado de resposta --> alterar apenas team e player
    strcpy(dado.header,GAMECONTROLLER_RETURN_STRUCT_HEADER);
    dado.version = GAMECONTROLLER_RETURN_STRUCT_VERSION;
    dado.team = 2;
    dado.player = 1;
    dado.message = GAMECONTROLLER_RETURN_MSG_ALIVE;
    if(!globalCom_isActivated)
    {
       // threadInitialize(referreBox_t, referreBox_communication, THREAD_PRIORITY_COM);
        //threadInitialize(client_t, client_communication, THREAD_PRIORITY_COM);
        threadInitialize(referreBox_t, recebendo, THREAD_PRIORITY_COM);
        threadInitialize(client_t, enviando, THREAD_PRIORITY_COM);
    }
}

#endif // ALFA_COMMUNICATION_H
