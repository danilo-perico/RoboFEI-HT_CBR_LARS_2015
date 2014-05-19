/*--------------------------------------------------------------------

******************************************************************************
  * @file       communication.cpp
  * @author     Danilo Hernnai Perico - ROBOFEI-HT - FEI
  * @version    V0.0.1
  * @created    08/04/2014
  * @Modified   30/04/2014
  * @e-mail     danilo.perico@gmail.com
  * @brief      Communication
  ****************************************************************************
/--------------------------------------------------------------------*/


#include <iostream>
#include "communication.h"
#include "blackboard.h"
#include<pthread.h>
#include <unistd.h>

#define DEBUG
//#define _cpluplus

#ifdef _cpluplus
communication::communication()
{
    //ctor
}

communication::~communication()
{
    //dtor
}
#endif


int main(void)
{
int A, B, C, D, E, F, G, H, I, J;
using namespace std;

using_shared_memory();

#ifdef DEBUG
cout << "Communication" << endl;
#endif

while(1)
{
for (int i = 0;i<10000;i++)
{
//SENT VARIABLES TO DECISION
COM_ACTION_ROBOT1 = 1;
COM_ACTION_ROBOT2 = 2;
COM_ACTION_ROBOT3 = 3;
COM_STATE_ROBOT1 = 10;
COM_STATE_ROBOT2 = 11;
COM_STATE_ROBOT3 = 12;
COM_POS_ROBOT1 = 20 ;
COM_POS_ROBOT2 = 21;
COM_POS_ROBOT3 = 22;
COM_POS_BALL_ROBOT1 = 30;
COM_POS_BALL_ROBOT2 = 31;
COM_POS_BALL_ROBOT3 = 32;
COM_POS_OPP_A_ROBOT1 = 40;
COM_POS_OPP_A_ROBOT2 = 41;
COM_POS_OPP_A_ROBOT3 = 42;
COM_POS_OPP_A_ROBOT4 = 43;
COM_POS_OPP_B_ROBOT1 = 50;
COM_POS_OPP_B_ROBOT2 = 51;
COM_POS_OPP_B_ROBOT3 = 52;
COM_POS_OPP_B_ROBOT4 = 53;
COM_POS_OPP_C_ROBOT1 = 60;
COM_POS_OPP_C_ROBOT2 = 61;
COM_POS_OPP_C_ROBOT3 = 62;
COM_POS_OPP_C_ROBOT4 = 63;
COM_REFEREE;

usleep(50);
//RECEIVED VARIABLES FROM DECISION
A = DECISION_STATE;
B = DECISION_POSITION_A;
C = DECISION_POSITION_B;
D = DECISION_POSITION_C;
E = DECISION_BALL_POS;
F = DECISION_OPP1_POS;
G = DECISION_OPP2_POS;
H = DECISION_OPP3_POS;
}

#ifdef DEBUG
cout << "communication received " << A <<" "<< B <<" "<< C <<" "<< D <<" "<<E <<" "<< F <<" "<< G<<" "<< H << endl;
#endif

}
}
