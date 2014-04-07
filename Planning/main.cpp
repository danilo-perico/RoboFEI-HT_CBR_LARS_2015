#include <iostream>
#include "planning.h"
#include <errno.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define KEY 123

//Backboard definitions
#define TYPE_FUNCTION *(mem+1)// 0 - Nenhuma função
                               // 1 - Função Andar rápido
                               // 2 - Função Andar curto
                               // 3 - Função Virar
                               // 4 - Função Andar lateral direita
                               // 5 - Função Andar lateral esquerda
                               // 6 - Chute pé direito
#define planning_command *(mem+2)
#define planning_parameter_vel *(mem+3)
#define planning_parameter_angle *(mem+4)
#define planning_parameter_r *(mem+5)

#define decision_action_a *(mem+6)
#define decision_action_b *(mem+7)

using namespace std;

int *mem ; //Variável que manipula memória compartilhada

int main()
{
    // --- Variaveis usada para memoria compartilhada -----
    int shmid ;  // identificador da memoria comum //
    int size = 1024 ;
    char *path="nome_de_arquivo_existente" ;
    int flag = 0;

    //
    // Recuperando ou criando uma memoria compartilhada-------------------
    //
     if (( shmid = shmget(ftok(path,(key_t)KEY), size,0)) == -1)
     {
          perror("Erro no shmget") ;
          printf("\nMemória será criada\n");
         //return(1) ;
        if (( shmid = shmget(ftok(path,(key_t)KEY), size, IPC_CREAT|IPC_EXCL|SHM_R|SHM_W)) == -1)
        {
            perror("Erro no shmget") ;
            return(1) ;
        }

     }

     printf("Sou o processo com pid: %d \n",getpid()) ;
     printf("Identificador do segmento recuperado: %d \n",shmid) ;
     printf("Este segmento e associado a chave unica: %d\n",ftok(path,(key_t)KEY)) ;
    //
    // acoplamento do processo a zona de memoria
    //recuperacao do pornteiro sobre a area de memoria comum
    //
     if ((mem = (int*)shmat (shmid, 0, flag)) == (int*)-1){
          perror("acoplamento impossivel") ;
          return (1) ;
     }
    *mem = 0;

    while(1){
       //teste de leitura de memória
       cout << planning_command << endl;
       cout << planning_parameter_vel << endl;
       cout << planning_parameter_angle << endl;
       cout << planning_parameter_r << endl;

       cin >> decision_action_a;
       cin >> decision_action_b;

    }

    return 0;
}
