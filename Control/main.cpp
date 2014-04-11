#include <iostream>
#include <stdio.h>
#include <communication.h>
#include <blackboard.h>
#include<pthread.h>
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
    CONTROL_ACTION = 1;
    sleep(1);


}
    return 0;
}
