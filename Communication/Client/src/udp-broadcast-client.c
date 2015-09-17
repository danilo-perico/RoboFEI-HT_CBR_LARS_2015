//verifique a classe de IP da sua rede na qual será lida e publicada a informação em broadcast

/* udp-broadcast-client.c 
  * udp datagram client 
  * Get datagram stock market quotes from UDP broadcast: 
  * see below the step by step explanation 
  */  
  #include <stdio.h>  
  #include <unistd.h>  
  #include <stdlib.h>  
  #include <errno.h>  
  #include <string.h>  
  #include <time.h>  
  #include <signal.h>  
  #include <sys/types.h>  
  #include <sys/socket.h>  
  #include <netinet/in.h>  
  #include <arpa/inet.h>  
  #include "blackboard.h"
  
  #ifndef TRUE  
  #define TRUE 1  
  #define FALSE 0  
  #endif  
  
  extern int mkaddr(  
                    void *addr,  
                    int *addrlen,  
                    char *str_addr,  
                    char *protocol);  
  
     static struct {  
      int robotNumber;  
      float ballDistance;
     }info;  

//erro ao compilar. Adicionei todo o arquivo Blackboard.cpp aqui para conseguir! Verificar!!!

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "blackboard.h"

//#define DEBUG


#define KEY 123

int *mem ; //Variável que manipula memória compartilhada
float *memf ; //Variável que manipula memória compartilhada

//Depois de criado a memória compartilhada, para verificar se ela realmente foi criada
// e quantos processos estão utilizando, digite no terminal o comando $ipcs -m
// será a memoria criada ->   key = 0x0000007b    bytes = 2048
// nattch = number of attached processes

int using_shared_memory()
{
    // --- Variaveis usada para memoria compartilhada -----
    int shmid ; // identificador da memoria comum //
    const unsigned short int size = 2048; // tamanho da memória em Bytes
    int flag = 0;
    //-----------------------------------------------------

     // Recuperando ou criando uma memoria compartilhada-------------------
     //

     //shmget:para criar um segmento de memória compartilhada
     if (( shmid = shmget((key_t)KEY, size,0)) == -1)
     {
          perror("Erro no shmget") ;
          printf("\nMemória será criada\n");
         //return(1) ;
        if (( shmid = shmget((key_t)KEY, size, IPC_CREAT|IPC_EXCL|SHM_R|SHM_W)) == -1)
        {
            perror("Erro no shmget") ;
            return(1) ;
        }

     }
#ifdef DEBUG
     printf("Sou o processo com pid: %d \n",getpid()) ;
     printf("Identificador do segmento recuperado: %d \n",shmid) ;
     printf("Este segmento e associado a chave unica: %d\n",(key_t)KEY);
#endif
    //
    // acoplamento do processo a zona de memoria
    //recuperacao do pornteiro sobre a area de memoria comum
    //
    //shmat:retorna um pointeiro para o segmento de memória compartilhada
     if ((mem = (int*)shmat (shmid, 0, flag)) == (int*)-1){
          perror("acoplamento impossivel") ;
          return (2) ;
     }

     memf = (float*)(mem+125);
     //---------------------------------------------------------------------

            /* destruicao do segmento */
            //if ((shmctl(shmid, IPC_RMID, NULL)) == -1){
            // perror("Erro shmctl()");
             // return(1) ;
            //}
    return(0);
}




/* 
 * This function reports the error and 
 * exits back to the shell: 
 */  
 static void  
 displayError(const char *on_what) {  
     fputs(strerror(errno),stderr);  
     fputs(": ",stderr);  
     fputs(on_what,stderr);  
     fputc('\n',stderr);  
     return 0;  
}  
  
int main(int argc,char **argv) {  
     using_shared_memory();
     int z;  
     int x;  
     struct sockaddr_in adr;  /* AF_INET */  
     int len_inet;            /* length */  
     int s;                   /* Socket */  
     char dgram[512];         /* Recv buffer */  
     static int so_reuseaddr = TRUE;  
     static char  
     *bc_addr = "127.255.255.2:9097";  
  

     /* Broadcast address: */  
        bc_addr = "255.255.255.255:3939";  
  
    /* 
     * Create a UDP socket to use: 
     */  
     s = socket(AF_INET,SOCK_DGRAM,0);  
     if ( s == -1 )  
        displayError("socket()");  
  
    /* 
     * Form the broadcast address: 
     */  
     len_inet = sizeof adr;  
  
     z = mkaddr(&adr,  
                &len_inet,  
                bc_addr,  
                "udp");  
  
     if ( z == -1 )  
        displayError("Bad broadcast address");  
  
    /* 
     * Allow multiple listeners on the 
     * broadcast address: 
     */  
     z = setsockopt(s,  
                    SOL_SOCKET,  
                    SO_REUSEADDR,  
                    &so_reuseaddr,  
                    sizeof so_reuseaddr);  
  
     if ( z == -1 )  
        displayError("setsockopt(SO_REUSEADDR)");  
  
    /* 
     * Bind our socket to the broadcast address: 
     */  
     z = bind(s,  
             (struct sockaddr *)&adr,  
             len_inet);  
  
     if ( z == -1 )  
        displayError("bind(2)");  
  
     for (;;) {  
        /* 
         * Wait for a broadcast message: 
         */  
         z = recvfrom(s,      /* Socket */  
                      dgram,  /* Receiving buffer */  
                      sizeof dgram,/* Max rcv buf size */  
                      0,      /* Flags: no options */  
                      (struct sockaddr *)&adr, /* Addr */  
                      &x);    /* Addr len, in & out */  
  
         if ( z < 0 )  
            displayError("recvfrom(2)"); /* else err */  

	char *robot;
	char *token;
	char *search = " ";


	// Token will point to "SEVERAL".
	robot = strtok(dgram, search);
	printf("Robot: %s", robot);
	token = strtok(NULL, search);
	printf(" | Ball Position: %s", token);
	printf("\n");

	if (strcmp(robot,"1")==0)
		COM_POS_BALL_ROBOT1 = atoi(token);
	else
	if (strcmp(robot,"2")==0)
		COM_POS_BALL_ROBOT2 = atoi(token);
	else
	if (strcmp(robot,"3")==0)
		COM_POS_BALL_ROBOT3 = atoi(token);

//	printf("\nR1 %f, R2 %f, R3 %f\n",COM_POS_BALL_ROBOT1, COM_POS_BALL_ROBOT2, COM_POS_BALL_ROBOT3);

//         fwrite(dgram,255,1,stdout);  
//         putchar('\n');  
  
//         fflush(stdout);  
     }  
  
     return 0;  
 }  
