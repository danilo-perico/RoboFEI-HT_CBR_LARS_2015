/*--------------------------------------------------------------------

******************************************************************************
  * @file       decision.cpp
  * @author     Danilo Hernnai Perico - ROBOFEI-HT - FEI
  * @version    V0.0.1
  * @created    08/04/2014
  * @Modified   10/04/2014
  * @e-mail     danilo.perico@gmail.com
  * @brief      Decision
  ****************************************************************************
/--------------------------------------------------------------------*/

#include <iostream>
#include <blackboard.h>
//#include<pthread.h>
#include<unistd.h>

#define DEBUG

int main(void)
{
using namespace std;
int a = 0, b = 0, c = 0;
int d, e, f, g, h, i, j, k, l , m, n, o, p, q, r, s, t, u, v, x, z, aa, ab, ac, ad, ae, af, ag, ah, ai, aj, inc = 0;

using_shared_memory();

#ifdef DEBUG
cout << "Decision" << endl;
#endif

while(1)
{


for (int ctrl=0; ctrl<10000; ctrl++)
{
//RECEIVED VARIABLES
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
}
}
