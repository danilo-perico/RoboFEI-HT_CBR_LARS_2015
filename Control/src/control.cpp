#include <iostream>
#include <stdio.h>
#include <blackboard.h>
#include <unistd.h>
#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <iostream>

#define RESOLUCAO_X 640
#define RESOLUCAO_Y 480

#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define VEL 200 // Define a velocidade
#define VEL_ANDAR 205 //define velocidade para andar
#define VEL_ANDAR2 80


//#define DEBUG
//#define _cpluplus

//#ifdef _cpluplus
//teste
//#endif

using namespace std;

int inclina=55;

//==============================================================================================================
unsigned int StandupPos_corrigido[22] = {304, 594, //01 , 02, (cabeça)
                            528, 274, 350, //03, 04, 05, (braço direito)
                            571, 675, 682, //06, 07, 08, (braço esquerdo)
                            581, 845, //09, 10, (torso) //10 era 835
                            585, 512, 498, 510, 498, 562, //11, 12, 13, 14, 15, 16 (perna direita)
                            601, 512, 512, 510, 508, 467}; //17, 18, 19, 20, 21, 22 (perna esquerda)

//==============================================================================================================
void espera_mov()
{
    for(int i=3;i<23;i++)
    {
       do
       {
        usleep(2000); //Espera 2 milisegundos evitando uso excessivo do processador
       }while(dxl_read_byte( i, P_MOVING ) != 0);
    }
}


//==============================================================================================================

void robo_ereto_corrigido()
{
    for(int i = 3;i<23;i++)
    {
       dxl_write_word(i, MOVING_SPEED, 100);
       dxl_write_word(i, P_GOAL_POSITION_L, StandupPos_corrigido[i-1]);
    }
}

//==============================================================================================================

void andar_curto(int passos)
{

//int braco = 300;
//int a = 2;
int ctrl;

espera_mov();
//-------conf veloc 40 ---------------------------------------------------------
for(int i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);

for (ctrl = 0; ctrl< passos; ctrl++){ // ******* for para quantidade de passos******

//======= 1o passo com a perna direira =======================================

//if (a == 2){
//------conf veloc para os braços ---------------------------------------------
//dxl_write_word(4, MOVING_SPEED, 200);
//dxl_write_word(7, MOVING_SPEED, 200);

//------ inclina robo para o lado esquerdo ------------------------------------
//dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.3);
//dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.3);
//dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.3);
//dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.3);

dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
espera_mov();

/*
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.35);
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.4);

dxl_write_word(13, P_GOAL_POSITION_L, 498-inclina+20);
dxl_write_word(19, P_GOAL_POSITION_L, 512-inclina+25);
dxl_write_word(15, P_GOAL_POSITION_L, 498-inclina+25);
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina+25);//20
//dxl_write_word(4, P_GOAL_POSITION_L, 274+braco);
//dxl_write_word(7, P_GOAL_POSITION_L, 675);*/
espera_mov();

    dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.20);
    dxl_write_word(14, MOVING_SPEED, VEL_ANDAR*0.20);
    dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.20);

    dxl_write_word(13, P_GOAL_POSITION_L, 498-inclina+10);
    dxl_write_word(19, P_GOAL_POSITION_L, 512-inclina+20);
    dxl_write_word(15, P_GOAL_POSITION_L, 498-inclina+10);
    dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina+5);//25
    dxl_write_word(12, P_GOAL_POSITION_L, 512+10);
    dxl_write_word(14, P_GOAL_POSITION_L, 510-10);
    dxl_write_word(16, P_GOAL_POSITION_L, 562-10);//25
    //dxl_write_word(22, P_GOAL_POSITION_L, 467+5);//25
    espera_mov();
//================================================================================
//levanta_perna_dir(){
//--------------conf velocidade 40------------------------------------------------
for(int i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.6);
//------conf veloc para os braços ---------------------------------------------
//dxl_write_word(4, MOVING_SPEED, 100);
//dxl_write_word(7, MOVING_SPEED, 100);
    //------------- conf velocidade p/ levantar perna dir ---------------------------
    dxl_write_word(14, MOVING_SPEED, 2*VEL_ANDAR);
    dxl_write_word(12, MOVING_SPEED, 2*VEL_ANDAR*0.4);
    dxl_write_word(16, MOVING_SPEED, 2*VEL_ANDAR*0.66);
    dxl_write_word(21, MOVING_SPEED, 2*VEL_ANDAR*.6);
    dxl_write_word(22, MOVING_SPEED, 2*VEL_ANDAR);
    //------------- levanta perna dir -----------------------------------------------
    dxl_write_word(12, P_GOAL_POSITION_L, 512-40);//115
    dxl_write_word(14, P_GOAL_POSITION_L, 510+80);//300
    dxl_write_word(16, P_GOAL_POSITION_L, 562+45);//190
    dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-10);//corrigi inclinação do servo 21
    dxl_write_word(22, P_GOAL_POSITION_L, 467+2);
    espera_mov();

//---------- abaixa a perna direita ---------------------------------------------
//------------conf velocidade para abaixar a perna dir --------------------------
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.5);
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR);

dxl_write_word(12, P_GOAL_POSITION_L, 512-37);
dxl_write_word(14, P_GOAL_POSITION_L, 510-12);
dxl_write_word(13, P_GOAL_POSITION_L, 498-45);//71
dxl_write_word(11, P_GOAL_POSITION_L, 585+27);
dxl_write_word(16, 24, 0);
dxl_write_word(15, 24, 0);
espera_mov();
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina);
espera_mov();
//-----conf velocidade de 40 para voltar a posição ereto ------------------------
for(int i = 10;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);


dxl_write_word(16, 24, 1);
dxl_write_word(15, 24, 1);
//----volta a posição ereto -----------------------------------------------------
dxl_write_word(11, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(11, P_GOAL_POSITION_L, 585+10);
dxl_write_word(16, P_GOAL_POSITION_L, 517+10);
dxl_write_word(22, P_GOAL_POSITION_L, 467-5);
espera_mov();
//a=0;

//------ inclina robo para o lado direito ------------------------------------
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
//dxl_write_word(11, P_GOAL_POSITION_L, 607);
dxl_write_word(12, P_GOAL_POSITION_L, 475);
dxl_write_word(14, P_GOAL_POSITION_L, 499);
dxl_write_word(16, P_GOAL_POSITION_L, 535);
dxl_write_word(17, P_GOAL_POSITION_L, 590+15);
dxl_write_word(18, P_GOAL_POSITION_L, 516);
dxl_write_word(20, P_GOAL_POSITION_L, 513);
dxl_write_word(22, P_GOAL_POSITION_L, 469);
espera_mov();
dxl_write_word(13, P_GOAL_POSITION_L, 498+inclina);
dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina+17);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+18);
dxl_write_word(21, P_GOAL_POSITION_L, 508+inclina);
dxl_write_word(18, P_GOAL_POSITION_L, 512+20);
dxl_write_word(20, P_GOAL_POSITION_L, 506+15);
dxl_write_word(22, P_GOAL_POSITION_L, 467-15);
espera_mov();

//levanta_perna_esq(){
//--------------conf velocidade 40------------------------------------------------
for(int i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//------------- conf velocidade p/ levantar perna esq ---------------------------
dxl_write_word(20, MOVING_SPEED, 3*VEL_ANDAR);
dxl_write_word(18, MOVING_SPEED, 3*VEL_ANDAR*0.4);
dxl_write_word(22, MOVING_SPEED, 3*VEL_ANDAR*0.66);
dxl_write_word(15, MOVING_SPEED, 3*VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, 3*VEL_ANDAR*1.5);
//------------- levanta perna esq -----------------------------------------------
dxl_write_word(18, P_GOAL_POSITION_L, 512+60);
dxl_write_word(20, P_GOAL_POSITION_L, 510-50);
dxl_write_word(22, P_GOAL_POSITION_L, 467-45);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+22);//corrigi inclinação do servo 25
dxl_write_word(16, P_GOAL_POSITION_L, 562-23);
espera_mov();

//-----movimenta perna para frente --------------------------------------------
dxl_write_word(12, MOVING_SPEED, 3*VEL_ANDAR*0.872);
dxl_write_word(15, MOVING_SPEED, 3*VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, 3*VEL_ANDAR);
dxl_write_word(17, MOVING_SPEED, 3*VEL_ANDAR*0.345);
dxl_write_word(18, MOVING_SPEED, 3*VEL_ANDAR*0.109);
dxl_write_word(19, MOVING_SPEED, 3*VEL_ANDAR*0.4);
dxl_write_word(20, MOVING_SPEED, 3*VEL_ANDAR*0.364);
dxl_write_word(22, MOVING_SPEED, 3*VEL_ANDAR*0.6);
//dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.139);

dxl_write_word(20, P_GOAL_POSITION_L, 510-5);
dxl_write_word(18, P_GOAL_POSITION_L, 512+65);
dxl_write_word(22, P_GOAL_POSITION_L, 467+5);//27
//dxl_write_word(11, P_GOAL_POSITION_L, 585);
dxl_write_word(17, P_GOAL_POSITION_L, 601-15);
//dxl_write_word(19, P_GOAL_POSITION_L, 512+50);
//dxl_write_word(12, P_GOAL_POSITION_L, 512+11);
//dxl_write_word(16, P_GOAL_POSITION_L, 562-16);
//dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+12);
//dxl_write_word(19, P_GOAL_POSITION_L, 512+71);
espera_mov();
//dxl_write_word(16, P_GOAL_POSITION_L, 562-13);

//-------- abaixa perna esq --------------------------------------------------
dxl_write_word(20, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.5);
//dxl_write_word(19, MOVING_SPEED, 110);
//dxl_write_word(17, MOVING_SPEED, 110);

dxl_write_word(22, 24, 0);
dxl_write_word(21, 24, 0);
dxl_write_word(18, P_GOAL_POSITION_L, 512+52);
dxl_write_word(20, P_GOAL_POSITION_L, 510);
dxl_write_word(11, P_GOAL_POSITION_L, 585);
dxl_write_word(17, P_GOAL_POSITION_L, 601-13);
espera_mov();

dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina);
//dxl_write_word(16, P_GOAL_POSITION_L, 562-0);
espera_mov();
dxl_write_word(22, 24, 1);
dxl_write_word(21, 24, 1);
dxl_write_word(21, P_GOAL_POSITION_L, 508+55);
dxl_write_word(22, P_GOAL_POSITION_L, 467+5);
espera_mov();
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.480);
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.714);
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR*0.143);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.299);
dxl_write_word(17, MOVING_SPEED, VEL_ANDAR*0.169);
dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.675);
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.935);
//dxl_write_word(20, MOVING_SPEED, VEL_ANDAR*0);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.714);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.065);

    for(int i = 10;i<23;i++)
    {
       dxl_write_word(i, MOVING_SPEED, 100);
       dxl_write_word(i, P_GOAL_POSITION_L, StandupPos_corrigido[i-1]);
    }
espera_mov();
//}
}
}



//==============================================================================================================

void virar_dir(int passos)
{
for(int w=0; w<passos; w++)
{
//int braco = 300;
int a = 2;
int i;
int ctrl;

espera_mov();
//-------conf veloc 40 ---------------------------------------------------------
for(i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.2);

if (a == 2){
//------conf veloc para os braços ---------------------------------------------
//dxl_write_word(4, MOVING_SPEED, 200);
//dxl_write_word(7, MOVING_SPEED, 200);

//------ inclina robo para o lado esquerdo ------------------------------------
    dxl_write_word(13, MOVING_SPEED, 0.5*VEL_ANDAR*0.35);
    dxl_write_word(19, MOVING_SPEED, 0.5*VEL_ANDAR*0.35);
    dxl_write_word(15, MOVING_SPEED, 0.5*VEL_ANDAR*0.35);
    dxl_write_word(21, MOVING_SPEED, 0.5*VEL_ANDAR*0.35);
    dxl_write_word(12, MOVING_SPEED, 0.5*VEL_ANDAR*0.20);
    dxl_write_word(14, MOVING_SPEED, 0.5*VEL_ANDAR*0.20);
    dxl_write_word(16, MOVING_SPEED, 0.5*VEL_ANDAR*0.20);

    dxl_write_word(13, P_GOAL_POSITION_L, 498-inclina+10);
    dxl_write_word(19, P_GOAL_POSITION_L, 512-inclina+20);
    dxl_write_word(15, P_GOAL_POSITION_L, 498-inclina+10);
    dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina+13);//25
    dxl_write_word(12, P_GOAL_POSITION_L, 512+6);
    dxl_write_word(14, P_GOAL_POSITION_L, 510);
    dxl_write_word(16, P_GOAL_POSITION_L, 562-10);//25
    //dxl_write_word(22, P_GOAL_POSITION_L, 467+5);//25
espera_mov();
usleep(1000000);
//================================================================================
//levanta_perna_dir(){
//--------------conf velocidade 40------------------------------------------------
//------conf veloc para os braços ---------------------------------------------
//dxl_write_word(4, MOVING_SPEED, 100);
//dxl_write_word(7, MOVING_SPEED, 100);
//------------- conf velocidade p/ levantar perna dir ---------------------------
dxl_write_word(14, MOVING_SPEED, 0.5*VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, 0.5*VEL_ANDAR*0.4);
dxl_write_word(16, MOVING_SPEED, 0.5*VEL_ANDAR*0.66);
dxl_write_word(21, MOVING_SPEED, 0.5*VEL_ANDAR*2);
dxl_write_word(22, MOVING_SPEED, 0.5*VEL_ANDAR);
dxl_write_word(11, MOVING_SPEED, 0.5*VEL_ANDAR);
//------------- levanta perna dir -----------------------------------------------
dxl_write_word(12, P_GOAL_POSITION_L, 512-55);//115
dxl_write_word(14, P_GOAL_POSITION_L, 510+150);//300
dxl_write_word(16, P_GOAL_POSITION_L, 562+95);//190
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-13);//corrigi inclinação do servo 21
dxl_write_word(22, P_GOAL_POSITION_L, 467+2);
espera_mov();
//dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-13);//corrigi inclinação do servo 21
//espera_mov_ID(13);
//------------vira perna direita ----------------------------------------------
dxl_write_word(11, P_GOAL_POSITION_L, 585+90);
espera_mov();

for(i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.3);

//-----------abaixa perna direita ---------------------------------------------
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR*0.4);

dxl_write_word(12, P_GOAL_POSITION_L, 512-15);
dxl_write_word(14, P_GOAL_POSITION_L, 510-28);
dxl_write_word(16, 24, 0);
dxl_write_word(15, 24, 0);
espera_mov();
dxl_write_word(16, 24, 1);
dxl_write_word(15, 24, 1);
espera_mov();

//------------volta da posição inclinada -------------------------------------
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR2);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR2);
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR2*0.78);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR2);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR2*0.6);
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR2*0.45);

dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(13, P_GOAL_POSITION_L, 498+12);//510
dxl_write_word(15, P_GOAL_POSITION_L, 498);
//dxl_write_word(16, P_GOAL_POSITION_L, 562+42);
//dxl_write_word(14, P_GOAL_POSITION_L, 510+73);
dxl_write_word(22, P_GOAL_POSITION_L, 467-10);
//dxl_write_word(12, P_GOAL_POSITION_L, 512-30);
dxl_write_word(12, P_GOAL_POSITION_L, 512-10);
dxl_write_word(14, P_GOAL_POSITION_L, 510+10);
dxl_write_word(16, P_GOAL_POSITION_L, 562-10);

espera_mov();
//-------------inclina para o lado direito -------------------------------------
dxl_write_word(19, MOVING_SPEED, 0.7*VEL_ANDAR2*0.9);
dxl_write_word(21, MOVING_SPEED, 0.7*VEL_ANDAR2*0.9);
dxl_write_word(13, MOVING_SPEED, 0.7*VEL_ANDAR2*0.7);
dxl_write_word(15, MOVING_SPEED, 0.7*VEL_ANDAR2*0.95);
dxl_write_word(16, MOVING_SPEED, 0.7*VEL_ANDAR2*0.8);
dxl_write_word(22, MOVING_SPEED, 0.7*VEL_ANDAR2*0.8);
dxl_write_word(14, MOVING_SPEED, 0.7*VEL_ANDAR2*0.79);
dxl_write_word(12, MOVING_SPEED, 0.7*VEL_ANDAR2);
dxl_write_word(18, MOVING_SPEED, 0.7*VEL_ANDAR2);
dxl_write_word(12, P_GOAL_POSITION_L, 512-10);
dxl_write_word(18, P_GOAL_POSITION_L, 512-20);
dxl_write_word(13, P_GOAL_POSITION_L, 498+inclina);//553 -> 43
dxl_write_word(15, P_GOAL_POSITION_L, 498+58);// -> 58
dxl_write_word(16, P_GOAL_POSITION_L, 627);//-> 61
dxl_write_word(22, P_GOAL_POSITION_L, 427);//-> 61
dxl_write_word(14, P_GOAL_POSITION_L, 510+80);// -> 48
dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina);// -> 55
dxl_write_word(21, P_GOAL_POSITION_L, 508+inclina+10);// -> 55
espera_mov();
//----corrigi velocidades ------------------------------------------------------
for(i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//------------- conf velocidade p/ levantar perna esq ---------------------------
dxl_write_word(20, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.66);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR);
//------------- levanta perna esq -----------------------------------------------
dxl_write_word(17, P_GOAL_POSITION_L, 601+20);
dxl_write_word(18, P_GOAL_POSITION_L, 512+50);
dxl_write_word(20, P_GOAL_POSITION_L, 510-150);
dxl_write_word(22, P_GOAL_POSITION_L, 467-95);
dxl_write_word(15, P_GOAL_POSITION_L, 498+81);// -> 58
dxl_write_word(16, P_GOAL_POSITION_L, 562+70);
espera_mov();
//--------- vira perna esquerda -------------------------------------------------
dxl_write_word(19, P_GOAL_POSITION_L, 512+76);
dxl_write_word(18, P_GOAL_POSITION_L, 592);
dxl_write_word(20, P_GOAL_POSITION_L, 319);
dxl_write_word(21, P_GOAL_POSITION_L, 508+56);
dxl_write_word(17, P_GOAL_POSITION_L, 601+68);
dxl_write_word(22, P_GOAL_POSITION_L, 380);
espera_mov();
//------coloca o pé esq no chão --------------------------------------------------
dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.2);
dxl_write_word(20, MOVING_SPEED, VEL_ANDAR*0.2);
dxl_write_word(21, 24, 0);
dxl_write_word(22, 24, 0);
dxl_write_word(18, P_GOAL_POSITION_L, 512+43);
dxl_write_word(20, P_GOAL_POSITION_L, 467);
espera_mov();
dxl_write_word(21, 24, 1);
dxl_write_word(22, 24, 1);
espera_mov();

// ------ volta a posição inicial ereta ------------------------------------------
int servo21 = dxl_read_word(21, P_PRESENT_POSITION_L);

dxl_write_word(11, MOVING_SPEED, VEL_ANDAR2);//-> 90
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR2*0.33);//-> 30
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR2*0.61);//-> 55
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR2*0.89);//-> 80
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR2*0.89);//-> 81
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR2*1.1);//-> 64    0.71
dxl_write_word(17, MOVING_SPEED, VEL_ANDAR2*0.75);//-> 68
dxl_write_word(18, MOVING_SPEED, VEL_ANDAR2*0.48);//-> 43
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR2*0.84);//-> 76
dxl_write_word(20, MOVING_SPEED, VEL_ANDAR2);//-> 0
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR2*(fabs(508-servo21))/90);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR2*1.1);

for(i = 3;i<23;i++)
{
dxl_write_word(i, P_GOAL_POSITION_L, StandupPos_corrigido[i-1]);
}
dxl_write_word(13, P_GOAL_POSITION_L, 498-7);
dxl_write_word(19, P_GOAL_POSITION_L, 512+18);
espera_mov();
dxl_write_word(13, P_GOAL_POSITION_L, 498-2);
dxl_write_word(19, P_GOAL_POSITION_L, 512+2);
espera_mov();
robo_ereto_corrigido();
usleep(500000);
/**/
}
}
}

//==============================================================================================================

void andar_rapido(int passos)
{

//int braco = 300;
int a = 2;
int ctrl;

espera_mov();
//-------conf veloc 40 ---------------------------------------------------------
for(int i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);

for (ctrl = 0; ctrl< passos; ctrl++){ // ******* for para quantidade de passos******

//======= 1o passo com a perna direira =======================================

if (a == 2){
//------conf veloc para os braços ---------------------------------------------
//dxl_write_word(4, MOVING_SPEED, 200);
//dxl_write_word(7, MOVING_SPEED, 200);

//------ inclina robo para o lado esquerdo ------------------------------------
//dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.3);
//dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.3);
//dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.3);
//dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.3);

dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
espera_mov();

/*
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.35);
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.4);

dxl_write_word(13, P_GOAL_POSITION_L, 498-inclina+20);
dxl_write_word(19, P_GOAL_POSITION_L, 512-inclina+25);
dxl_write_word(15, P_GOAL_POSITION_L, 498-inclina+25);
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina+25);//20
//dxl_write_word(4, P_GOAL_POSITION_L, 274+braco);
//dxl_write_word(7, P_GOAL_POSITION_L, 675);*/
espera_mov();
    //---inclina----------------------
    dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.20);
    dxl_write_word(14, MOVING_SPEED, VEL_ANDAR*0.20);
    dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.20);

    dxl_write_word(13, P_GOAL_POSITION_L, 498-inclina+10);
    dxl_write_word(19, P_GOAL_POSITION_L, 512-inclina+20);
    dxl_write_word(15, P_GOAL_POSITION_L, 498-inclina+10);
    dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina+5);//25
    dxl_write_word(12, P_GOAL_POSITION_L, 512+10);
    dxl_write_word(14, P_GOAL_POSITION_L, 510-10);
    dxl_write_word(16, P_GOAL_POSITION_L, 562-10);//25
    //dxl_write_word(22, P_GOAL_POSITION_L, 467+5);//25
    //espera_mov_ID(13);
    //espera_mov_ID(19);
    //espera_mov_ID(15);
    espera_mov();


//================================================================================
//levanta_perna_dir(){
//--------------conf velocidade 40------------------------------------------------
for(int i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.6);
//------conf veloc para os braços ---------------------------------------------
//dxl_write_word(4, MOVING_SPEED, 100);
//dxl_write_word(7, MOVING_SPEED, 100);
    //------------- conf velocidade p/ levantar perna dir ---------------------------
    dxl_write_word(14, MOVING_SPEED, 3*VEL_ANDAR);
    dxl_write_word(12, MOVING_SPEED, 3*VEL_ANDAR*0.4);
    dxl_write_word(16, MOVING_SPEED, 3*VEL_ANDAR*0.66);
    dxl_write_word(21, MOVING_SPEED, 3*VEL_ANDAR*.6);
    dxl_write_word(22, MOVING_SPEED, 3*VEL_ANDAR);
    //------------- levanta perna dir -----------------------------------------------
    dxl_write_word(12, P_GOAL_POSITION_L, 512-40);//115
    dxl_write_word(14, P_GOAL_POSITION_L, 510+80);//300
    dxl_write_word(16, P_GOAL_POSITION_L, 562+45);//190
    dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-10);//corrigi inclinação do servo 21
    dxl_write_word(22, P_GOAL_POSITION_L, 467);
    //espera_mov();
    //usleep(2000);
    espera_mov();
    //espera_mov_ID(21);
    //espera_mov_ID(12);


//---------- abaixa a perna direita ---------------------------------------------
//------------conf velocidade para abaixar a perna dir --------------------------
dxl_write_word(14, MOVING_SPEED, 2*VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, 2*VEL_ANDAR*0.4);
dxl_write_word(16, MOVING_SPEED, 2*VEL_ANDAR*0.5);
dxl_write_word(13, MOVING_SPEED, 2*VEL_ANDAR);

dxl_write_word(12, P_GOAL_POSITION_L, 512-37);
dxl_write_word(14, P_GOAL_POSITION_L, 510-12);
dxl_write_word(13, P_GOAL_POSITION_L, 498-45);//71
dxl_write_word(11, P_GOAL_POSITION_L, 585+27);
dxl_write_word(16, 24, 0);
dxl_write_word(15, 24, 0);
espera_mov();
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina);
espera_mov();
//-----conf velocidade de 40 para voltar a posição ereto ------------------------
for(int i = 10;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);


dxl_write_word(16, 24, 1);
dxl_write_word(15, 24, 1);
//----volta a posição ereto -----------------------------------------------------
dxl_write_word(11, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(11, P_GOAL_POSITION_L, 585+10);
dxl_write_word(16, P_GOAL_POSITION_L, 517+10);
dxl_write_word(22, P_GOAL_POSITION_L, 467-5);
espera_mov();
a=0;

}


//====== passo longo com a perna esquerda ==========================================

if (a == 0){
//------ inclina robo para o lado direito ------------------------------------
//dxl_write_word(13, P_GOAL_POSITION_L, 498);
//dxl_write_word(19, P_GOAL_POSITION_L, 512);
//dxl_write_word(15, P_GOAL_POSITION_L, 498);
//dxl_write_word(21, P_GOAL_POSITION_L, 508);
//dxl_write_word(11, P_GOAL_POSITION_L, 607);
//dxl_write_word(12, P_GOAL_POSITION_L, 512-20);
//dxl_write_word(14, P_GOAL_POSITION_L, 499);
//dxl_write_word(16, P_GOAL_POSITION_L, 535);
//dxl_write_word(17, P_GOAL_POSITION_L, 590+15);
//dxl_write_word(18, P_GOAL_POSITION_L, 512+40);
//dxl_write_word(20, P_GOAL_POSITION_L, 513);
//dxl_write_word(22, P_GOAL_POSITION_L, 469);
//espera_mov();//---- inclina-------------------------
dxl_write_word(13, MOVING_SPEED, 0.4*VEL_ANDAR*0.753);
dxl_write_word(19, MOVING_SPEED, 0.4*VEL_ANDAR*0.959);
dxl_write_word(15, MOVING_SPEED, 0.4*VEL_ANDAR);
dxl_write_word(21, MOVING_SPEED, 0.4*VEL_ANDAR*0.890);
dxl_write_word(18, MOVING_SPEED, 0.4*VEL_ANDAR*0.548);
dxl_write_word(20, MOVING_SPEED, 0.4*VEL_ANDAR*0.205);
dxl_write_word(22, MOVING_SPEED, 0.4*VEL_ANDAR*0.274);
dxl_write_word(17, MOVING_SPEED, 0.4*VEL_ANDAR*0.274);
dxl_write_word(12, MOVING_SPEED, 0.4*VEL_ANDAR*0.233);
dxl_write_word(14, MOVING_SPEED, 0.4*VEL_ANDAR*0.712);
//dxl_write_word(16, MOVING_SPEED, VEL_ANDAR);

dxl_write_word(13, P_GOAL_POSITION_L, 498+inclina);
dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina+15);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+18);
dxl_write_word(21, P_GOAL_POSITION_L, 508+inclina+10);
dxl_write_word(18, P_GOAL_POSITION_L, 512-40);
dxl_write_word(20, P_GOAL_POSITION_L, 506+15);
dxl_write_word(22, P_GOAL_POSITION_L, 467-30);
dxl_write_word(17, P_GOAL_POSITION_L, 590+20);

dxl_write_word(12, P_GOAL_POSITION_L, 512-20);
dxl_write_word(14, P_GOAL_POSITION_L, 510+30);
dxl_write_word(16, P_GOAL_POSITION_L, 562+10);

espera_mov();

//levanta_perna_esq(){
//--------------conf velocidade 40------------------------------------------------
for(int i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//------------- conf velocidade p/ levantar perna esq ---------------------------
dxl_write_word(20, MOVING_SPEED, 2*VEL_ANDAR);
dxl_write_word(18, MOVING_SPEED, 2*VEL_ANDAR*0.4);
dxl_write_word(22, MOVING_SPEED, 2*VEL_ANDAR*0.66);
dxl_write_word(15, MOVING_SPEED, 2*VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, 2*VEL_ANDAR*1.5);
//------------- levanta perna esq -----------------------------------------------
dxl_write_word(18, P_GOAL_POSITION_L, 512+10);
dxl_write_word(20, P_GOAL_POSITION_L, 510-92);
dxl_write_word(22, P_GOAL_POSITION_L, 467-70);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+25);//corrigi inclinação do servo 25
dxl_write_word(16, P_GOAL_POSITION_L, 562+18);
espera_mov();

//dxl_write_word(18, P_GOAL_POSITION_L, 512+125);
//-----movimenta perna para frente --------------------------------------------
//dxl_write_word(22, P_GOAL_POSITION_L, 467+52);

dxl_write_word(12, MOVING_SPEED, 3*VEL_ANDAR*0.150);
dxl_write_word(15, MOVING_SPEED, 3*VEL_ANDAR*0.2);
dxl_write_word(16, MOVING_SPEED, 3*VEL_ANDAR*0.2);
dxl_write_word(17, MOVING_SPEED, 3*VEL_ANDAR*0.522);
dxl_write_word(18, MOVING_SPEED, 3*VEL_ANDAR);
dxl_write_word(19, MOVING_SPEED, 3*VEL_ANDAR*0.2);
dxl_write_word(20, MOVING_SPEED, 3*VEL_ANDAR*0.254);
dxl_write_word(22, MOVING_SPEED, 3*VEL_ANDAR*0.821);



//dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.139);

dxl_write_word(20, P_GOAL_POSITION_L, 510-75);
dxl_write_word(18, P_GOAL_POSITION_L, 512+77);
dxl_write_word(22, P_GOAL_POSITION_L, 467-15);//27
dxl_write_word(17, P_GOAL_POSITION_L, 601-15);
//dxl_write_word(19, P_GOAL_POSITION_L, 512+50);
dxl_write_word(12, P_GOAL_POSITION_L, 512-10);
dxl_write_word(16, P_GOAL_POSITION_L, 562+18);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+20);
dxl_write_word(19, P_GOAL_POSITION_L, 512+71);
espera_mov();
//dxl_write_word(16, P_GOAL_POSITION_L, 562-13);
//dxl_write_word(17, P_GOAL_POSITION_L, 601-25);
//espera_mov();

//-------- abaixa perna esq --------------------------------------------------
dxl_write_word(20, MOVING_SPEED, 2*VEL_ANDAR);
dxl_write_word(18, MOVING_SPEED, 2*VEL_ANDAR*0.4);
dxl_write_word(22, MOVING_SPEED, 2*VEL_ANDAR*0.5);
//dxl_write_word(19, MOVING_SPEED, 110);
//dxl_write_word(17, MOVING_SPEED, 110);

dxl_write_word(22, 24, 0);
dxl_write_word(21, 24, 0);
dxl_write_word(18, P_GOAL_POSITION_L, 512+60);
dxl_write_word(20, P_GOAL_POSITION_L, 510-45);
//dxl_write_word(11, P_GOAL_POSITION_L, 585);
espera_mov();

dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+15);
//dxl_write_word(16, P_GOAL_POSITION_L, 562-0);
espera_mov();
dxl_write_word(22, 24, 1);
dxl_write_word(21, 24, 1);

//-----conf velocidade de 40 para voltar a posição ereto ------------------------
for(int i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//----volta a posição ereto -------------------------  --------------------------
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
//dxl_write_word(17, P_GOAL_POSITION_L, 601-20);
dxl_write_word(22, P_GOAL_POSITION_L, 467);
dxl_write_word(17, P_GOAL_POSITION_L, 601-5);

dxl_write_word(16, P_GOAL_POSITION_L, 562+30);
dxl_write_word(14, P_GOAL_POSITION_L, 510+45);
dxl_write_word(12, P_GOAL_POSITION_L, 512-30);

espera_mov();

a = 1;

}

//========== passo longo perna direita ==========================================

if (a == 1){
//------ inclina robo para o lado esquerdo ------------------------------------

//dxl_write_word(13, P_GOAL_POSITION_L, 498);
//dxl_write_word(19, P_GOAL_POSITION_L, 512);
//dxl_write_word(15, P_GOAL_POSITION_L, 498);
//dxl_write_word(21, P_GOAL_POSITION_L, 508);
//dxl_write_word(11, P_GOAL_POSITION_L, 581);
//dxl_write_word(12, P_GOAL_POSITION_L, 517);
//dxl_write_word(14, P_GOAL_POSITION_L, 492);
//dxl_write_word(16, P_GOAL_POSITION_L, 565-20);//20
//dxl_write_word(17, P_GOAL_POSITION_L, 563);
//dxl_write_word(18, P_GOAL_POSITION_L, 548);
//dxl_write_word(20, P_GOAL_POSITION_L, 504);
//dxl_write_word(22, P_GOAL_POSITION_L, 468);
//espera_mov();

dxl_write_word(11, P_GOAL_POSITION_L, 585);
dxl_write_word(13, P_GOAL_POSITION_L, 498-inclina);
dxl_write_word(19, P_GOAL_POSITION_L, 512-inclina);
dxl_write_word(15, P_GOAL_POSITION_L, 498-inclina);
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina);

dxl_write_word(16, P_GOAL_POSITION_L, 562+30);
dxl_write_word(22, P_GOAL_POSITION_L, 467-10);
//dxl_write_word(16, P_GOAL_POSITION_L, 565);

espera_mov();

//levanta_perna_dir(){
//--------------conf velocidade 40------------------------------------------------
for(int i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//------------- conf velocidade p/ levantar perna esq ---------------------------
dxl_write_word(14, MOVING_SPEED, 2*VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, 2*VEL_ANDAR*0.4);
dxl_write_word(16, MOVING_SPEED, 2*VEL_ANDAR*0.66);
dxl_write_word(21, MOVING_SPEED, 2*VEL_ANDAR);
dxl_write_word(22, MOVING_SPEED, 2*VEL_ANDAR);
//------------- levanta perna dir -----------------------------------------------
//dxl_write_word(18, P_GOAL_POSITION_L, 512);
dxl_write_word(12, P_GOAL_POSITION_L, 512-55);//115
dxl_write_word(14, P_GOAL_POSITION_L, 510+150);//300
dxl_write_word(16, P_GOAL_POSITION_L, 562+95);//190
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-22);//corrigi inclinação do servo 21
dxl_write_word(22, P_GOAL_POSITION_L, 467-14);//2
espera_mov();

//dxl_write_word(18, P_GOAL_POSITION_L, 512+125);
//-----movimenta perna para frente --------------------------------------------
//dxl_write_word(22, P_GOAL_POSITION_L, 467+52);
dxl_write_word(14, MOVING_SPEED, 3*VEL_ANDAR*0.737);
dxl_write_word(12, MOVING_SPEED, 3*VEL_ANDAR*0.684);
dxl_write_word(16, MOVING_SPEED, 3*VEL_ANDAR);

dxl_write_word(14, P_GOAL_POSITION_L, 510+80);//37
dxl_write_word(12, P_GOAL_POSITION_L, 512-120);
dxl_write_word(16, P_GOAL_POSITION_L, 562);
dxl_write_word(11, P_GOAL_POSITION_L, 585+15);
dxl_write_word(13, P_GOAL_POSITION_L, 498-50);
dxl_write_word(18, P_GOAL_POSITION_L, 512+58);
dxl_write_word(22, P_GOAL_POSITION_L, 467-14);
espera_mov();
//dxl_write_word(13, P_GOAL_POSITION_L, 498-65);
//dxl_write_word(11, P_GOAL_POSITION_L, 585+25);
//espera_mov();

//-------- abaixa perna esq --------------------------------------------------
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.5);
dxl_write_word(22, P_GOAL_POSITION_L, 467-7);
//dxl_write_word(11, MOVING_SPEED, 110);

dxl_write_word(16, 24, 0);
dxl_write_word(15, 24, 0);
dxl_write_word(13, P_GOAL_POSITION_L, 498-60);
dxl_write_word(12, P_GOAL_POSITION_L, 512-105);//512-37
dxl_write_word(14, P_GOAL_POSITION_L, 510+60);//510-2
//dxl_write_word(17, P_GOAL_POSITION_L, 601);
espera_mov();
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-6);
dxl_write_word(22, P_GOAL_POSITION_L, 467-10);
espera_mov();
dxl_write_word(16, 24, 1);
dxl_write_word(15, 24, 1);
//dxl_write_word(15, P_GOAL_POSITION_L, 498);
//dxl_write_word(16, P_GOAL_POSITION_L, 517+10);
espera_mov();

//-----conf velocidade de 40 para voltar a posição ereto ------------------------
for(int i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//----volta a posição ereto -------------------------  --------------------------
///dxl_write_word(19, P_GOAL_POSITION_L, 512);
//dxl_write_word(13, P_GOAL_POSITION_L, 498);
//dxl_write_word(21, P_GOAL_POSITION_L, 508);
//dxl_write_word(15, P_GOAL_POSITION_L, 498);
//dxl_write_word(12, P_GOAL_POSITION_L, 512-54);
//dxl_write_word(14, P_GOAL_POSITION_L, 510+10);
//dxl_write_word(16, P_GOAL_POSITION_L, 547);

//----volta a posição ereto -----------------------------------------------------
dxl_write_word(11, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(11, P_GOAL_POSITION_L, 585+10);
dxl_write_word(12, P_GOAL_POSITION_L, 512-37);
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(14, P_GOAL_POSITION_L, 510);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(16, P_GOAL_POSITION_L, 517+10);
dxl_write_word(17, P_GOAL_POSITION_L, 601);
dxl_write_word(18, P_GOAL_POSITION_L, 512);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(20, P_GOAL_POSITION_L, 510);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(22, P_GOAL_POSITION_L, 467-6);


espera_mov();
a = 0;
} // if
} //for

//------ inclina robo para o lado direito ------------------------------------
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
//dxl_write_word(11, P_GOAL_POSITION_L, 607);
dxl_write_word(12, P_GOAL_POSITION_L, 475);
dxl_write_word(14, P_GOAL_POSITION_L, 499);
dxl_write_word(16, P_GOAL_POSITION_L, 535);
dxl_write_word(17, P_GOAL_POSITION_L, 590+15);
dxl_write_word(18, P_GOAL_POSITION_L, 516);
dxl_write_word(20, P_GOAL_POSITION_L, 513);
dxl_write_word(22, P_GOAL_POSITION_L, 469);
espera_mov();
dxl_write_word(13, P_GOAL_POSITION_L, 498+inclina);
dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina+17);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+18);
dxl_write_word(21, P_GOAL_POSITION_L, 508+inclina);
dxl_write_word(18, P_GOAL_POSITION_L, 512+20);
dxl_write_word(20, P_GOAL_POSITION_L, 506+15);
dxl_write_word(22, P_GOAL_POSITION_L, 467-15);
espera_mov();

//levanta_perna_esq(){
//--------------conf velocidade 40------------------------------------------------
for(int i = 3;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//------------- conf velocidade p/ levantar perna esq ---------------------------
dxl_write_word(20, MOVING_SPEED, 3*VEL_ANDAR);
dxl_write_word(18, MOVING_SPEED, 3*VEL_ANDAR*0.4);
dxl_write_word(22, MOVING_SPEED, 3*VEL_ANDAR*0.66);
dxl_write_word(15, MOVING_SPEED, 3*VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, 3*VEL_ANDAR*1.5);
//------------- levanta perna esq -----------------------------------------------
dxl_write_word(18, P_GOAL_POSITION_L, 512+60);
dxl_write_word(20, P_GOAL_POSITION_L, 510-50);
dxl_write_word(22, P_GOAL_POSITION_L, 467-45);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+22);//corrigi inclinação do servo 25
dxl_write_word(16, P_GOAL_POSITION_L, 562-23);
espera_mov();

//-----movimenta perna para frente --------------------------------------------
dxl_write_word(12, MOVING_SPEED, 3*VEL_ANDAR*0.872);
dxl_write_word(15, MOVING_SPEED, 3*VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, 3*VEL_ANDAR);
dxl_write_word(17, MOVING_SPEED, 3*VEL_ANDAR*0.345);
dxl_write_word(18, MOVING_SPEED, 3*VEL_ANDAR*0.109);
dxl_write_word(19, MOVING_SPEED, 3*VEL_ANDAR*0.4);
dxl_write_word(20, MOVING_SPEED, 3*VEL_ANDAR*0.364);
dxl_write_word(22, MOVING_SPEED, 3*VEL_ANDAR*0.6);
//dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.139);

dxl_write_word(20, P_GOAL_POSITION_L, 510-5);
dxl_write_word(18, P_GOAL_POSITION_L, 512+65);
dxl_write_word(22, P_GOAL_POSITION_L, 467+5);//27
//dxl_write_word(11, P_GOAL_POSITION_L, 585);
dxl_write_word(17, P_GOAL_POSITION_L, 601-15);
//dxl_write_word(19, P_GOAL_POSITION_L, 512+50);
//dxl_write_word(12, P_GOAL_POSITION_L, 512+11);
//dxl_write_word(16, P_GOAL_POSITION_L, 562-16);
//dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+12);
//dxl_write_word(19, P_GOAL_POSITION_L, 512+71);
espera_mov();
//dxl_write_word(16, P_GOAL_POSITION_L, 562-13);

//-------- abaixa perna esq --------------------------------------------------
dxl_write_word(20, MOVING_SPEED, 2*VEL_ANDAR);
dxl_write_word(18, MOVING_SPEED, 2*VEL_ANDAR*0.4);
dxl_write_word(22, MOVING_SPEED, 2*VEL_ANDAR*0.5);
//dxl_write_word(19, MOVING_SPEED, 110);
//dxl_write_word(17, MOVING_SPEED, 110);

dxl_write_word(22, 24, 0);
dxl_write_word(21, 24, 0);
dxl_write_word(18, P_GOAL_POSITION_L, 512+52);
dxl_write_word(20, P_GOAL_POSITION_L, 510);
dxl_write_word(11, P_GOAL_POSITION_L, 585);
dxl_write_word(17, P_GOAL_POSITION_L, 601-13);
espera_mov();

dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina);
//dxl_write_word(16, P_GOAL_POSITION_L, 562-0);
espera_mov();
dxl_write_word(22, 24, 1);
dxl_write_word(21, 24, 1);
dxl_write_word(21, P_GOAL_POSITION_L, 508+55);
dxl_write_word(22, P_GOAL_POSITION_L, 467+5);
espera_mov();
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.480);
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.714);
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR*0.143);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.299);
dxl_write_word(17, MOVING_SPEED, VEL_ANDAR*0.169);
dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.675);
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.935);
//dxl_write_word(20, MOVING_SPEED, VEL_ANDAR*0);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.714);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.065);

    for(int i = 3;i<23;i++)
    {
       dxl_write_word(i, MOVING_SPEED, 100);
       dxl_write_word(i, P_GOAL_POSITION_L, StandupPos_corrigido[i-1]);
    }
espera_mov();

} // função




int main()
{

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);

int i;
int baudnum = DEFAULT_BAUDNUM; //velocidade de transmissao
int deviceIndex = 1; //endereça USB
int Moving, PresentPos;
int CommStatus;

system("echo | sudo chmod 777 /dev/ttyUSB*");//libera USB

printf( "\nMilton Robot running...\n\n" );

using_shared_memory();
// ---- Open USB2Dynamixel -----------------------------------------------{
if( dxl_initialize(deviceIndex, baudnum) == 0 )
{
	printf( "Failed to open USB2Dynamixel!\n" );
	printf( "Press Enter key to terminate...\n" );
	getchar();
	return 0;
}
else
	printf( "Succeed to open USB2Dynamixel!\n" );
//-----------------------------------------------------------------------------}


while(1)
{

robo_ereto_corrigido();
sleep(1);
switch (DECISION_ACTION_A){
case 0:
robo_ereto_corrigido();
break;
case 1:
virar_dir(1);
break;
case 2:
andar_rapido(1);
break;
case 3:
andar_curto(1);
break;
}
}
	


//usleep(50);
//CONTROL_HEIGHT_A = 100;
//CONTROL_HEIGHT_B = 200;
//CONTROL_HEIGHT_C = 300;

    return 0;
}
