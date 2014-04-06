#include <dynamixel.h>
#include <stdio.h>

#define VEL 200

//---- Função que levanta o robô que esteja no chão  -----------------------------
//---- com as costas para cima e peito no chão       -----------------------------
void levantar_de_costas_fitPC(){
printf("Levantando\n");
    for(int i = 1;i<23;i++)
    	dxl_write_word(i, MOVING_SPEED, 700);
	dxl_write_word(14, P_GOAL_POSITION_L, 850);
	dxl_write_word(20, P_GOAL_POSITION_L, 171);
	dxl_write_word(4, P_GOAL_POSITION_L, 854);
	dxl_write_word(7, P_GOAL_POSITION_L, 150);
	dxl_write_word(3, P_GOAL_POSITION_L, 268);
	dxl_write_word(6, P_GOAL_POSITION_L, 750);
	dxl_write_word(16, P_GOAL_POSITION_L, 722);
	dxl_write_word(22, P_GOAL_POSITION_L, 247);
	dxl_write_word(5, P_GOAL_POSITION_L, 0);
	dxl_write_word(8, P_GOAL_POSITION_L, 1023);

	espera_mov();

	dxl_write_word(12, P_GOAL_POSITION_L, 232);
	dxl_write_word(18, P_GOAL_POSITION_L, 795);

	espera_mov();
	dxl_write_word(12, P_GOAL_POSITION_L, 159);
	dxl_write_word(18, P_GOAL_POSITION_L, 870);
	dxl_write_word(14, P_GOAL_POSITION_L, 884);
	dxl_write_word(20, P_GOAL_POSITION_L, 152);

	espera_mov();
	dxl_write_word(6, MOVING_SPEED, 1000);
	dxl_write_word(6, P_GOAL_POSITION_L, 432);
	dxl_write_word(3, MOVING_SPEED, 1000);
	dxl_write_word(3, P_GOAL_POSITION_L, 583);
//dxl_write_word(12, P_GOAL_POSITION_L, 100);
//dxl_write_word(18, P_GOAL_POSITION_L, 929);

	espera_mov();
	dxl_write_word(10, MOVING_SPEED, 300);
	dxl_write_word(10, P_GOAL_POSITION_L, 710);
	dxl_write_word(22, P_GOAL_POSITION_L, 416);
	dxl_write_word(16, P_GOAL_POSITION_L, 631);

	for(int i = 1;i<23;i++)
    	dxl_write_word(i, MOVING_SPEED, 100);

	espera_mov();
	dxl_write_word(16, MOVING_SPEED, 50);
	dxl_write_word(12, MOVING_SPEED, 50);
	dxl_write_word(18, MOVING_SPEED, 50);
	dxl_write_word(22, MOVING_SPEED, 50);
	dxl_write_word(12, P_GOAL_POSITION_L, 159);
	dxl_write_word(18, P_GOAL_POSITION_L, 870);
	dxl_write_word(22, P_GOAL_POSITION_L, 461);
	dxl_write_word(16, P_GOAL_POSITION_L, 559);
	dxl_write_word(20, P_GOAL_POSITION_L, 127);
	dxl_write_word(14, P_GOAL_POSITION_L, 908);

	espera_mov();
	dxl_write_word(20, P_GOAL_POSITION_L, 157);
	dxl_write_word(14, P_GOAL_POSITION_L, 878);
	dxl_write_word(12, P_GOAL_POSITION_L, 189);
	dxl_write_word(18, P_GOAL_POSITION_L, 840);

	//espera_mov();
	dxl_write_word(5, MOVING_SPEED, 200);
	dxl_write_word(8, MOVING_SPEED, 200);
	dxl_write_word(4, MOVING_SPEED, 200);
	dxl_write_word(7, MOVING_SPEED, 200);
	dxl_write_word(5, P_GOAL_POSITION_L, 480);
	dxl_write_word(8, P_GOAL_POSITION_L, 542);
	dxl_write_word(4, P_GOAL_POSITION_L, 304);
	dxl_write_word(7, P_GOAL_POSITION_L, 674);

	espera_mov();

	dxl_write_word(16, MOVING_SPEED, 20);
	dxl_write_word(22, MOVING_SPEED, 20);
	dxl_write_word(20, P_GOAL_POSITION_L, 257);
	dxl_write_word(14, P_GOAL_POSITION_L, 778);
	dxl_write_word(12, P_GOAL_POSITION_L, 289);
	dxl_write_word(18, P_GOAL_POSITION_L, 740);
	dxl_write_word(16, P_GOAL_POSITION_L, 609);
	dxl_write_word(22, P_GOAL_POSITION_L, 411);

	espera_mov();
	dxl_write_word(18, MOVING_SPEED, 100);//50
	dxl_write_word(12, MOVING_SPEED, 100);//50
	dxl_write_word(16, MOVING_SPEED, 40);//20
	dxl_write_word(22, MOVING_SPEED, 40);//20
	dxl_write_word(16, P_GOAL_POSITION_L, 740);
	dxl_write_word(22, P_GOAL_POSITION_L, 295);
	dxl_write_word(18, P_GOAL_POSITION_L, 457);
	dxl_write_word(12, P_GOAL_POSITION_L, 573);

	//usleep(5000000);

	espera_mov();
	dxl_write_word(14, MOVING_SPEED, VEL*0.24);
	dxl_write_word(20, MOVING_SPEED, VEL*0.24);
	dxl_write_word(12, MOVING_SPEED, VEL*0.48);
	dxl_write_word(18, MOVING_SPEED, VEL*0.48);
	dxl_write_word(10, MOVING_SPEED, VEL*0.55);
	dxl_write_word(14, P_GOAL_POSITION_L, 748);
	dxl_write_word(20, P_GOAL_POSITION_L, 287);
	dxl_write_word(18, P_GOAL_POSITION_L, 517);
	dxl_write_word(12, P_GOAL_POSITION_L, 513);
	dxl_write_word(10, P_GOAL_POSITION_L, 835);

	//usleep(5000000);

	espera_mov();
	dxl_write_word(14, MOVING_SPEED, VEL*1.1);
	dxl_write_word(20, MOVING_SPEED, VEL*1.1);
	dxl_write_word(16, MOVING_SPEED, VEL*0.804);
	dxl_write_word(22, MOVING_SPEED, VEL*0.745);
	dxl_write_word(12, MOVING_SPEED, VEL*0.087);
	dxl_write_word(18, MOVING_SPEED, VEL*0.033);

	for(int i = 1;i<23;i++)
    	    dxl_write_word(i, P_GOAL_POSITION_L, StandupPos_corrigido[i-1]);

}
//================================================================================
