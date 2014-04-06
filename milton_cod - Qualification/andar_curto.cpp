#include <dynamixel.h>

#define VEL 200 // Define a velocidade
#define VEL_ANDAR 200 //define velocidade para andar

extern int inclina;

void andar_curto(int passos)
{

//int braco = 300;
//int a = 2;
int ctrl;

espera_mov();
//-------conf veloc 40 ---------------------------------------------------------
for(int i = 1;i<23;i++)
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
for(int i = 1;i<23;i++)
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
for(int i = 1;i<23;i++)
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
