#include <iostream>
#include <stdio.h>
#include <blackboard.h>
#include <unistd.h>

//#define DEBUG
//#define _cpluplus

//#ifdef _cpluplus
//teste
//#endif

using namespace std;

int main()
{
using_shared_memory();

//#ifdef DEBUG
cout << "Control running"<<endl;
//#endif
while(1)
{

    cout << "Command received = "<< PLANNING_COMMAND << " | Velocidade =  "<<
    PLANNING_PARAMETER_VEL << " | Angle = "<< PLANNING_PARAMETER_ANGLE<<endl;
    CONTROL_ACTION = PLANNING_COMMAND;
    usleep(300000);

    cout<< "CONTROL_ACTION = "<<CONTROL_ACTION<<endl<<endl;

usleep(50);
CONTROL_HEIGHT_A = 100;
CONTROL_HEIGHT_B = 200;
CONTROL_HEIGHT_C = 300;

}
    return 0;
}
