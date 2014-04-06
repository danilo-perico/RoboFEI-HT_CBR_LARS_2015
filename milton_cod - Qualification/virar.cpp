#include <dynamixel.h>
#include <math.h>
#include <cmath>
#include <unistd.h>

#define VEL 200 // Define a velocidade
#define VEL_ANDAR 400 //define velocidade para andar
#define VEL_ANDAR2 80

void virar(int passos)
{
for(int w=0; w<passos; w++)
{
//int braco = 300;
int a = 2;
int i;
int ctrl;


espera_mov();
//-------conf veloc 40 ---------------------------------------------------------
for(i = 1;i<23;i++)
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

for(i = 1;i<23;i++)
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
for(i = 1;i<23;i++)
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

for(i = 1;i<23;i++)
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
