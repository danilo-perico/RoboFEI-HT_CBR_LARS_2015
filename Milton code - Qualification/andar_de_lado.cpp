#include <dynamixel.h>

#define VEL_ANDAR 200 //define velocidade para andar //200

void andar_lateral_direita(int passos)
{
//int braco = 300;
//int a = 2;
int ctrl;

    espera_mov();
//-------conf veloc 40 ---------------------------------------------------------
    for(int i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.3);

    for (ctrl = 0; ctrl< passos; ctrl++){ // ******* for para quantidade de passos******

    //dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.3);
    //dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.3);
    //dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.3);
    //dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.3);

    dxl_write_word(13, P_GOAL_POSITION_L, 498);
    dxl_write_word(19, P_GOAL_POSITION_L, 512);
    dxl_write_word(15, P_GOAL_POSITION_L, 498);
    dxl_write_word(21, P_GOAL_POSITION_L, 508);
    espera_mov();

    //---- inclina robo para lado direito-----------------------
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
    dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
    dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
    dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.66);
    dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*.6);
    dxl_write_word(22, MOVING_SPEED, VEL_ANDAR);
    //------------- levanta perna dir -----------------------------------------------
    dxl_write_word(12, P_GOAL_POSITION_L, 512-40);//115
    dxl_write_word(14, P_GOAL_POSITION_L, 510+80);//300
    dxl_write_word(16, P_GOAL_POSITION_L, 562+45);//190
    dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-10);//corrigi inclinação do servo 21
    dxl_write_word(22, P_GOAL_POSITION_L, 467+2);
    espera_mov();
    //dxl_write_word(4, P_GOAL_POSITION_L, 274);
    //--------------abre perna direita -----------------------------------------------
    dxl_write_word(13, P_GOAL_POSITION_L, 498-60);
    dxl_write_word(14, MOVING_SPEED, VEL_ANDAR*2);
    dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-12);//corrigi inclinação do servo 21
    dxl_write_word(15, P_GOAL_POSITION_L, 498-inclina);
    espera_mov();
    //---------- abaixa a perna direita ---------------------------------------------
    //------------conf velocidade para abaixar a perna dir --------------------------
    dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
    dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
    dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.5);
    dxl_write_word(13, MOVING_SPEED, VEL_ANDAR);

    dxl_write_word(12, P_GOAL_POSITION_L, 512);
    dxl_write_word(14, P_GOAL_POSITION_L, 510);
    dxl_write_word(13, P_GOAL_POSITION_L, 498-75);
    //dxl_write_word(13, P_GOAL_POSITION_L, 498-45);//71
    dxl_write_word(11, P_GOAL_POSITION_L, 585+27);
    dxl_write_word(16, 24, 0);
    dxl_write_word(15, 24, 0);
    espera_mov();
    dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina);
    espera_mov();
    dxl_write_word(16, 24, 1);
    dxl_write_word(15, 24, 1);
    //-----conf velocidade de 40 para voltar a posição ereto ------------------------
    for(int i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
    //----volta a posição ereto -----------------------------------------------------
    dxl_write_word(19, P_GOAL_POSITION_L, 512);
    dxl_write_word(13, P_GOAL_POSITION_L, 498-30);
    dxl_write_word(21, P_GOAL_POSITION_L, 508);
    dxl_write_word(15, P_GOAL_POSITION_L, 498-20);
    dxl_write_word(16, P_GOAL_POSITION_L, 562);
    espera_mov();
    //-------inclinação -----
    dxl_write_word(13, P_GOAL_POSITION_L, 498+inclina);
    dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina+10);
    dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina-7);
    dxl_write_word(21, P_GOAL_POSITION_L, 508+inclina-4);
    dxl_write_word(20, P_GOAL_POSITION_L, 510+10);
    dxl_write_word(18, P_GOAL_POSITION_L, 512-5);
    dxl_write_word(22, P_GOAL_POSITION_L, 467+5);
    dxl_write_word(12, P_GOAL_POSITION_L, 512-10);
    dxl_write_word(14, P_GOAL_POSITION_L, 510+20);
    dxl_write_word(16, P_GOAL_POSITION_L, 562+10);//25
    //------------- conf velocidade p/ levantar perna dir ---------------------------
    dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
    dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
    dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.66);
    dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*2);
    dxl_write_word(22, MOVING_SPEED, VEL_ANDAR);
    espera_mov();
    //------------- conf velocidade p/ levantar perna esq ---------------------------
    dxl_write_word(20, MOVING_SPEED, 1.3*VEL_ANDAR*.5);
    dxl_write_word(18, MOVING_SPEED, 1.3*VEL_ANDAR*0.35);
    dxl_write_word(22, MOVING_SPEED, 1.3*VEL_ANDAR*0.33);
    dxl_write_word(15, MOVING_SPEED, 1.3*VEL_ANDAR*.5);
    dxl_write_word(16, MOVING_SPEED, 1.3*VEL_ANDAR*.75);
    //------------- levanta perna esq -----------------------------------------------
    dxl_write_word(18, P_GOAL_POSITION_L, 512+60);
    dxl_write_word(20, P_GOAL_POSITION_L, 510-80);
    dxl_write_word(22, P_GOAL_POSITION_L, 467-27);
    dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+22);//corrigi inclinação do servo 15
    dxl_write_word(16, P_GOAL_POSITION_L, 562+10);
    //dxl_write_word(7, P_GOAL_POSITION_L, 675);
    espera_mov();
    //---------- abaixa a perna direita ---------------------------------------------
    //------------conf velocidade para abaixar a perna dir --------------------------
    dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.5);
    dxl_write_word(20, MOVING_SPEED, VEL_ANDAR*0.6);
    dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.7);
    dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*1.1);

    dxl_write_word(18, P_GOAL_POSITION_L, 512+6);
    dxl_write_word(20, P_GOAL_POSITION_L, 510);
    dxl_write_word(19, P_GOAL_POSITION_L, 512+65);
    dxl_write_word(12, P_GOAL_POSITION_L, 512);
    dxl_write_word(14, P_GOAL_POSITION_L, 510);
    dxl_write_word(16, P_GOAL_POSITION_L, 562-4);//25
    //dxl_write_word(13, P_GOAL_POSITION_L, 498-45);//71
    //dxl_write_word(11, P_GOAL_POSITION_L, 585+27);
    dxl_write_word(21, 24, 0);
    dxl_write_word(22, 24, 0);
    espera_mov();
    dxl_write_word(15, P_GOAL_POSITION_L, 598-inclina);
    espera_mov();
    dxl_write_word(21, 24, 1);
    dxl_write_word(22, 24, 1);
    dxl_write_word(22, P_GOAL_POSITION_L, 467+5);
    espera_mov();
    dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.4);
    dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.4);
    dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.4);
    dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.4);
    dxl_write_word(13, P_GOAL_POSITION_L, 498);
    dxl_write_word(19, P_GOAL_POSITION_L, 512);
    dxl_write_word(15, P_GOAL_POSITION_L, 498);
    dxl_write_word(21, P_GOAL_POSITION_L, 508);
    espera_mov();
    }
}

void andar_lateral_esquerda(int passos)
{
    dxl_write_word(13, P_GOAL_POSITION_L, 498);
    dxl_write_word(19, P_GOAL_POSITION_L, 512);
    dxl_write_word(15, P_GOAL_POSITION_L, 498);
    dxl_write_word(21, P_GOAL_POSITION_L, 508);
    espera_mov();

    //---- inclina robo lado esquerdo-----------------------
    dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.35);
    dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*0.20);
    dxl_write_word(20, MOVING_SPEED, VEL_ANDAR*0.20);
    dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.20);

    dxl_write_word(19, P_GOAL_POSITION_L, 512+inclina-15);
    dxl_write_word(13, P_GOAL_POSITION_L, 498+inclina-5);
    dxl_write_word(21, P_GOAL_POSITION_L, 508+inclina-8);
    dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+2);//25
    dxl_write_word(16, P_GOAL_POSITION_L, 562-7);//25

    dxl_write_word(18, P_GOAL_POSITION_L, 512-10);
    dxl_write_word(20, P_GOAL_POSITION_L, 510+15);
    dxl_write_word(22, P_GOAL_POSITION_L, 467-5);

    dxl_write_word(17, P_GOAL_POSITION_L, 601-8);
    espera_mov();

    //================================================================================
    //levanta_perna_esq(){
    //--------------conf velocidade 40------------------------------------------------
    for(int i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.6);
    //------conf veloc para os braços ---------------------------------------------
    //dxl_write_word(4, MOVING_SPEED, 100);
    //dxl_write_word(7, MOVING_SPEED, 100);
    //------------- conf velocidade p/ levantar perna esq ---------------------------
    dxl_write_word(18, MOVING_SPEED, VEL_ANDAR*.526);
    dxl_write_word(20, MOVING_SPEED, VEL_ANDAR);
    dxl_write_word(22, MOVING_SPEED, VEL_ANDAR*0.474);
    dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*.3);
    dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.1);
    //------------- levanta perna esq -----------------------------------------------
    dxl_write_word(18, P_GOAL_POSITION_L, 512+40);//115
    dxl_write_word(20, P_GOAL_POSITION_L, 510-80);//300
    dxl_write_word(22, P_GOAL_POSITION_L, 467-45);//190
    dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+16);//corrigi inclinação do servo 21
    dxl_write_word(16, P_GOAL_POSITION_L, 562-7);
    espera_mov();
    //--------------abre perna esquerda -----------------------------------------------
    dxl_write_word(19, P_GOAL_POSITION_L, 512+90);
    dxl_write_word(15, MOVING_SPEED, VEL_ANDAR);
    dxl_write_word(21, P_GOAL_POSITION_L, 508+inclina);
    dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+22);
    espera_mov();

    //---------- abaixa a perna esquerda ---------------------------------------------
    //------------conf velocidade para abaixar a perna dir --------------------------
    dxl_write_word(20, MOVING_SPEED, 1.5*VEL_ANDAR);
    dxl_write_word(18, MOVING_SPEED, 1.5*VEL_ANDAR*0.483);
    dxl_write_word(17, MOVING_SPEED, 1.5*VEL_ANDAR*0.2);
    dxl_write_word(19, MOVING_SPEED, 1.5*VEL_ANDAR*0.1);

    dxl_write_word(22, 24, 0);
    dxl_write_word(21, 24, 0);
    dxl_write_word(18, P_GOAL_POSITION_L, 512-18);
    dxl_write_word(20, P_GOAL_POSITION_L, 510+40);
    dxl_write_word(19, P_GOAL_POSITION_L, 512+92);
    dxl_write_word(17, P_GOAL_POSITION_L, 601-15);
    dxl_write_word(16, P_GOAL_POSITION_L, 562-5);
    espera_mov();


    dxl_write_word(15, P_GOAL_POSITION_L, 498+inclina+10);
    espera_mov();
    dxl_write_word(22, 24, 1);
    dxl_write_word(21, 24, 1);
    dxl_write_word(21, MOVING_SPEED, 2*VEL_ANDAR);
    dxl_write_word(22, MOVING_SPEED, 2*VEL_ANDAR);
    dxl_write_word(21, P_GOAL_POSITION_L, 508+inclina+15);
    dxl_write_word(22, P_GOAL_POSITION_L, 467+10);
    espera_mov();
    //-----conf velocidade de 40 para voltar a posição ereto ------------------------
    for(int i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.4);
    //----volta a posição ereto -----------------------------------------------------
    dxl_write_word(19, P_GOAL_POSITION_L, 512);
    dxl_write_word(13, P_GOAL_POSITION_L, 498-30);
    dxl_write_word(21, P_GOAL_POSITION_L, 508);
    dxl_write_word(15, P_GOAL_POSITION_L, 498-20);
    dxl_write_word(16, P_GOAL_POSITION_L, 562);
    espera_mov();

    //---- inclina robo para lado direito-----------------------
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
    dxl_write_word(12, P_GOAL_POSITION_L, 512+15);
    dxl_write_word(14, P_GOAL_POSITION_L, 510-30);
    dxl_write_word(16, P_GOAL_POSITION_L, 562-10);

    dxl_write_word(18, P_GOAL_POSITION_L, 512+10);
    dxl_write_word(20, P_GOAL_POSITION_L, 510-25);
    dxl_write_word(22, P_GOAL_POSITION_L, 467-14);
    //dxl_write_word(22, P_GOAL_POSITION_L, 467+5);//25
    espera_mov();

    //levanta_perna_dir(){
    //--------------conf velocidade 40------------------------------------------------
    for(int i = 1;i<23;i++)
       dxl_write_word(i, MOVING_SPEED, VEL_ANDAR*0.6);
    //------conf veloc para os braços ---------------------------------------------
    //dxl_write_word(4, MOVING_SPEED, 100);
    //dxl_write_word(7, MOVING_SPEED, 100);
    //------------- conf velocidade p/ levantar perna dir ---------------------------
    dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
    dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.4);
    dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.66);
    dxl_write_word(21, MOVING_SPEED, VEL_ANDAR*.6);
    dxl_write_word(22, MOVING_SPEED, VEL_ANDAR);
    //------------- levanta perna dir -----------------------------------------------
    dxl_write_word(12, P_GOAL_POSITION_L, 512-40);//115
    dxl_write_word(14, P_GOAL_POSITION_L, 510+80);//300
    dxl_write_word(16, P_GOAL_POSITION_L, 562+45);//190
    dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina-10);//corrigi inclinação do servo 21
    dxl_write_word(22, P_GOAL_POSITION_L, 467-14);
    espera_mov();

    //---------- abaixa a perna direita ---------------------------------------------
    //------------conf velocidade para abaixar a perna dir --------------------------
    dxl_write_word(14, MOVING_SPEED, VEL_ANDAR);
    dxl_write_word(12, MOVING_SPEED, VEL_ANDAR*0.5);
    //dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.5);
    dxl_write_word(13, MOVING_SPEED, VEL_ANDAR);

    dxl_write_word(12, P_GOAL_POSITION_L, 512);
    dxl_write_word(14, P_GOAL_POSITION_L, 510);
    dxl_write_word(13, P_GOAL_POSITION_L, 498-45);
    //dxl_write_word(13, P_GOAL_POSITION_L, 498-45);//71
    //dxl_write_word(11, P_GOAL_POSITION_L, 585+27);
    dxl_write_word(16, 24, 0);
    dxl_write_word(15, 24, 0);
    espera_mov();
    dxl_write_word(21, P_GOAL_POSITION_L, 508-inclina);
    espera_mov();
    dxl_write_word(16, 24, 1);
    dxl_write_word(15, 24, 1);
    espera_mov();

    //-----conf velocidade de 40 para voltar a posição ereto ------------------------
    dxl_write_word(19, MOVING_SPEED, VEL_ANDAR*0.538);
    dxl_write_word(13, MOVING_SPEED, VEL_ANDAR*0.615);
    dxl_write_word(21, MOVING_SPEED, VEL_ANDAR);
    dxl_write_word(15, MOVING_SPEED, VEL_ANDAR*0.5);
    dxl_write_word(16, MOVING_SPEED, VEL_ANDAR*0.692);
    //----volta a posição ereto -----------------------------------------------------
    dxl_write_word(19, P_GOAL_POSITION_L, 512);
    dxl_write_word(13, P_GOAL_POSITION_L, 498-5);
    dxl_write_word(21, P_GOAL_POSITION_L, 508);
    dxl_write_word(15, P_GOAL_POSITION_L, 498-20);
    dxl_write_word(16, P_GOAL_POSITION_L, 562);
    espera_mov();
    robo_ereto_corrigido();

}
