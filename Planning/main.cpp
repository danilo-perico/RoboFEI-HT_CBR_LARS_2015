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
#include "planning.h"
#include "../blackboard.h"


using namespace std;



int main()
{
     using_shared_memory(); //Função que cria e acopla a memória compartilhada

    while(1){
       //teste de leitura de memória
       cout << PLANNING_COMMAND << endl;
       cout << PLANNING_PARAMETER_VEL << endl;
       cout << PLANNING_PARAMETER_ANGLE << endl;
       DECISION_ACTION_A = 1;
       cout << DECISION_ACTION_A<<endl;
       cout << DECISION_ACTION_B<<endl;
       //sleep(1);
       //cin >> DECISION_ACTION_A;
       //cin >> DECISION_ACTION_B;
       //sleep(1);
    }

    return 0;
}
