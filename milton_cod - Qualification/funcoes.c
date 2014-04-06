#include <dynamixel.h>

#define VEL 200 // Define a velocidade
#define VEL_ANDAR 200 //define velocidade para andar

int inclina = 55;

void delay_mov(unsigned int delta_posicao, unsigned int velocidade);
//------ Posição dos servos para o robo em pé --------------------
const unsigned int StandupPos[22] = {304, 594, //01 , 02, (cabeça)
                                     478, 304, 409, //03, 04, 05, (braço direito)
                                     546, 650, 609, //06, 07, 08, (braço esquerdo)
                                     573, 835, //09, 10, (torso)
                                     613, 534, 501, 508, 492, 559, //11, 12, 13, 14, 15, 16 (perna direita)
                                     590, 509, 514, 506, 510, 461}; //17, 18, 19, 20, 21, 22 (perna esquerda)
//----------------------------------------------------------------
const unsigned int StandupPos_corrigido[22] = {304, 594, //01 , 02, (cabeça)
                                                478, 274, 350, //03, 04, 05, (braço direito)
                                                541, 675, 682, //06, 07, 08, (braço esquerdo)
                                                571, 835, //09, 10, (torso)
                                                585, 512, 498, 510, 498, 562, //11, 12, 13, 14, 15, 16 (perna direita)
                                                601, 512, 512, 510, 508, 467}; //17, 18, 19, 20, 21, 22 (perna esquerda)

int i;

//================================================================================
//---- Função que verifica se algum servo está em movimento exceto a cabeça-------
void espera_mov()
{
    for(i=3;i<23;i++)
    {
       do
       {
        usleep(2000); //Espera 2 milisegundos evitando uso excessivo do processador
       }while(dxl_read_byte( i, P_MOVING ) != 0);
    }
}
//================================================================================

/*void espera_mov(){
    int Moving;
    for(i=3;i<23;i++){
    do {
     usleep(2000);
        Moving = dxl_read_byte( i, P_MOVING );
        } while(Moving != 0);
    }
}*/

//================================================================================
//---- Posiciona o robô na posição em pé------------------------------------------
void robo_ereto()
{
    for(i = 1;i<23;i++)
    {
       dxl_write_word(i, MOVING_SPEED, 100);
       dxl_write_word(i, P_GOAL_POSITION_L, StandupPos[i-1]);
    }
}
//================================================================================
void robo_ereto_corrigido()
{
    for(i = 1;i<23;i++)
    {
       dxl_write_word(i, MOVING_SPEED, 100);
       dxl_write_word(i, P_GOAL_POSITION_L, StandupPos_corrigido[i-1]);
    }
}

//================================================================================
//---- Chuta com o pé direito ----------------------------------------------------
void chutar_reto_direita()
{
    printf("Chutando com a perna direita\n");
    for(i = 1;i<23;i++)
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
    for(i = 1;i<23;i++)
        dxl_write_word(i, MOVING_SPEED, 60);

    espera_mov();
    dxl_write_word(16, P_GOAL_POSITION_L, 646);
    dxl_write_word(14, P_GOAL_POSITION_L, 600);
    dxl_write_word(21, P_GOAL_POSITION_L, 439);
    dxl_write_word(22, P_GOAL_POSITION_L, 459);

    //-------------------------------------------

    for(i = 1;i<23;i++)
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
    for(i = 1;i<23;i++)
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
for(i = 1;i<23;i++){
    dxl_write_word(i, MOVING_SPEED, 35);
    dxl_write_word(i, P_GOAL_POSITION_L, Pos[i-1]);
}

//-------volta posicao inicial-----------
   espera_mov();
   robo_ereto();
//---------------------------------------
}
//================================================================================


//================================================================================
//---- Função que trava todos os servos ------------------------------------------
void travar_servos()
{
    for(i = 1;i<23;i++)
         dxl_write_word(i, 24, 1);
}
//================================================================================

//================================================================================
//---- Função que levanta o robô que esteja no chão  -----------------------------
//---- com as costas para cima e peito no chão       -----------------------------
void levantar(){
printf("Levantando\n");
    for(i = 1;i<23;i++)
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

	for(i = 1;i<23;i++)
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

	for(i = 1;i<23;i++)
    	    dxl_write_word(i, P_GOAL_POSITION_L, StandupPos[i-1]);

}
//================================================================================

//================================================================================
//---- Função que faz o robo caminhar --------------------------------------------
void andar_reto(){

dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.43);
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.53);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.6);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(13, P_GOAL_POSITION_L, 476);
dxl_write_word(19, P_GOAL_POSITION_L, 483);
dxl_write_word(15, P_GOAL_POSITION_L, 457);
dxl_write_word(21, P_GOAL_POSITION_L, 452);
//delay_mov(58,VEL_ANDAR);
espera_mov();
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.77);
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.3);
dxl_write_word(12, P_GOAL_POSITION_L, 480);
dxl_write_word(14, P_GOAL_POSITION_L, 578);
dxl_write_word(16, P_GOAL_POSITION_L, 580);
espera_mov();
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR*0.92);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.61 );
dxl_write_word(12, P_GOAL_POSITION_L, 444);
dxl_write_word(14, P_GOAL_POSITION_L, 545);
dxl_write_word(16, P_GOAL_POSITION_L, 518);
dxl_write_word(22, P_GOAL_POSITION_L, 470);
espera_mov();
dxl_write_word(21, P_GOAL_POSITION_L, 501);
dxl_write_word(15, P_GOAL_POSITION_L, 500);
dxl_write_word(16, P_GOAL_POSITION_L, 516);
dxl_write_word(22, P_GOAL_POSITION_L, 467);
dxl_write_word(19, P_GOAL_POSITION_L, 502);
dxl_write_word(13, P_GOAL_POSITION_L, 502);
dxl_write_word(12, P_GOAL_POSITION_L, 426);
dxl_write_word(18, P_GOAL_POSITION_L, 540);
dxl_write_word(9, P_GOAL_POSITION_L, 590);//605
dxl_write_word(8, P_GOAL_POSITION_L, 488);
espera_mov();
dxl_write_word(21, P_GOAL_POSITION_L, 539);
dxl_write_word(15, P_GOAL_POSITION_L, 559);
dxl_write_word(19, P_GOAL_POSITION_L, 534);
dxl_write_word(13, P_GOAL_POSITION_L, 545);
dxl_write_word(14, P_GOAL_POSITION_L, 500);
dxl_write_word(4, P_GOAL_POSITION_L, 230);
dxl_write_word(9, P_GOAL_POSITION_L, 540);//510
dxl_write_word(18, P_GOAL_POSITION_L, 547);
dxl_write_word(12, P_GOAL_POSITION_L, 461);
dxl_write_word(22, P_GOAL_POSITION_L, 460);


espera_mov();
dxl_write_word(12, P_GOAL_POSITION_L, 426);
dxl_write_word(15, P_GOAL_POSITION_L, 570);
dxl_write_word(16, P_GOAL_POSITION_L, 520);//531
espera_mov();
dxl_write_word(12, P_GOAL_POSITION_L, 481);
dxl_write_word(16, P_GOAL_POSITION_L, 531);//531
espera_mov();
//dxl_write_word(5, P_GOAL_POSITION_L, 554);
dxl_write_word(8, P_GOAL_POSITION_L, 635);
dxl_write_word(9, P_GOAL_POSITION_L, 580);
espera_mov();
//dxl_write_word(18, MOVING_SPEED, VEL_ANDAR);
//dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.22);
//dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.61 );
//dxl_write_word(20, MOVING_SPEED, VEL_ANDAR*0.51 );
//dxl_write_word(22, P_GOAL_POSITION_L, 512);
//dxl_write_word(15, P_GOAL_POSITION_L, 539);
//dxl_write_word(20, P_GOAL_POSITION_L, 532);
//dxl_write_word(18, P_GOAL_POSITION_L, 601);





//------inclinar esquerda---------------------
  /*  dxl_write_word(13, P_GOAL_POSITION_L, 456);
    dxl_write_word(15, P_GOAL_POSITION_L, 444);
    dxl_write_word(19, P_GOAL_POSITION_L, 463);
    dxl_write_word(21, P_GOAL_POSITION_L, 427);
    dxl_write_word(22, P_GOAL_POSITION_L, 453);
    dxl_write_word(18, P_GOAL_POSITION_L, 662);
    //dxl_write_word(21, P_GOAL_POSITION_L, 454);
    // espera_mov();
    //dxl_write_word(22, P_GOAL_POSITION_L, 453);
    // dxl_write_word(18, P_GOAL_POSITION_L, 662);
    espera_mov();

//------volta do inclinar esquerda---------------------
    dxl_write_word(22, P_GOAL_POSITION_L, 467);
    dxl_write_word(21, P_GOAL_POSITION_L, 454);
    dxl_write_word(18, P_GOAL_POSITION_L, 667);
    espera_mov();
    dxl_write_word(13, P_GOAL_POSITION_L, 487);
    dxl_write_word(15, P_GOAL_POSITION_L, 492);
    dxl_write_word(19, P_GOAL_POSITION_L, 510);
    dxl_write_word(21, P_GOAL_POSITION_L, 512);
    espera_mov();
//--------------------------------------------------------

//-----inclina direita------------------------------------
    dxl_write_word(13, P_GOAL_POSITION_L, 487+31);
   // dxl_write_word(15, P_GOAL_POSITION_L, 492+48);
    dxl_write_word(15, P_GOAL_POSITION_L, 587);
    dxl_write_word(19, P_GOAL_POSITION_L, 510+51);
    dxl_write_word(21, P_GOAL_POSITION_L, 512+56);
    espera_mov();
    dxl_write_word(13, P_GOAL_POSITION_L, 537);
    dxl_write_word(16, P_GOAL_POSITION_L, 588);


//------volta do inclinar direita---------------------
    dxl_write_word(16, P_GOAL_POSITION_L, 586);
    dxl_write_word(13, P_GOAL_POSITION_L, 487);
    dxl_write_word(15, P_GOAL_POSITION_L, 492);
    espera_mov();
  //  dxl_write_word(19, P_GOAL_POSITION_L, 510);
 //   dxl_write_word(21, P_GOAL_POSITION_L, 512);
 //   dxl_write_word(18, P_GOAL_POSITION_L, 667);
 //   espera_mov();


// const unsigned int StandupPos[22] = {304, 594, //01 , 02, (cabeça)
   //                                  478, 304, 409, //03, 04, 05, (braço direito)
   //                                  546, 650, 609, //06, 07, 08, (braço esquerdo)
   //                                  573, 835, //09, 10, (torso)
   //                                  613, 534, 501, 508, 492, 559, //11, 12, 13, 14, 15, 16 (perna direita)
   //                                  590, 509, 514, 506, 510, 461}; //17, 18, 19, 20, 21, 22 (perna esquerda)

*/
}
//================================================================================
void abaixa_robo(){
for(i = 1;i<23;i++)
dxl_write_word(i, MOVING_SPEED, 200);
//dxl_write_word(10, P_GOAL_POSITION_L, 1023);
    dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.75);
    dxl_write_word(16, MOVING_SPEED, VEL_ANDAR);
    dxl_write_word(11, P_GOAL_POSITION_L, 613);
    dxl_write_word(12, P_GOAL_POSITION_L, 358);
    dxl_write_word(13, P_GOAL_POSITION_L, 487);
    dxl_write_word(14, P_GOAL_POSITION_L, 640);
    dxl_write_word(15, P_GOAL_POSITION_L, 492);
    dxl_write_word(16, P_GOAL_POSITION_L, 586);

    dxl_write_word(22, MOVING_SPEED, VEL_ANDAR);
    dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.75);
    dxl_write_word(17, P_GOAL_POSITION_L, 588);
    dxl_write_word(18, P_GOAL_POSITION_L, 667);
    dxl_write_word(19, P_GOAL_POSITION_L, 510);
    dxl_write_word(20, P_GOAL_POSITION_L, 412);
    dxl_write_word(21, P_GOAL_POSITION_L, 512);
    dxl_write_word(22, P_GOAL_POSITION_L, 467);
    espera_mov();
//--------------------------------------------------
}

void andar(){

 for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR);

//-------levantar perna direita--------------------

dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.43);
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.53);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.6);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(13, P_GOAL_POSITION_L, 476);
dxl_write_word(19, P_GOAL_POSITION_L, 483);
dxl_write_word(15, P_GOAL_POSITION_L, 457);
dxl_write_word(21, P_GOAL_POSITION_L, 457);//453
espera_mov();
 for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.77);
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.3);
dxl_write_word(12, P_GOAL_POSITION_L, 480);
dxl_write_word(14, P_GOAL_POSITION_L, 578);
dxl_write_word(16, P_GOAL_POSITION_L, 580);
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(13, P_GOAL_POSITION_L, 473);
espera_mov();
 for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR);

dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.77);
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.3);
dxl_write_word(12, P_GOAL_POSITION_L, 534);
dxl_write_word(14, P_GOAL_POSITION_L, 508);
dxl_write_word(16, P_GOAL_POSITION_L, 559);
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*1.1);
dxl_write_word(13, P_GOAL_POSITION_L, 455);
espera_mov();
 for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.43);
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.53);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.6);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(13, P_GOAL_POSITION_L, 501);
dxl_write_word(19, P_GOAL_POSITION_L, 514);
dxl_write_word(15, P_GOAL_POSITION_L, 492);
dxl_write_word(21, P_GOAL_POSITION_L, 510);
espera_mov();
 for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR);

//-------levantar perna esquerda--------------------
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.63);//0.43
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.73);//0.53
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*1);//0.6
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*1.3);//1
dxl_write_word(19, P_GOAL_POSITION_L, 514+35);
dxl_write_word(13, P_GOAL_POSITION_L, 501+31);
dxl_write_word(21, P_GOAL_POSITION_L, 510+40);
dxl_write_word(15, P_GOAL_POSITION_L, 567);//564
dxl_write_word(16, P_GOAL_POSITION_L, 570);
espera_mov();
 for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR);

dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.17);
dxl_write_word(20, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.61);
dxl_write_word(18, P_GOAL_POSITION_L, 516);
dxl_write_word(20, P_GOAL_POSITION_L, 464);
dxl_write_word(22, P_GOAL_POSITION_L, 436);
espera_mov();
 for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR);

dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.17);
dxl_write_word(20, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.61);
dxl_write_word(18, P_GOAL_POSITION_L, 509);
dxl_write_word(20, P_GOAL_POSITION_L, 506);
dxl_write_word(22, P_GOAL_POSITION_L, 461);
dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*1.1);
dxl_write_word(19, P_GOAL_POSITION_L, 514+56);
espera_mov();
 for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR);

dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.43);
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.53);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.6);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(19, P_GOAL_POSITION_L, 514);
dxl_write_word(13, P_GOAL_POSITION_L, 501);
dxl_write_word(21, P_GOAL_POSITION_L, 510);
dxl_write_word(15, P_GOAL_POSITION_L, 492);
dxl_write_word(16, P_GOAL_POSITION_L, 559);
espera_mov();

// const unsigned int StandupPos[22] = {304, 594, //01 , 02, (cabeça)
   //                                  478, 304, 409, //03, 04, 05, (braço direito)
   //                                  546, 650, 609, //06, 07, 08, (braço esquerdo)
   //                                  573, 835, //09, 10, (torso)
   //                                  613, 534, 501, 508, 492, 559, //11, 12, 13, 14, 15, 16 (perna direita)
   //                                  590, 509, 514, 506, 510, 461}; //17, 18, 19, 20, 21, 22 (perna esquerda)

}

andar_matriz(){
int Pos12[9] = {516, 507, 476, 444, 431, 433, 459, 475, 477};
int Pos14[9] = {525,531,580,603,604,591,534,502,497};
int Pos16[9] = {568,572,582,578,562,553,525,505,502};


for(i=0;i<9;i++){
    dxl_write_word(12, P_GOAL_POSITION_L, Pos12[i]);
    dxl_write_word(14, P_GOAL_POSITION_L, Pos14[i]);
    dxl_write_word(16, P_GOAL_POSITION_L, Pos16[i]);
    usleep(100000);
}
}
//==============================================================================
inclinar(){
int braco = 300;
int a = 0;
int ctrl;

espera_mov();
//-------conf veloc 40 ---------------------------------------------------------
for(i = 1;i<23;i++)
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

levanta_e_abaixa_perna_dir();

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

levanta_e_abaixa_perna_esq();

usleep(500000);
a=0;
}
}

//================================================================================
levanta_e_abaixa_perna_esq(){
espera_mov();
//--------------conf velocidade 40------------------------------------------------
for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, 40);
//------conf veloc para os braços ---------------------------------------------
dxl_write_word(4, MOVING_SPEED, 100);
dxl_write_word(7, MOVING_SPEED, 100);
//------------- conf velocidade p/ levantar perna esq ---------------------------
dxl_write_word(20, MOVING_SPEED, 100);
dxl_write_word(18, MOVING_SPEED, 100*0.4);
dxl_write_word(22, MOVING_SPEED, 100*0.66);
dxl_write_word(15, MOVING_SPEED, 100);
dxl_write_word(16, MOVING_SPEED, 150);
//------------- levanta perna esq -----------------------------------------------
dxl_write_word(18, P_GOAL_POSITION_L, 512+55);
dxl_write_word(20, P_GOAL_POSITION_L, 510-150);
dxl_write_word(22, P_GOAL_POSITION_L, 467-95);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+22);//corrigi inclinação do servo 15
dxl_write_word(16, P_GOAL_POSITION_L, 562-8);
dxl_write_word(7, P_GOAL_POSITION_L, 675);
espera_mov();
//------------conf velocidade para abaixar a perna esq --------------------------
dxl_write_word(20, MOVING_SPEED, 100);
dxl_write_word(18, MOVING_SPEED, 100*0.4);
dxl_write_word(22, MOVING_SPEED, 100*0.5);
dxl_write_word(19, MOVING_SPEED, 100);
//------------- abaixa perna esq -------------------------------------------------
dxl_write_word(18, P_GOAL_POSITION_L, 512+4);
dxl_write_word(20, P_GOAL_POSITION_L, 510-22);
dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina+30);//corrigi inclinação do servo 19 para voltar na posição correta
dxl_write_word(22, 24, 0); //desliga torque do servo 22 para evitar tranco
espera_mov();
//-------volta servos na posição inicial de inclinação depois de pé no chão -----
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina);
dxl_write_word(16, P_GOAL_POSITION_L, 562);
dxl_write_word(22, 24, 1);//liga torque do 22 para travar pé esq
dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina);
espera_mov();
//-----conf velocidade de 40 para voltar a posição ereto ------------------------
for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, 40);
//----volta a posição ereto -------------------------  --------------------------
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(18, P_GOAL_POSITION_L, 512);
dxl_write_word(20, P_GOAL_POSITION_L, 510);
dxl_write_word(22, P_GOAL_POSITION_L, 467+2);//corrigi desvio temporário de postura
dxl_write_word(16, P_GOAL_POSITION_L, 562-2);//corrigi desvio temporário de postura
espera_mov();
dxl_write_word(22, P_GOAL_POSITION_L, 467);//volta a posição inicial ereto
dxl_write_word(16, P_GOAL_POSITION_L, 562);//volta a posição inicial ereto

}

//================================================================================
levanta_e_abaixa_perna_dir(){
espera_mov();
//--------------conf velocidade 40------------------------------------------------
for(i = 1;i<23;i++)
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
for(i = 1;i<23;i++)
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

/*vira_torso_para_esq(){
for(i = 1;i<23;i++)
dxl_write_word(i, MOVING_SPEED, 40);
dxl_write_word(9, P_GOAL_POSITION_L, 571-51);
dxl_write_word(11, P_GOAL_POSITION_L, 585-51);
dxl_write_word(17, P_GOAL_POSITION_L, 601-51);
dxl_write_word(8, MOVING_SPEED, 100);
dxl_write_word(8, P_GOAL_POSITION_L, 682+200);


dxl_write_word(16, MOVING_SPEED, 300);
dxl_write_word(15, MOVING_SPEED, 300);
dxl_write_word(12, MOVING_SPEED, 400);
dxl_write_word(16, P_GOAL_POSITION_L, 562-10);
dxl_write_word(15, P_GOAL_POSITION_L, 498+77);
dxl_write_word(12, P_GOAL_POSITION_L, 512+30);



//dxl_write_word(16, MOVING_SPEED, 300);
//dxl_write_word(15, MOVING_SPEED, 300);
//dxl_write_word(12, MOVING_SPEED, 300);
//dxl_write_word(16, P_GOAL_POSITION_L, 562-30);
//dxl_write_word(15, P_GOAL_POSITION_L, 498+75);
//dxl_write_word(12, P_GOAL_POSITION_L, 512+30);

espera_mov();
//------------conf velocidade para abaixar a perna esq --------------------------
dxl_write_word(20, MOVING_SPEED, 100);
dxl_write_word(18, MOVING_SPEED, 100*0.4);
dxl_write_word(22, MOVING_SPEED, 100*0.5);

dxl_write_word(19, MOVING_SPEED, 200);
dxl_write_word(16, MOVING_SPEED, 300);
dxl_write_word(8, MOVING_SPEED, 200);
//------------- abaixa perna esq -------------------------------------------------
dxl_write_word(8, P_GOAL_POSITION_L, 682-50);
dxl_write_word(16, P_GOAL_POSITION_L, 562-20);
dxl_write_word(18, P_GOAL_POSITION_L, 512+4);
dxl_write_word(20, P_GOAL_POSITION_L, 510-6);
dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina+30);//corrigi inclinação do servo 19 para voltar na posição correta
dxl_write_word(22, 24, 0); //desliga torque do servo 22 para evitar tranco
usleep(500000);
dxl_write_word(12, P_GOAL_POSITION_L, 512+40);
dxl_write_word(21, P_GOAL_POSITION_L, 508+80);

espera_mov();
//-------volta servos na posição inicial de inclinação depois de pé no chão -----

dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina);
dxl_write_word(16, P_GOAL_POSITION_L, 562+2);
dxl_write_word(22, 24, 1);//liga torque do 22 para travar pé esq
dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina);

espera_mov();
//-----conf velocidade de 40 para voltar a posição ereto ------------------------
for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, 40);
//----volta a posição ereto -------------------------  --------------------------
dxl_write_word(5, MOVING_SPEED, 100);
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(18, P_GOAL_POSITION_L, 512);
dxl_write_word(20, P_GOAL_POSITION_L, 510);
dxl_write_word(5, P_GOAL_POSITION_L, 350+200);
//dxl_write_word(22, P_GOAL_POSITION_L, 467+10);//corrigi desvio temporário de postura
dxl_write_word(16, P_GOAL_POSITION_L, 562+10);//corrigi desvio temporário de postura
espera_mov();
dxl_write_word(22, P_GOAL_POSITION_L, 467);//volta a posição inicial ereto
//dxl_write_word(16, P_GOAL_POSITION_L, 562);//volta a posição inicial ereto

}*/


//================================================================================
//---- Função que faz o robô levantar-se se estiver no chão  ---------------------
//---- com o peito para cima e as costas no chão             ---------------------
void virar_de_costas_deitado(){
    for(i = 1;i<23;i++)
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


    levantar();
}
//================================================================================


//================================================================================
//---- Realiza um tempo de espera enquanto o motor estiver em movimento ----------
void delay_mov(unsigned int delta_posicao, unsigned int velocidade)
{
unsigned int delay; // tempo total para percorrer caminho informado

// 538.421 tempo em segundos que o servo leva par completar uma volta na velocidade 1
// 1228.8 definição de 1 volta completa -> pq 1023 = 300° portanto 1228.8 = 360°
delay = ((538.421/velocidade)*delta_posicao)/1228.8;

usleep(delay*1000000);

}
//================================================================================

andar_8(){
int braco = 300;
int a = 2;
int ctrl;

espera_mov();
//-------conf veloc 40 ---------------------------------------------------------
for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);

for (ctrl = 0; ctrl<11; ctrl++){

//======= 1o passo com a perna direira =======================================

if (a == 2){
//------conf veloc para os braços ---------------------------------------------
//dxl_write_word(4, MOVING_SPEED, 200);
//dxl_write_word(7, MOVING_SPEED, 200);

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
//dxl_write_word(4, P_GOAL_POSITION_L, 274+braco);
//dxl_write_word(7, P_GOAL_POSITION_L, 675);
espera_mov();

//================================================================================
//levanta_perna_dir(){
//--------------conf velocidade 40------------------------------------------------
for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//------conf veloc para os braços ---------------------------------------------
//dxl_write_word(4, MOVING_SPEED, 100);
//dxl_write_word(7, MOVING_SPEED, 100);
//------------- conf velocidade p/ levantar perna dir ---------------------------
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.66);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*2);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR);
//------------- levanta perna dir -----------------------------------------------
dxl_write_word(12, P_GOAL_POSITION_L, 512-55);//115
dxl_write_word(14, P_GOAL_POSITION_L, 510+150);//300
dxl_write_word(16, P_GOAL_POSITION_L, 562+95);//190
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-18);//corrigi inclinação do servo 21
dxl_write_word(22, P_GOAL_POSITION_L, 467+2);
//dxl_write_word(4, P_GOAL_POSITION_L, 274);
espera_mov();
//---------- abaixa a perna direita ---------------------------------------------
//------------conf velocidade para abaixar a perna dir --------------------------
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.5);
dxl_write_word(13, MOVING_SPEED, VEL_ANDAR);

dxl_write_word(12, P_GOAL_POSITION_L, 512-37);
dxl_write_word(14, P_GOAL_POSITION_L, 510-12);
dxl_write_word(13, P_GOAL_POSITION_L, 498-71);
dxl_write_word(11, P_GOAL_POSITION_L, 585+27);
dxl_write_word(16, 24, 0);
dxl_write_word(15, 24, 0);
espera_mov();
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina);
espera_mov();
dxl_write_word(16, 24, 1);
dxl_write_word(15, 24, 1);
//-----conf velocidade de 40 para voltar a posição ereto ------------------------
for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//----volta a posição ereto -----------------------------------------------------
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
espera_mov();
a=0;
}


//====== passo longo com a perna esquerda ==========================================

if (a == 0){
//------ inclina robo para o lado direito ------------------------------------
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(11, P_GOAL_POSITION_L, 607);
dxl_write_word(12, P_GOAL_POSITION_L, 475);
dxl_write_word(14, P_GOAL_POSITION_L, 499);
dxl_write_word(16, P_GOAL_POSITION_L, 535);
dxl_write_word(17, P_GOAL_POSITION_L, 603);
dxl_write_word(18, P_GOAL_POSITION_L, 516);
dxl_write_word(20, P_GOAL_POSITION_L, 513);
dxl_write_word(22, P_GOAL_POSITION_L, 469);
espera_mov();
dxl_write_word(13, P_GOAL_POSITION_L, 498+inclina);
dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina);
dxl_write_word(21, P_GOAL_POSITION_L, 508+inclina);
espera_mov();
//levanta_perna_esq(){
//--------------conf velocidade 40------------------------------------------------
for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//------------- conf velocidade p/ levantar perna esq ---------------------------
dxl_write_word(20, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.66);
dxl_write_word(15, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*1.5);
//------------- levanta perna esq -----------------------------------------------
dxl_write_word(18, P_GOAL_POSITION_L, 512+55);
dxl_write_word(20, P_GOAL_POSITION_L, 510-150);
dxl_write_word(22, P_GOAL_POSITION_L, 467-95);
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+22);//corrigi inclinação do servo 15
dxl_write_word(16, P_GOAL_POSITION_L, 562-20);
espera_mov();
//dxl_write_word(18, P_GOAL_POSITION_L, 512+125);
//-----movimenta perna para frente --------------------------------------------
//dxl_write_word(22, P_GOAL_POSITION_L, 467+52);
dxl_write_word(20, P_GOAL_POSITION_L, 510-30);
dxl_write_word(18, P_GOAL_POSITION_L, 512+54);
dxl_write_word(22, P_GOAL_POSITION_L, 467+27);
dxl_write_word(17, P_GOAL_POSITION_L, 601-15);
dxl_write_word(19, P_GOAL_POSITION_L, 512+50);
dxl_write_word(12, P_GOAL_POSITION_L, 512+11);
dxl_write_word(16, P_GOAL_POSITION_L, 562-10);
espera_mov();
dxl_write_word(19, P_GOAL_POSITION_L, 512+71);
dxl_write_word(17, P_GOAL_POSITION_L, 601-25);

espera_mov();
//-------- abaixa perna esq --------------------------------------------------
dxl_write_word(20, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.5);
//dxl_write_word(19, MOVING_SPEED, 110);
//dxl_write_word(17, MOVING_SPEED, 110);

dxl_write_word(22, 24, 0);
dxl_write_word(21, 24, 0);
dxl_write_word(18, P_GOAL_POSITION_L, 512+37);
dxl_write_word(20, P_GOAL_POSITION_L, 510-2);

dxl_write_word(11, P_GOAL_POSITION_L, 585);
espera_mov();
dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina);
//dxl_write_word(16, P_GOAL_POSITION_L, 562-0);

espera_mov();
dxl_write_word(22, 24, 1);
dxl_write_word(21, 24, 1);
//-----conf velocidade de 40 para voltar a posição ereto ------------------------
for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//----volta a posição ereto -------------------------  --------------------------
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
espera_mov();
a = 1;
}

//========== passo longo perna direita ==========================================

if (a == 1){
//------ inclina robo para o lado esquerdo ------------------------------------
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(11, P_GOAL_POSITION_L, 581);
dxl_write_word(12, P_GOAL_POSITION_L, 517);
dxl_write_word(14, P_GOAL_POSITION_L, 492);
dxl_write_word(16, P_GOAL_POSITION_L, 565-20);
dxl_write_word(17, P_GOAL_POSITION_L, 563);
dxl_write_word(18, P_GOAL_POSITION_L, 548);
dxl_write_word(20, P_GOAL_POSITION_L, 504);
dxl_write_word(22, P_GOAL_POSITION_L, 468);
espera_mov();
dxl_write_word(13, P_GOAL_POSITION_L, 498-inclina);
dxl_write_word(19, P_GOAL_POSITION_L, 512-inclina);
dxl_write_word(15, P_GOAL_POSITION_L, 498-inclina);
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina);
dxl_write_word(16, P_GOAL_POSITION_L, 565);

espera_mov();
//levanta_perna_dir(){
//--------------conf velocidade 40------------------------------------------------
for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//------------- conf velocidade p/ levantar perna esq ---------------------------
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.66);
dxl_write_word(21, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(22, MOVING_SPEED, VEL_ANDAR);
//------------- levanta perna dir -----------------------------------------------
//dxl_write_word(18, P_GOAL_POSITION_L, 512);
dxl_write_word(12, P_GOAL_POSITION_L, 512-55);//115
dxl_write_word(14, P_GOAL_POSITION_L, 510+150);//300
dxl_write_word(16, P_GOAL_POSITION_L, 562+95);//190
dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-13);//corrigi inclinação do servo 21
dxl_write_word(22, P_GOAL_POSITION_L, 467+2);
espera_mov();
//dxl_write_word(18, P_GOAL_POSITION_L, 512+125);
//-----movimenta perna para frente --------------------------------------------
//dxl_write_word(22, P_GOAL_POSITION_L, 467+52);
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.2);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.8);

dxl_write_word(14, P_GOAL_POSITION_L, 610);//37
dxl_write_word(12, P_GOAL_POSITION_L, 428);
dxl_write_word(16, P_GOAL_POSITION_L, 576);//39
dxl_write_word(11, P_GOAL_POSITION_L, 585+15);
dxl_write_word(13, P_GOAL_POSITION_L, 498-50);
dxl_write_word(18, P_GOAL_POSITION_L, 512);
dxl_write_word(22, P_GOAL_POSITION_L, 467-5);
espera_mov();
dxl_write_word(13, P_GOAL_POSITION_L, 498-65);
dxl_write_word(11, P_GOAL_POSITION_L, 585+25);
espera_mov();

//-------- abaixa perna esq --------------------------------------------------
dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.5);
//dxl_write_word(11, MOVING_SPEED, 110);

dxl_write_word(16, 24, 0);
dxl_write_word(15, 24, 0);
dxl_write_word(12, P_GOAL_POSITION_L, 512-37);//512-37
dxl_write_word(14, P_GOAL_POSITION_L, 510+20);//510-2
dxl_write_word(17, P_GOAL_POSITION_L, 601);
espera_mov();
dxl_write_word(21, P_GOAL_POSITION_L, 498-inclina);
espera_mov();
dxl_write_word(16, 24, 1);
dxl_write_word(15, 24, 1);
//-----conf velocidade de 40 para voltar a posição ereto ------------------------
for(i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
//----volta a posição ereto -------------------------  --------------------------
dxl_write_word(19, P_GOAL_POSITION_L, 512);
dxl_write_word(13, P_GOAL_POSITION_L, 498);
dxl_write_word(21, P_GOAL_POSITION_L, 508);
dxl_write_word(15, P_GOAL_POSITION_L, 498);
dxl_write_word(12, P_GOAL_POSITION_L, 512-54);
dxl_write_word(14, P_GOAL_POSITION_L, 510+10);
dxl_write_word(16, P_GOAL_POSITION_L, 547);
espera_mov();
a = 0;
} //for
} // if
} // função
