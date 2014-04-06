#include <dynamixel.h>
#include <stdio.h>

void chutepedireito_fitPC()
{
    printf("Chutando com a perna direita\n");
    for(int i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, 25);

    //--------------inclina esquerda--------------

    dxl_write_word(13, P_GOAL_POSITION_L, 454);
    dxl_write_word(19, P_GOAL_POSITION_L, 467);

    dxl_write_word(21, P_GOAL_POSITION_L, 446);
    dxl_write_word(15, P_GOAL_POSITION_L, 426);

    dxl_write_word(3, MOVING_SPEED, 50);
    dxl_write_word(3, P_GOAL_POSITION_L, 456);
    dxl_write_word(4, MOVING_SPEED, 50);
    dxl_write_word(4, P_GOAL_POSITION_L, 469);
    dxl_write_word(5, MOVING_SPEED, 50);
    dxl_write_word(5, P_GOAL_POSITION_L, 572);
    //-------------------------------------------

    //-----------tira pe direito do chao---------
    for(int i = 1;i<23;i++)
        dxl_write_word(i, MOVING_SPEED, 60);

    espera_mov();
    dxl_write_word(16, P_GOAL_POSITION_L, 646);
    dxl_write_word(14, P_GOAL_POSITION_L, 600);
    dxl_write_word(21, P_GOAL_POSITION_L, 439);
    dxl_write_word(22, P_GOAL_POSITION_L, 459);

    //-------------------------------------------

    for(int i = 1;i<23;i++)
        dxl_write_word(i, MOVING_SPEED, 100);
    //--------levanta o pe direito para chute----
    espera_mov();
    dxl_write_word(16, P_GOAL_POSITION_L, 678);
    dxl_write_word(14, P_GOAL_POSITION_L, 636);
    dxl_write_word(12, P_GOAL_POSITION_L, 472);
    dxl_write_word(22, P_GOAL_POSITION_L, 461);
    //-------------------------------------------

    //-------corrigi posicao pe direito----------
    espera_mov();
    //dxl_write_word(16, P_GOAL_POSITION_L, 640);
     //dxl_write_word(8, MOVING_SPEED, 100);
     dxl_write_word(8, P_GOAL_POSITION_L, 700);
    //-------------------------------------------


    //----------chuta com pe direito-----------
    espera_mov();
    for(int i = 1;i<23;i++)
        dxl_write_word(i, MOVING_SPEED, 200);
    dxl_write_word(12, P_GOAL_POSITION_L, 375);
    dxl_write_word(16, P_GOAL_POSITION_L, 530);
    dxl_write_word(18, P_GOAL_POSITION_L, 493);
    dxl_write_word(22, P_GOAL_POSITION_L, 456);
    dxl_write_word(8, P_GOAL_POSITION_L, 750);
    dxl_write_word(5, MOVING_SPEED, 300);
    dxl_write_word(5, P_GOAL_POSITION_L, 350);
    dxl_write_word(4, P_GOAL_POSITION_L, 550);
    //------------------------------------------

    //--------volta a perna direita--------------
    espera_mov();
    dxl_write_word(16, P_GOAL_POSITION_L, 678);
    dxl_write_word(14, P_GOAL_POSITION_L, 636);
    dxl_write_word(12, P_GOAL_POSITION_L, 472);
    dxl_write_word(18, P_GOAL_POSITION_L, 509);
    //dxl_write_word(8, P_GOAL_POSITION_L, 609);
    dxl_write_word(5, MOVING_SPEED, 200);
    dxl_write_word(5, P_GOAL_POSITION_L, 572);
    dxl_write_word(13, P_GOAL_POSITION_L, 446);
    //dxl_write_word(22, P_GOAL_POSITION_L, 461);
    //------------------------------------------

    //--------volta braço esquerdo--------------
    espera_mov();
    dxl_write_word(8, MOVING_SPEED, 100);
    dxl_write_word(8, P_GOAL_POSITION_L, 650);
    //-----------------------------------------

    //-------corrigi perna direita------------
    espera_mov();
    dxl_write_word(16, P_GOAL_POSITION_L, 600);
    dxl_write_word(14, P_GOAL_POSITION_L, 600);
    dxl_write_word(21, P_GOAL_POSITION_L, 439);
    //-----------------------------------------
    espera_mov();

    //-----volta perna direita----------------
int Pos[22] = {304, 594, 456, 469, 572, 546, 674, 609, 573, 835, 613, 534, 430, 508, 426, 559, 590, 509, 467, 506, 444, 461};
//int i;
for(int i = 1;i<23;i++){
    dxl_write_word(i, MOVING_SPEED, 35);
    dxl_write_word(i, P_GOAL_POSITION_L, Pos[i-1]);
}

//-------volta posicao inicial-----------
   espera_mov();
   robo_ereto();
//---------------------------------------
}
