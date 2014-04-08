#include <iostream>
#include <communication.h>
#include "/home/perico/RoboFEI-HT/blackboard.h"

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

using namespace std;

#ifdef DEBUG
cout << "Communication" << endl;
#endif

using_shared_memory();

COM_ACTION_ROBOT1 = 1; //walking
COM_ACTION_ROBOT2 = 0; //stationary
COM_ACTION_ROBOT3 = 1; //walking


}
