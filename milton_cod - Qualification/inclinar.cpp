#include <dynamixel.h>
#include <unistd.h>

void inclinar(){
int braco = 300;
int a = 0;
int ctrl;

espera_mov();
//-------conf veloc 40 ---------------------------------------------------------
for(int i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, 40);

//for (ctrl = 0; ctrl<3; ctrl++){
//------ inclina robo para o lado esquerdo ------------------------------------
if (a == 0){
//------conf veloc para os braços ---------------------------------------------
dxl_write_word(4, MOVING_SPEED, 200);
dxl_write_word(7, MOVING_SPEED, 200);
//------ inclina robo para o lado esquerdo ------------------------------------
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
espera_mov();
dxl_write_word(13, P_GOAL_POSITION_L, 498-inclina);
dxl_write_word(19, P_GOAL_POSITION_L, 512-inclina);
dxl_write_word(15, P_GOAL_POSITION_L, 498-inclina);
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina);
dxl_write_word(4, P_GOAL_POSITION_L, 274+braco);
dxl_write_word(7, P_GOAL_POSITION_L, 675);
espera_mov();
usleep(500000);
a=1;
}
//------ inclina robo para o lado direito ------------------------------------
if (a == 1){
//------conf veloc para os braços ---------------------------------------------
dxl_write_word(4, MOVING_SPEED, 200);
dxl_write_word(7, MOVING_SPEED, 200);
//------ inclina robo para o lado direito ------------------------------------
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
espera_mov();
dxl_write_word(13, P_GOAL_POSITION_L, 498+inclina);
dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina);
dxl_write_word(21, P_GOAL_POSITION_L, 508+inclina);
dxl_write_word(4, P_GOAL_POSITION_L, 274);
dxl_write_word(7, P_GOAL_POSITION_L, 675-braco);
espera_mov();
usleep(500000);
a=0;
}
}

