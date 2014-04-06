#include <dynamixel.h>

//================================================================================
void levanta_abaixa_perna_dir(){
espera_mov();
//--------------conf velocidade 40------------------------------------------------
for(int i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, 40);
//------conf veloc para os braços ---------------------------------------------
dxl_write_word(4, MOVING_SPEED, 100);
dxl_write_word(7, MOVING_SPEED, 100);
//------------- conf velocidade p/ levantar perna dir ---------------------------
dxl_write_word(14, MOVING_SPEED, 100);
dxl_write_word(12, MOVING_SPEED, 100*0.4);
dxl_write_word(16, MOVING_SPEED, 100*0.66);
dxl_write_word(21, MOVING_SPEED, 200);
dxl_write_word(22, MOVING_SPEED, 100);
//------------- levanta perna dir -----------------------------------------------
dxl_write_word(12, P_GOAL_POSITION_L, 512-55);//115
dxl_write_word(14, P_GOAL_POSITION_L, 510+150);//300
dxl_write_word(16, P_GOAL_POSITION_L, 562+95);//190
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-18);//corrigi inclinação do servo 21
dxl_write_word(22, P_GOAL_POSITION_L, 467+2);
dxl_write_word(4, P_GOAL_POSITION_L, 274);
espera_mov();
//------------conf velocidade para abaixar a perna dir --------------------------
dxl_write_word(14, MOVING_SPEED, 100);
dxl_write_word(12, MOVING_SPEED, 100*0.4);
dxl_write_word(16, MOVING_SPEED, 100*0.5);
dxl_write_word(13, MOVING_SPEED, 100);
//------------- abaixa perna dir -------------------------------------------------
dxl_write_word(12, P_GOAL_POSITION_L, 512-4);
dxl_write_word(14, P_GOAL_POSITION_L, 510+22);
dxl_write_word(13, P_GOAL_POSITION_L, 498-inclina-30);//corrigi inclinação do servo 13 para voltar na posição correta
dxl_write_word(16, 24, 0); //desliga torque do servo 16 para evitar tranco
espera_mov();
//-------volta servos na posição inicial de inclinação depois de pé no chão -----
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina);
dxl_write_word(22, P_GOAL_POSITION_L, 467);
dxl_write_word(16, 24, 1);//liga torque do 16 para travar pé esq
dxl_write_word(13, P_GOAL_POSITION_L, 498-inclina);
espera_mov();
//-----conf velocidade de 40 para voltar a posição ereto ------------------------
for(int i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, 40);
//----volta a posição ereto -------------------------  --------------------------
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(12, P_GOAL_POSITION_L, 512);
dxl_write_word(14, P_GOAL_POSITION_L, 510);
dxl_write_word(16, P_GOAL_POSITION_L, 562-2);//corrigi desvio temporário de postura
dxl_write_word(22, P_GOAL_POSITION_L, 467+2);//corrigi desvio temporário de postura
espera_mov();
dxl_write_word(16, P_GOAL_POSITION_L, 562);//volta a posição inicial ereto
dxl_write_word(22, P_GOAL_POSITION_L, 467);//volta a posição inicial ereto */
}

