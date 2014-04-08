/*--------------------------------------------------------------------

******************************************************************************
  * @file       Management/main.cpp
  * @author     Thiago Pedro Donadon Homem - ROBOFEI-HT - FEI
  * @version    V0.0.0
  * @created    07/04/2014
  * @Modified   07/04/2014
  * @e-mail     thiago.homem@gmail.com
  * @brief      Management
  ****************************************************************************

  Arquivo fonte contendo as inicializações dos processos.

/--------------------------------------------------------------------*/


#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include "../blackboard.h"

using namespace std;

int spawn (char* program, char** arg_list)
{
  pid_t child_pid;
  /* Duplicate this process. */
  child_pid = fork ();
  if (child_pid != 0)
       /* This is the parent process. */
      return child_pid;
  else {
       /* Now execute PROGRAM, searching for it in the path. */
       execvp (program, arg_list);
       /* The execvp function returns only if an error occurs. */
       fprintf (stderr, "an error occurred in execvp\n");
       abort ();
   }
}

int main()
{
    using_shared_memory(); //Função que cria e acopla a memória compartilhada

    char* arg_list[] = {
    NULL
    };
   // spawn ("/home/thiago/GitHub/RoboFEI-HT/Vision/bin/Debug/vision", arg_list);
   // spawn ("/home/thiago/GitHub/RoboFEI-HT/Decision/bin/Debug/decision", arg_list);
    spawn ("/home/thiago/GitHub/RoboFEI-HT/Planning/bin/Debug/planning",arg_list);
   // spawn ("/home/thiago/GitHub/RoboFEI-HT/Control/bin/Debug/control", arg_list);
   // spawn ("/home/thiago/GitHub/RoboFEI-HT/Localization/bin/Debug/localization", arg_list);
   // spawn ("/home/thiago/GitHub/RoboFEI-HT/Communication/bin/Debug/communication", arg_list);
    return 0;
}
