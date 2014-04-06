#include <dynamixel.h>
#include <stdio.h>

void chutepedireito()
{
    printf("Chutando com a perna direita\n");
    for(int i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, 25);

    //--------------inclina esquerda--------------

    dxl_write_word(13, P_GOAL_POSITION_L, 454);
    dxl_write_word(19, P_GOAL_POSITION_L, 467);

    dxl_write_word(21, P_GOAL_POSITION_L, 446);
    dxl_write_word(15, P_GOAL_POSITION_L, 426);

    dxl_write_word(3, MOVING_SPEED, 450);
    dxl_write_word(3, P_GOAL_POSITION_L, 456);
    dxl_write_word(4, MOVING_SPEED, 690);
    dxl_write_word(4, P_GOAL_POSITION_L, 469);
    dxl_write_word(5, MOVING_SPEED, 450);
    dxl_write_word(5, P_GOAL_POSITION_L, 572);
    //-------------------------------------------

    //-----------tira pe direito do chao---------
    for(int i = 1;i<23;i++)
        dxl_write_word(i, MOVING_SPEED, 60);

    espera_mov();
    dxl_write_word(16, P_GOAL_POSITION_L, 646);
    dxl_write_word(14, P_GOAL_POSITION_L, 600);
    dxl_write_word(21, P_GOAL_POSITION_L, 439);
    dxl_write_word(22, P_GOAL_POSITION_L, 459+10);

    //-------------------------------------------

    for(int i = 1;i<23;i++)
        dxl_write_word(i, MOVING_SPEED, 300);
    //--------levanta o pe direito para chute----
    espera_mov();
    dxl_write_word(16, P_GOAL_POSITION_L, 678-15);
    dxl_write_word(14, P_GOAL_POSITION_L, 636);
    dxl_write_word(12, P_GOAL_POSITION_L, 472);
    dxl_write_word(22, P_GOAL_POSITION_L, 459+10);
    //-------------------------------------------

    //-------corrigi posicao pe direito----------
    espera_mov();
    //dxl_write_word(16, P_GOAL_POSITION_L, 640);
     //dxl_write_word(8, MOVING_SPEED, 100);
    // dxl_write_word(8, P_GOAL_POSITION_L, 700);
    //-------------------------------------------


    //----------chuta com pe direito-----------
    espera_mov();
    for(int i = 1;i<23;i++)
        dxl_write_word(i, MOVING_SPEED, 1000);
    dxl_write_word(12, P_GOAL_POSITION_L, 375);
    dxl_write_word(11, P_GOAL_POSITION_L, 585+15);
    dxl_write_word(16, P_GOAL_POSITION_L, 530+40);
    dxl_write_word(18, P_GOAL_POSITION_L, 493);
    dxl_write_word(22, P_GOAL_POSITION_L, 459+10);
    dxl_write_word(8, P_GOAL_POSITION_L, 750);
    dxl_write_word(5, MOVING_SPEED, 300);
    dxl_write_word(5, P_GOAL_POSITION_L, 350);
    dxl_write_word(4, P_GOAL_POSITION_L, 550);
    //------------------------------------------

    //--------volta a perna direita--------------
     espera_mov();
    for(int i = 1;i<23;i++)
        dxl_write_word(i, MOVING_SPEED, 800);

    dxl_write_word(16, P_GOAL_POSITION_L, 530+120);
    dxl_write_word(14, P_GOAL_POSITION_L, 636);
    dxl_write_word(12, P_GOAL_POSITION_L, 472);
    dxl_write_word(18, P_GOAL_POSITION_L, 509);
    //dxl_write_word(8, P_GOAL_POSITION_L, 609);
    dxl_write_word(5, MOVING_SPEED, 200);
    dxl_write_word(5, P_GOAL_POSITION_L, 572);
    dxl_write_word(13, P_GOAL_POSITION_L, 446);
    dxl_write_word(22, P_GOAL_POSITION_L, 467);
    //------------------------------------------

    //--------volta braço esquerdo--------------
  //  espera_mov();
  //  dxl_write_word(8, MOVING_SPEED, 100);
  //  dxl_write_word(8, P_GOAL_POSITION_L, 650);
    //-----------------------------------------

    //-------corrigi perna direita------------
    espera_mov();
    dxl_write_word(16, P_GOAL_POSITION_L, 600);
    dxl_write_word(14, P_GOAL_POSITION_L, 600);
    dxl_write_word(21, P_GOAL_POSITION_L, 439);
    //-----------------------------------------
    espera_mov();
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
dxl_write_word(22, P_GOAL_POSITION_L, 467);//volta a posição inicial ereto

//-------volta posicao inicial-----------
   espera_mov();
   robo_ereto_corrigido();
//---------------------------------------

}
