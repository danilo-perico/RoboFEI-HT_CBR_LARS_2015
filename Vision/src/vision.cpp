/*--------------------------------------------------------------------

******************************************************************************
  * @file       vision.cpp
  * @author     Claudio Vilao - ROBOFEI-HT - FEI
  * @version    V0.0.1
  * @created    24/04/2014
  * @Modified   24/04/2014
  * @e-mail     
  * @brief      Vision
  ****************************************************************************
/--------------------------------------------------------------------*/

#include <iostream>
#include "blackboard.h"
#include <unistd.h>

#define DEBUG

int main(void)
{
int A, B, C, D, E, F, G, H, I, J;
using namespace std;

using_shared_memory();

#ifdef DEBUG
cout << "Vision" << endl;
#endif

while(1)
{
for (int i = 0;i<10000;i++)
{
//SENT VARIABLES TO DECISION
VISION_DIST_BALL = 9.99;
VISION_DIST_GOAL = 9.88;
VISION_DIST_OPP1 = 10.11;
VISION_DIST_OPP2 = 10.22;
VISION_DIST_OPP3 = 10.33;

usleep(50);
//RECEIVED VARIABLES FROM CONTROL
A = CONTROL_HEIGHT_A;
B = CONTROL_HEIGHT_B;
C = CONTROL_HEIGHT_C;

}

#ifdef DEBUG
cout << "vision received" << A <<" "<< B <<" "<< C << endl;
#endif

}

}
