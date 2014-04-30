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
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#include "blackboard.h"


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
    char *path;
    char path1[100];   //vision
    char path2[100];   //decision
    char path3[100];   //planning
    char path4[100];   //control
    char path5[100];   //localization
    char path6[100];   //communication

    //captura e converte o caminho absoluto para qualquer usuário
    //ja tentamos concatenar, mas dá erro de tipo misturado!
    //path = getenv("HOME");
    sprintf(path1,"control");
    sprintf(path2,"decision");
    sprintf(path3,"communication");
    sprintf(path4,"vision");
    sprintf(path5,"localization");
    sprintf(path6,"planning");

    //sprintf(path2,"%s/RoboFEI-HT/Decision/bin/Debug/decision",path);
    //sprintf(path3,"%s/RoboFEI-HT/Planning/bin/Debug/planning",path);
    //sprintf(path4,"%s/RoboFEI-HT/Control/bin/Debug/Control",path);
    //sprintf(path5,"%s/RoboFEI-HT/Localization/bin/Debug/localization",path);
    //sprintf(path6,"%s/RoboFEI-HT/Communication/bin/Debug/communication",path);

    using_shared_memory(); //Função que cria e acopla a memória compartilhada

    char* arg_list[] = {
    NULL
    };


    //inicializa os processos;
    //spawn (path1,arg_list);
    spawn (path1,arg_list);
    spawn (path2,arg_list);
    spawn (path3,arg_list);
    spawn (path4,arg_list);
    spawn (path5,arg_list);
    spawn (path6,arg_list);

    //spawn (path3,arg_list);
    //spawn (path4,arg_list);
    //spawn (path5,arg_list);
    //spawn (path6,arg_list);

    return 0;
}
