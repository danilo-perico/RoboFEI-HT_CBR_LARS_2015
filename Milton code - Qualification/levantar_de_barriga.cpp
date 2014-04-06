#include <dynamixel.h>

//================================================================================
//---- Função que faz o robô levantar-se se estiver no chão  ---------------------
//---- com a barriga para cima e as costas no chão             ---------------------
void levantar_de_barriga(){
    for(int i = 1;i<23;i++)
    dxl_write_word(i, MOVING_SPEED, 500);

    dxl_write_word(8, P_GOAL_POSITION_L, 400);
    dxl_write_word(7, P_GOAL_POSITION_L, 150);
    dxl_write_word(5, P_GOAL_POSITION_L, 930);
    dxl_write_word(4, P_GOAL_POSITION_L, 222);
    espera_mov();
    dxl_write_word(5, P_GOAL_POSITION_L, 1014);
    dxl_write_word(4, P_GOAL_POSITION_L, 140);
    dxl_write_word(9, P_GOAL_POSITION_L, 775);
    dxl_write_word(18, P_GOAL_POSITION_L, 650);//773
    dxl_write_word(12, P_GOAL_POSITION_L, 650);//702
    espera_mov();
    dxl_write_word(7, P_GOAL_POSITION_L, 650);
    dxl_write_word(9, P_GOAL_POSITION_L, 600);
    espera_mov();
    dxl_write_word(18, P_GOAL_POSITION_L, 451);
    dxl_write_word(12, P_GOAL_POSITION_L, 600);
    dxl_write_word(8, P_GOAL_POSITION_L, 694);
    dxl_write_word(4, MOVING_SPEED, 700);
    dxl_write_word(5, MOVING_SPEED, 700);
    espera_mov();
    dxl_write_word(4, P_GOAL_POSITION_L, 521);
    espera_mov();
    dxl_write_word(5, P_GOAL_POSITION_L, 293);
    dxl_write_word(9, P_GOAL_POSITION_L, 573);
    espera_mov();
    dxl_write_word(4, MOVING_SPEED, 1000);
    dxl_write_word(5, MOVING_SPEED, 1000);
    dxl_write_word(5, P_GOAL_POSITION_L, 409);
    dxl_write_word(4, P_GOAL_POSITION_L, 304);

    levantar_de_costas();
}
//================================================================================
