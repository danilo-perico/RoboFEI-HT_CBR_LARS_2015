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
       plan.read_actionA= DECISION_ACTION_A;
       plan.read_actionB = DECISION_ACTION_B;
       while(1){
           cout<<plan.read_actionA<<"  "<<plan.read_actionB<<endl;
           switch (plan.read_actionA) {
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
