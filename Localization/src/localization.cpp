/*--------------------------------------------------------------------

******************************************************************************
  * @file       localization.cpp
  * @author     Claudio Vilao - ROBOFEI-HT - FEI
  * @version    V0.0.1
  * @created    24/04/2014
  * @Modified   24/04/2014
  * @e-mail     
  * @brief      Localization
  ****************************************************************************
/--------------------------------------------------------------------*/


#include <iostream>
#include "blackboard.h"
#include <unistd.h>

#define DEBUG


int main(void)
{
float A, B, C, D, E, F, G, H, I, J;
using namespace std;

using_shared_memory();

#ifdef DEBUG
cout << "Localization" << endl;
#endif

while(1)
{
for (int i = 0;i<10000;i++)
{
//SENT VARIABLES TO DECISION
LOCALIZATION_THETA = 6000;
LOCALIZATION_X = 7000;
LOCALIZATION_Y = 8000;

usleep(50);
//RECEIVED VARIABLES FROM DECISION
A = VISION_DIST_BALL;
B = VISION_DIST_GOAL;
C = VISION_DIST_OPP1;
D = VISION_DIST_OPP2;
E = VISION_DIST_OPP3;

}

#ifdef DEBUG
cout << "localization received " << A <<" "<< B <<" "<< C << " " << D << " " << E << endl;
#endif

}

}
