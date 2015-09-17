/*--------------------------------------------------------------------

******************************************************************************
  * @file       decision.cpp
  * @author     Isaac Jesus da Silva - ROBOFEI-HT - FEI
  * @version    V0.0.3
  * @created    11/07/2014
  * @Modified   14/07/2014
  * @e-mail     isaac25silva@yahoo.com.br
  * @brief      Decision
  ****************************************************************************
/--------------------------------------------------------------------*/

#include <iostream>
#include <blackboard.h>
#include<unistd.h>
#include <cmath>

#define DEBUG

int main(void)
{
int pos_servo1 = 803;
int pos_servo2 = 512;

float dist_ball;

using_shared_memory();


std::cout<<"Decision"<<std::endl;
VISION_MOTOR1_GOAL=0;

DECISION_ACTION_VISION=0;
VISION_DIST_GOAL=0;
VISION_LOST_GOAL=0;

while(1)
{

	{


std::cout<<"VIsion State "<<VISION_STATE;
    if(VISION_STATE==0) // SE for 1 significa QUE O ROBO esta com os olhos vendados
    {
//DECISION_ACTION_VISION=0;
//sleep(8);
//DECISION_ACTION_VISION=1;
//sleep(8);


	dist_ball = VISION_DIST_BALL;
	//head_motor_angle = VISION_MOTOR_ANGLE;
	if (VISION_SEARCH_BALL == 1) // vira se a bola estiver perdida no campo
	{
		DECISION_ACTION_A = 0;
		if (VISION_LOST_BALL == 1)
			DECISION_ACTION_A = 1; //andar
	}
	else
	{
	     if (VISION_LOST_BALL == 1)
		DECISION_ACTION_A = 0; //Parar porque pode ter encontrado a bola
	     else
	     {
	     //-----------Fora do circulo1 ---#servo1 maior que 535-----------------------------------------------------------
		if (VISION_MOTOR1_ANGLE > (pos_servo1 - 268)) //535 // Bola esta longe----------------
        	{
			
			//Bola longe do pé porem entre 430 a 610 (em uma posição quase enfrente ao robo)
            		if(VISION_MOTOR2_ANGLE >= (pos_servo2 - 82) && VISION_MOTOR2_ANGLE <= (pos_servo2 + 90)) // 430 a 610
			{
				// Circulo2 #Servo1 entre 535 a 575----------------------------------------------
				if(VISION_MOTOR1_ANGLE < (pos_servo1 - 228))//575
				{
               				DECISION_ACTION_A = 8;  //andar lento
				}
				//-------------------------------------------------------------------------------

				else
					DECISION_ACTION_A = 1;  //andar
			}

           		if(VISION_MOTOR2_ANGLE > (pos_servo2 + 90)) //610 // Bola a esquerda do robo------------
			{
				DECISION_ACTION_A = 2;  //virar a esquerda
			}
           		if(VISION_MOTOR2_ANGLE < (pos_servo2 - 82)) //430 // Bola a direita do robô-------------
			{
                		DECISION_ACTION_A = 3;  //virar a direita
			}
        	}
	    //--------------------------------------------------------------------------------------------------------

	    //------- Circulo1  #servo1 de 0 a 535#-------------------------------------------------------------------
       		else  // Bola esta perto--------------------------------------------------------------
		{
		     //VISION_DIST_GOAL=0;
		     std::cout<<"Lost Goal "<<VISION_LOST_GOAL<<"\n";
		     while(VISION_DIST_GOAL==0 && VISION_LOST_GOAL==0) //fica preso no laço enquanto procura o gol e sai se achar o gol ou se não achar
		     {
		     	DECISION_ACTION_VISION=1;
			sleep(4);
		     }
			VISION_LOST_GOAL=0;
		     	DECISION_ACTION_VISION=0;			
			std::cout<<"dif = "<<std::abs(VISION_MOTOR2_GOAL-VISION_MOTOR2_ANGLE)<<std::endl;
			    //---- verifica se ele e o gol esta alinhado com a bola---------------------
			    if(std::abs(VISION_MOTOR2_GOAL-VISION_MOTOR2_ANGLE) < 70 )
			    {
				//--------Chutar somente se estiver alinhado com a bola----------------------------------------------
				
				//chute direito 520 a 400
				if(VISION_MOTOR2_ANGLE <= (pos_servo2 - 32) && VISION_MOTOR2_ANGLE >= (pos_servo2 - 113)) //480 a 400
              				DECISION_ACTION_A = 4; //chutar com pe direito
				if(VISION_MOTOR2_ANGLE <= (pos_servo2 + 8)  && VISION_MOTOR2_ANGLE >(pos_servo2 - 32)) // 520 a 480
              				DECISION_ACTION_A = 4; //chutar com pe direito

				//chute esquerdo 520 a 652
				if(VISION_MOTOR2_ANGLE >= (pos_servo2 + 58) && VISION_MOTOR2_ANGLE <= (pos_servo2 + 140)) // 570 a 652
              				DECISION_ACTION_A = 5; //chutar com pe esquerdo
				if(VISION_MOTOR2_ANGLE < (pos_servo2 + 58)  && VISION_MOTOR2_ANGLE >(pos_servo2 + 8)) //570 a 520
              				DECISION_ACTION_A = 5; //chutar com pe esquerdo
				//---------------------------------------------------------------------------------------------------

				if(VISION_MOTOR2_ANGLE < (pos_servo2 - 113) && VISION_MOTOR2_ANGLE >= (pos_servo2 - 197)) //400 a 315
					DECISION_ACTION_A = 7; //andar de lado direita
				if(VISION_MOTOR2_ANGLE < (pos_servo2 - 197)) // 315
					DECISION_ACTION_A = 3;  //virar a direita

				if(VISION_MOTOR2_ANGLE > (pos_servo2 + 140) && VISION_MOTOR2_ANGLE <= (pos_servo2 + 225)) //652 a 737
					DECISION_ACTION_A = 6; //andar de lado esquerda
				if(VISION_MOTOR2_ANGLE > (pos_servo2 + 225)) //737
					DECISION_ACTION_A = 2; //virar a esquerda;

			    }
			    else
			    {
				if((VISION_MOTOR2_GOAL-VISION_MOTOR2_ANGLE) > 0 )//---- verifica se ele necessita se alinhar a direita ou a esquerda--
				{
					//---- Alinhamento a direita-------------------
					DECISION_ACTION_A = 7; //andar de lado direita
				}
				else
				{
					//---- Alinhamento a esquerda------------------
					DECISION_ACTION_A = 6; //andar de lado esquerda		
				}
			    }

		}
	    //----------------------------------------------------------------------------------------------------------
	     }
	}
    }
    else
    {
	DECISION_ACTION_A = 0;
	DECISION_ACTION_VISION=0;
    }
	}
	usleep(500000);
	std::cout<< "Action " << DECISION_ACTION_A;
	if(DECISION_ACTION_A == 0)
		std::cout<<" | Nada a fazer"<<std::endl;
	if(DECISION_ACTION_A == 1)
		std::cout<<" | Andar"<<std::endl;
	if(DECISION_ACTION_A == 2)
		std::cout<<" | Virar a esquerda"<<std::endl;
	if(DECISION_ACTION_A == 3)
		std::cout<<" | Virar a direita"<<std::endl;
	if(DECISION_ACTION_A == 4)
		std::cout<<"| Chutar com pe direito"<<std::endl;
	if(DECISION_ACTION_A == 5)
		std::cout<<" | Chutar com pe esquerdo"<<std::endl;
	if(DECISION_ACTION_A == 6)
		std::cout<<" | Andar de Lado esquerda"<<std::endl;
	if(DECISION_ACTION_A == 7)
		std::cout<<" | Andar de Lado direita"<<std::endl;
	if(DECISION_ACTION_A == 8)
		std::cout<<" | Andar Lento"<<std::endl;
	if(DECISION_ACTION_A < 0 || DECISION_ACTION_A > 8)
		std::cout<<" | Esperando receber dados da visão"<<std::endl;

	std::cout<< "Lost Ball " << VISION_LOST_BALL << std::endl;
	std::cout<< "Search ball " << VISION_SEARCH_BALL <<std::endl;

	std::cout<<"Search Goal "<<VISION_DIST_GOAL<<std::endl;
	std::cout<<"Servo Goal "<<VISION_MOTOR2_GOAL<<std::endl;
	std::cout<<"Servo Ball "<<VISION_MOTOR1_ANGLE<<std::endl;

/*
a = COM_ACTION_ROBOT1;
b = COM_ACTION_ROBOT2;
c = COM_ACTION_ROBOT3;
d=COM_STATE_ROBOT1;
e=COM_STATE_ROBOT2;
f=COM_STATE_ROBOT3;
g=COM_POS_ROBOT1;
h=COM_POS_ROBOT2;
i=COM_POS_ROBOT3;
j=COM_POS_BALL_ROBOT1;
k=COM_POS_BALL_ROBOT2;
l=COM_POS_BALL_ROBOT3;
m=COM_POS_OPP_A_ROBOT1;
n=COM_POS_OPP_A_ROBOT2;
o=COM_POS_OPP_A_ROBOT3;
p=COM_POS_OPP_A_ROBOT4;
q=COM_POS_OPP_B_ROBOT1;
r=COM_POS_OPP_B_ROBOT2;
s=COM_POS_OPP_B_ROBOT3;
t=COM_POS_OPP_B_ROBOT4;
u=COM_POS_OPP_C_ROBOT1;
v=COM_POS_OPP_C_ROBOT2;
x=COM_POS_OPP_C_ROBOT3;
z=COM_POS_OPP_C_ROBOT4;
aa = COM_REFEREE;
ab = LOCALIZATION_THETA;
ac = LOCALIZATION_X;
ad = LOCALIZATION_Y;
ae = VISION_DIST_BALL;
af = VISION_DIST_GOAL;
ag = VISION_DIST_OPP1;
ah = VISION_DIST_OPP2;
ai = VISION_DIST_OPP3;
aj = CONTROL_ACTION;

usleep(50);
//SENT VARIABLES
DECISION_ACTION_A = 15; //PLANNING
DECISION_ACTION_B = 16+inc; //PLANNING
DECISION_STATE = 1+inc; //COMMUNICATION
DECISION_POSITION_A = 10+inc; //COMMUNICATION
DECISION_POSITION_B = 11+inc; //COMMUNICATION
DECISION_POSITION_C = 12+inc; //COMMUNICATION
DECISION_BALL_POS = 20+inc; //COMMUNICATION
DECISION_OPP1_POS = 30+inc; //COMMUNICATION
DECISION_OPP2_POS = 31+inc; //COMMUNICATION
DECISION_OPP3_POS = 32+inc; //COMMUNICATION
}

inc++;

#ifdef DEBUG
cout << "decision received " << a <<" "<< b <<" "<< c <<" "<< d <<" "<< e <<" "<< f <<" "<< g <<" "<< h <<" "<< i <<" "<< j <<" "<< k <<" "<< l <<" "<< m <<" "<< n <<" "<< o <<" "<< p <<" "<< q <<" "<< r <<" "<< s <<" "<< t <<" "<< u <<" "<< v <<" "<< x <<" "<< z << " " << ab << " " << ac <<" " << ad << " "<< ae << " " << af <<" "<< ag << " " << ah << " " << ai << endl;
#endif
*/
}
}

