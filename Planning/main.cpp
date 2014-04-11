/*--------------------------------------------------------------------

******************************************************************************
  * @file       Planning/main.cpp
  * @author     Thiago Pedro Donadon Homem - ROBOFEI-HT - FEI
  * @version    V0.0.0
  * @created    07/04/2014
  * @Modified   07/04/2014
  * @e-mail     thiago.homem@gmail.com
  * @brief      Planning
  ****************************************************************************

  Arquivo fonte contendo as funções de planejamento.

/--------------------------------------------------------------------*/


#include <iostream>
#include <unistd.h>
#include "../Include/planning.h"
#include "../Include/blackboard.h"

#define DEBUG

using namespace std;

int main()
{
       using_shared_memory(); //Função que cria e acopla a memória compartilhada
       planning plan;
       // READ VARIABLES
       plan.read_action = PLANNING_COMMAND;
       plan.read_velocity = PLANNING_PARAMETER_VEL;
       plan.read_angle = PLANNING_PARAMETER_ANGLE;
       while(1){
           cout<<plan.read_action<<"  "<<plan.read_velocity<<"  "<<plan.read_angle<<endl;
           switch (plan.read_action) {
                case 1:
                        plan.stop();
                        break;
                case 2:
                        plan.turn_left();
                        break;
                case 3:
                        plan.turn_right();
                        break;
           }
       sleep(1)    ;
       }
    return 0;
}
