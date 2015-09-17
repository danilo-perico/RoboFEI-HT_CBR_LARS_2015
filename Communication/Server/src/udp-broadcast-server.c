//verifique a classe de IP da sua rede na qual será lida e publicada a informação em broadcast

    /* udp-broadcast-server.c: 
     * udp broadcast server example  
     * Example Stock Index Broadcast: 
     */  
     #include <stdio.h>  
     #include <unistd.h>  
     #include <stdlib.h>  
     #include <errno.h>  
     #include <string.h>  
     #include <time.h>  
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
      
     #define MAXQ 4  
      
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





     static struct {  
      char *robotName;  
      float ballDistance;
     }info;  

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
      exit(1);  
     }  



int main(int argc,char **argv) {  
      using_shared_memory();
      short x;    /* index of Stock Indexes */  
      double I0;  /* Initial index value */  
      double I;   /* Index value */  
      char bcbuf[512], *bp;/* Buffer and ptr */  
      int z;      /* Status return code */  
      int s;      /* Socket */  
      struct sockaddr_in adr_srvr;/* AF_INET */  
      int len_srvr;               /* length */  
      struct sockaddr_in adr_bc;  /* AF_INET */  
      int len_bc;                 /* length */  
      static int so_broadcast = TRUE;  
      static char  
      *sv_addr = "127.0.0:*",  
      *bc_addr = "127.255.255.2 9097";  
       
        /* 
      * Form a server address: 
      */  
      if ( argc > 2 )  
      /* Server address: */  
      sv_addr = argv[2];  
       

	bc_addr = "255.255.255.255:3939";
       
     /* 
      * Form the server address: 
      */  
      len_srvr = sizeof adr_srvr;  
      
      z = mkaddr(  
              &adr_srvr,  /* Returned address */  
              &len_srvr,  /* Returned length */  
              sv_addr,    /* Input string addr */  
              "udp");     /* UDP protocol */  
      
      if ( z == -1 )  
        displayError("Bad server address");  
       
     /* 
      * Form the broadcast address: 
      */  
      len_bc = sizeof adr_bc;  
       
      z = mkaddr(  
          &adr_bc, /* Returned address */  
          &len_bc, /* Returned length */  
          bc_addr, /* Input string addr */  
          "udp"); /* UDP protocol */  
       
      if ( z == -1 )  
        displayError("Bad broadcast address");  
       
     /* 
      * Create a UDP socket to use: 
      */  
      s = socket(AF_INET,SOCK_DGRAM,0);  
      if ( s == -1 )  
        displayError("socket()");  
       
     /* 
      * Allow broadcasts: 
      */  
      z = setsockopt(s,  
                     SOL_SOCKET,  
                     SO_BROADCAST,  
                     &so_broadcast,  
                     sizeof so_broadcast);  
       
      if ( z == -1 )  
        displayError("setsockopt(SO_BROADCAST)");  
      
     /* 
      * Bind an address to our socket, so that 
      * client programs can listen to this 
      * server: 
      */  
      z = bind(s,  
              (struct sockaddr *)&adr_srvr,  
              len_srvr);  
       
      if ( z == -1 )  
        displayError("bind()");  
       
      
      for (;;) {  
         /* 
          * Form a packet to send out: 
          */  
          bp = bcbuf;  
	  //info.robotName = ROBOT_NUMBER;
	  if (ROBOT_NUMBER == 1)
		info.robotName = "1";
	  if (ROBOT_NUMBER == 2)
		info.robotName = "2";
	  if (ROBOT_NUMBER == 3)
		info.robotName = "3";
	  //info.robotName = "2";
	  info.ballDistance = 3.1415;
	  sprintf(bp,"%s %f ",info.robotName,info.ballDistance);  
          //bp += strlen(bp);  
      
         /* 
          * Broadcast the updated info: 
          */  
          z = sendto(s,  
                     bcbuf,  
                     strlen(bcbuf),  
                     0,  
                    (struct sockaddr *)&adr_bc,  
                    len_bc);   
           
          if ( z == -1 )  
            displayError("sendto()");  
      
          sleep(4);  
      }  
       
      return 0;  
     }  
