#include <stdio.h>
#include <termio.h>
#include <unistd.h>
#include <dynamixel.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <cvaux.h>
#include <highgui.h>
#include <cxcore.h>
#include <time.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <ctype.h>
#include <iostream>

#define RESOLUCAO_X 640
#define RESOLUCAO_Y 480


void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);


//////global variables////////////////////////////////////////////////
int inclina = 55;
unsigned int StandupPos[22] = {304, 594, //01 , 02, (cabeça)
                478, 304, 409, //03, 04, 05, (braço direito)
                546, 650, 609, //06, 07, 08, (braço esquerdo)
                573, 835, //09, 10, (torso)
                613, 534, 501, 508, 492, 559, //11, 12, 13, 14, 15, 16 (perna direita)
                590, 509, 514, 506, 510, 461}; //17, 18, 19, 20, 21, 22 (perna esquerda)

unsigned int StandupPos_corrigido[22] = {304, 594, //01 , 02, (cabeça)
                            478, 274, 350, //03, 04, 05, (braço direito)
                            541, 675, 682, //06, 07, 08, (braço esquerdo)
                            571, 835, //09, 10, (torso)
                            585, 512, 498, 510, 498, 562, //11, 12, 13, 14, 15, 16 (perna direita)
                            601, 512, 512, 510, 508, 467}; //17, 18, 19, 20, 21, 22 (perna esquerda)

int main(int argc, char* argv[])
{
     int i;
	 int baudnum = DEFAULT_BAUDNUM; //velocidade de transmissao
	 int index = 0;
	 int deviceIndex = 0; //endereça USB
	 int Moving, PresentPos;
	 int CommStatus;
     char string[50]; //String usada para definir prioridade do programa
     int retorno_visao = 0;

    system("echo 123456 | sudo -S chmod 777 /dev/ttyUSB*");//libera USB

    sprintf(string,"echo 123456 | sudo -S renice -20 -p %d", getpid()); // prioridade maxima do codigo
    system(string);//prioridade
	printf( "\nMiltonBot running...\n\n" );
	// ---- Open USB2Dynamixel -----------------------------------------------{
	if( dxl_initialize(deviceIndex, baudnum) == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		printf( "Press Enter key to terminate...\n" );
		getchar();
		return 0;
	}
	else
		printf( "Succeed to open USB2Dynamixel!\n" );
//-----------------------------------------------------------------------------}


robo_ereto_corrigido();
	while(1)
	{
	//
//	printf( "Press Enter key to continue!(press ESC and Enter to quit)\n" );
	//	if(getchar() == 0x1b)
	//		break;

//robo_ereto_corrigido();
//sleep(1);
//andar_rapido(3);
//andar_lateral_direita(1);
//andar_lateral_esquerda(1);
//virar(5);
//andar(2);
//andar_curto(3);
//levantar_de_costas();
sleep(5);

//chutepedireito();
//levantar_de_barriga();

retorno_visao = visao();// Entra no programa da visão e recebe retorno se achou a bola


if (retorno_visao == 1) // Se retornou 1, então o robo encontrou a bola e está com a câmera centralizado nela
{
printf("distancia: %2.2f |", distancia_bola());
printf("  virou %d",594-dxl_read_word( 2, P_PRESENT_POSITION_L));
printf("  Servo1: %d - Servo2: %d\n",dxl_read_word( 1, P_PRESENT_POSITION_L), dxl_read_word( 2, P_PRESENT_POSITION_L));

   float dist_bola = distancia_bola(); // Calcula a distância que o robo está da bola
        if (dist_bola > 0.05 && ((594-dxl_read_word( 2, P_PRESENT_POSITION_L))/100) < 1)
        {
            //virar((594-dxl_read_word( 2, P_PRESENT_POSITION_L))/100);//100

            if(dist_bola > 0.20)
                andar(dist_bola/0.095);//0.95

            if(dist_bola <= 0.20)
                andar_curto(dist_bola/0.04);
            //robo_ereto_corrigido();

        }
        else
           if(dist_bola > 0.05)
              virar((594-dxl_read_word( 2, P_PRESENT_POSITION_L))/100);

        //posição exata de chute servo 1: 537 / servo 2: 627
        if ((dxl_read_word( 1, P_PRESENT_POSITION_L) >= 540 && dxl_read_word( 1, P_PRESENT_POSITION_L) <= 578)
             && (dxl_read_word( 2, P_PRESENT_POSITION_L) >= 518 && (dxl_read_word( 2, P_PRESENT_POSITION_L) <= 560)))
        {
            chutepedireito();
        }

        if ((dxl_read_word( 1, P_PRESENT_POSITION_L) >= 540 && dxl_read_word( 1, P_PRESENT_POSITION_L) <= 577)
             && (dxl_read_word( 2, P_PRESENT_POSITION_L) < 518)) // verifica se necessita andar lateral
        {
            andar_lateral_direita(1);
        }

        if ((dxl_read_word( 1, P_PRESENT_POSITION_L) >= 540 && dxl_read_word( 1, P_PRESENT_POSITION_L) <= 577)
             && (dxl_read_word( 2, P_PRESENT_POSITION_L) > 560)) // verifica se necessita andar lateral
        {
            andar_lateral_esquerda(1);
        }

        if ((dxl_read_word( 2, P_PRESENT_POSITION_L) >= 518 && (dxl_read_word( 2, P_PRESENT_POSITION_L) <= 560))
            && dist_bola<=0.05 && (dxl_read_word( 1, P_PRESENT_POSITION_L) < 540))
        {
            andar_curto(1);
        }


}

if (retorno_visao == 2)
{
    printf("Não achou bola");
    dxl_write_word(2, P_GOAL_POSITION_L, 594);
    dxl_write_word(1, P_GOAL_POSITION_L, 304);
    //for (int i = 0; i<3 ; i++)
    virar(3);
}


//if (visao() == 2)
//printf("Não achou bola");
//robo_ereto();
//usleep(1000000);
//robo_ereto_corrigido();
//usleep(3000000);
//andar();
//virar();
//chutepedireito_fitPC();
//inclinar();
//levanta_abaixa_perna_esq();
//levantar_de_barriga();

    }
	// Close device
	dxl_terminate();
	printf( "Press Enter key to terminate...\n" );
	getchar();
	return 0;
}
//=================================================================================================================

//-----------------------------------------------------------------------------------------------------------------
// Print communication result
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
	case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now recieving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		printf("This is unknown error code!\n");
		break;
	}
}

// Print error bit of status packet
void PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
		printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
		printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
		printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
		printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
		printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
		printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
		printf("Instruction code error!\n");
}
