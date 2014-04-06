#include <dynamixel.h>
#include <math.h>
#include <stdio.h>
#define XC .038 //
#define YC .05 //Altura da bola em relação ao servo

//===========================================================================================================
//------- Calcula a ditãncia da Bola em relação a frente do robô --------------------------------------------
float distancia_bola()
{
    float alpha;
    float dist;
    printf("servo 1: %d",dxl_read_word( 1, P_PRESENT_POSITION_L));
    alpha = ((531-dxl_read_word( 1, P_PRESENT_POSITION_L))*0.005111)+0.0349;  //0.0349
    dist = tan(alpha)*(cos(3.1415-alpha)*YC+.448-sin(1.5708-alpha)*XC)+(sin(3.1415-alpha)*YC)+cos(1.5708-alpha)*XC;
    return (dist+0.05); // Distancia da bola em ralação aos pés do robô
}


//===========================================================================================================
//------- Calcula o angulo da Bola em relação a frente do robô ----------------------------------------------
float angle_bola()
{
//angle = 594-dxl_read_word( 1, P_PRESENT_POSITION_L);


}
//-----------------------------------------------------------------------------------------------------------
/*
float distancia_bola(){
float alpha;
float dist;
printf("servo 1: %d",dxl_read_word( 1, P_PRESENT_POSITION_L));
alpha = (531-dxl_read_word( 1, P_PRESENT_POSITION_L))*0.005113;
dist = tan(alpha)*(cos(3.1415-alpha)*0.05+.45)-(sin(3.1415-alpha)*0.05);
return dist;
}*/
