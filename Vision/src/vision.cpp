/*--------------------------------------------------------------------

******************************************************************************
  * @file       vision.cpp
  * @author     Claudio Vilao - ROBOFEI-HT - FEI
  * @version    V0.0.1
  * @created    24/04/2014
  * @Modified   24/04/2014
  * @e-mail
  * @brief      Vision
  ****************************************************************************
/--------------------------------------------------------------------*/

#include <iostream>
#include "blackboard.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
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
#include <string>
#include <dynamixel.h>
#include <unistd.h>

#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING		46
#define DEFAULT_BAUDNUM		1 // 1Mbps

//#define DEBUG
#define XC .038 //
#define YC .05 //Altura da bola em relação ao servo

// Tamanho Padrão de Captura WebCam Robo- 640x480
#define RESOLUCAO_X 640
#define RESOLUCAO_Y 480
void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);
void captura_pixel_hsv();
int head_follow_ball(float posx, float posy, bool *head_move1, bool *head_move2);
void search_ball(bool inicio);

int cont_search_ball;

CvCapture* captura;
CvMemStorage* memoria;

int head_follow_ball(float, float, bool*, bool*);

void search_ball(bool);

int main()
{

    bool head_move2 = false;
    bool head_move1 = false;
    int i;
    int baudnum = DEFAULT_BAUDNUM; //velocidade de transmissao
    int index = 0;
    int deviceIndex = 0; //endereça USB
    int Moving, PresentPos;
    unsigned int lost_ball = 0; // Conta quantos frames a bola está perdida
    int saida = 0;
    int CommStatus;

    using_shared_memory();

  system("echo | sudo chmod 777 /dev/ttyUSB*");//libera USB

if( dxl_initialize(deviceIndex, baudnum) == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		printf( "Press Enter key to terminate...\n" );
		getchar();
		return 0;
	}
	else
		printf( "Succeed to open USB2Dynamixel!\n" );

    CvFont font;
    double hScale=1.0;
    double vScale=1.0;
    int    lineWidth=1;
    int posX = 1;
    int posY = 1;
    bool inicio=1;
    float alpha;
    float dist;

   cont_search_ball = 0;



cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);

	// Tamanho Padrão de Captura WebCam Robo- 640x480
	CvSize tamanho = cvSize(RESOLUCAO_X,RESOLUCAO_Y); //320 240

    	// Abre o dispositivo de Captura "0" que é /dev/video0
 	captura = cvCaptureFromCAM( 0 );
//  	CvCapture* captura1 = cvCaptureFromCAM( 1 );

	if( !captura )
		{
	        fprintf( stderr, "ERRO Não há Captura na Camera 0/n" );
	        getchar();
	        return -1;
        }


	cvNamedWindow( "Video1", CV_WINDOW_AUTOSIZE );

	CvScalar min = cvScalar(5, 160, 50, 0);//4 160 //160
	CvScalar max = cvScalar(20, 250, 255, 0);//11 //250

//r 80~110
//g 130~150
//b 40~70


    usleep(1000000);

//*****************************************************************************

	IplImage*  Quadro_hsv  = cvCreateImage(tamanho, IPL_DEPTH_8U, 3);
	IplImage*  segmentada  = cvCreateImage(tamanho, IPL_DEPTH_8U, 1);

    while( 1 )
 	{

        // Pegar um quadro
        IplImage* Quadro = cvQueryFrame( captura );


        if( !Quadro )
        {
                fprintf( stderr, "ERRO Quadro vazio...\n" );
                getchar();
                break;
        }

       	// Converter o espaço de cores para o HSV para ficar mais fácil filtrar as cores.
        cvCvtColor(Quadro, Quadro_hsv, CV_BGR2HSV);

        // Filtrar cores que não interessam.
        cvInRangeS(Quadro_hsv, min, max, segmentada);

        // Memoria para os circulos de Hough
        memoria = cvCreateMemStorage(0);

        cvDilate(segmentada,segmentada,NULL,5);
        cvErode(segmentada,segmentada,NULL,5);
        cvSmooth( segmentada, segmentada, CV_GAUSSIAN, 9, 9 );
        cvSmooth( segmentada, segmentada, CV_GAUSSIAN, 9, 9 );
        CvSeq* circulos = cvHoughCircles(segmentada, memoria, CV_HOUGH_GRADIENT, 2, segmentada->height/4, 100, 50, 5, 70);// 200 100 10 400
                                                                                                                            //vilão 100 50 10 400
        std::string str;

        for (int i = 0; i < circulos->total; i++)
        {
            float* p = (float*)cvGetSeqElem( circulos, i );



            CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
            cvMoments(segmentada, moments, 1);

            double area = cvGetCentralMoment(moments, 0, 0);



            posX = p[0]; // variavel que contem a posição da bola em X da tela
            posY = p[1]; // variavel que contem a posição da bola em Y da tela

            cvCircle( Quadro, cvPoint(cvRound(posX),cvRound(posY)), 3, CV_RGB(0,255,0), -1, 8, 0 );
            cvCircle( Quadro, cvPoint(cvRound(posX),cvRound(posY)), cvRound(p[2]), CV_RGB(255,255,255), 2, 8, 0 );




            lost_ball=0;
            cont_search_ball = 0;

            //---- Escreve na Tela a posição X da bola ---------------------------------------
            std::stringstream X_str ;
            X_str << p[0];                 // valor de X detectado pelo circulo de Hough
            std::string x = X_str.str();
            const char * cx = x.c_str();
            cvPutText (Quadro, cx ,cvPoint(100,300), &font, cvScalar(255,255,0));

            //---- Escrve a diferença entre a posição X da bola e o centro da tela------------
            std::stringstream posiX_str ;
            posiX_str << posX - RESOLUCAO_X/2; // diferença entre a bola e o centro da tela em x
            std::string posix = posiX_str.str();
            const char * posx = posix.c_str();
            cvPutText (Quadro, posx ,cvPoint(100,400), &font, cvScalar(255,255,0));

            char cD[10];
            sprintf(cD, "DifY %d", (posY - RESOLUCAO_Y/2));
            cvPutText (Quadro, cD ,cvPoint(100,450), &font, cvScalar(255,255,0));

            //---- Escreve na Tela a posição Y da bola ---------------------------------------
            std::stringstream Y_str ;
            Y_str << p[1];                // valor de Y detectado pelo circulo de Hough
            std::string y = Y_str.str();
            const char * cy = y.c_str();
            cvPutText (Quadro, cy ,cvPoint(200,300), &font, cvScalar(255,255,0));

            //---- Escreve na Tela o raio de pixels da bola ------------------------------------
            std::stringstream R_str ;
            R_str << p[2];                // valor de R detectado pelo circulo de Hough
            std::string r = R_str.str();
            const char * cr = r.c_str();
            cvPutText (Quadro, cr ,cvPoint(300,300), &font, cvScalar(255,255,0));


            std::stringstream A_str ;
            A_str << area;                // valor da area em pixels
            std::string A = A_str.str();
            const char * cA= A.c_str();
            cvPutText (Quadro, cA ,cvPoint(300,400), &font, cvScalar(255,255,0));
       }


        if(lost_ball>20) //verifica se depois de 30 frames a bola está perdida no campo
        {
            //while(*comunica!=0);
            //if(*comunica)
		VISION_SEARCH_BALL = 1;
                search_ball(inicio);//Procura a bola pelo campo
                saida = 0;
            //char cD[10];
            //sprintf(cD, "procurando a bola");
            cvPutText (Quadro, "Procurando a bola" ,cvPoint(150,450), &font, cvScalar(255,255,0));
            //call_search = 1;
            inicio = 0;
        }
        else
        {
	VISION_SEARCH_BALL = 0;
            if(inicio==0)// Faz o robô parar a cabeça no instante que achou a bola que estava perdida
            {            // Sem esse comando o código não funciona porque ele não para a cabeça
                dxl_write_word(1, P_GOAL_POSITION_L, dxl_read_word( 1, P_PRESENT_POSITION_L));
                dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L));
            }

            inicio =1;

                if (head_follow_ball(posX, posY, &head_move1, &head_move2) == 1){// Move a cabeça para seguir a bola
               saida++;
                }

        }
        if(lost_ball<100)
            lost_ball++;


        cvShowImage( "Filtragem das cores", segmentada ); // O stream depois da filtragem de cores
        cvShowImage( "Video1", Quadro ); // Stream Original com a bola detectada
//	cvShowImage( "Video2", Quadro2 ); // Stream Original com a bola detectada
		cvReleaseMemStorage(&memoria);




     if( (cvWaitKey(10) & 255) == 27 || saida > 22 || cont_search_ball > 9 )
        {
     ///////////  cvReleaseCapture( &captura );
//      cvReleaseCapture( &captura1 );
     //   cvDestroyWindow( "Video1" );
    //    cvDestroyWindow( "Filtragem das cores" );
    //    cvDestroyWindow( "Sistema de cores RGB - HSV" );
        if (cont_search_ball > 9){
        //dxl_write_word(2, P_GOAL_POSITION_L, 594);
        //dxl_write_word(1, P_GOAL_POSITION_L, 304);
        
        std::cout << "bola não encontrada" << std::endl;
	VISION_LOST_BALL = 1;
	//return 2;
        }
        else
        {
        //printf("servo 1: %d",dxl_read_word( 1, P_PRESENT_POSITION_L));
	VISION_LOST_BALL = 0;
        alpha = ((531-dxl_read_word( 1, P_PRESENT_POSITION_L))*0.005111)+0.0349;  //0.0349
        dist = tan(alpha)*(cos(3.1415-alpha)*YC+.448-sin(1.5708-alpha)*XC)+(sin(3.1415-alpha)*YC)+cos(1.5708-alpha)*XC;
        
	VISION_DIST_BALL = dist;
	std::cout << VISION_DIST_BALL << std::endl;	 

	VISION_MOTOR_ANGLE = dxl_read_word( 2, P_PRESENT_POSITION_L);
	std::cout << VISION_MOTOR_ANGLE << std::endl;	

        }
        }
        
	}

return 0;
}

//=======================================================================================================================
//------------- Função que faz a cabeça centralizar a bola no vídeo------------------------------------------------------
int head_follow_ball(float posx, float posy, bool *head_move1, bool *head_move2)
{
    int pan = 0;
    int tilt = 0;
// Posição inicial da cabeça {304, 594} //01 , 02, cabeça

    dxl_write_word(1, MOVING_SPEED, 400);//300
    dxl_write_word(2, MOVING_SPEED, 400);//300

    //int key = kbhit();
    //if (key != 0)
    //{
      //  cvReleaseCapture( &captura );
      //  cvReleaseMemStorage(&memoria);

       // keyboard(key);
       // }


//------ Realiza o movimento do Pan -----------------------------------------------------------
    //------ Segue a bola para a esquerda do video -----------------------------------------
    if(posx<(RESOLUCAO_X/2)*0.95 && *head_move2==false)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L)+( ((RESOLUCAO_X/2)-posx) * 0.285));

        //head_move = true;
    }

    //------ Segue a bola para a direita do video -----------------------------------------
    if(posx>(RESOLUCAO_X/2)*1.05 && *head_move2==false)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L)-( (posx-(RESOLUCAO_X/2)) * 0.285));

        //dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L)-(((RESOLUCAO_X/2)-posx) * 0.30) );
        //head_move = true;
    }

    // Para a cabeça se chegou na posição desejada ----------------------------------------
    if(posx>=(RESOLUCAO_X/2)*0.95 && posx<=(RESOLUCAO_X/2)*1.05){
        dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L));
        pan = 1;

    }

    if(dxl_read_byte( 2, P_MOVING ))
        *head_move2 = true;  // verifica se a cabeça está em movimento
    else
        *head_move2 = false; // verifica se a cabeça está em movimento
//---------------------------------------------------------------------------------------------


//------ Realiza o movimento do Tilt -----------------------------------------------------------
    //------ Segue a bola para a esquerda do video -----------------------------------------
    if(posy<(RESOLUCAO_Y/2)*0.95 && *head_move1==false)
    {
        dxl_write_word(1, P_GOAL_POSITION_L, dxl_read_word( 1, P_PRESENT_POSITION_L)-( ((RESOLUCAO_Y/2)-posy) * 0.285));

        //head_move = true;
    }

    //------ Segue a bola para a direita do video -----------------------------------------
    if(posy>(RESOLUCAO_Y/2)*1.05 && *head_move1==false)
    {
       dxl_write_word(1, P_GOAL_POSITION_L, dxl_read_word( 1, P_PRESENT_POSITION_L)+( (posy-(RESOLUCAO_Y/2)) * 0.285));

        //dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L)-(((RESOLUCAO_X/2)-posx) * 0.30) );
        //head_move = true;
    }

    // Para a cabeça se chegou na posição desejada ----------------------------------------
    if(posy>=(RESOLUCAO_Y/2)*0.95 && posy<=(RESOLUCAO_Y/2)*1.05){
        dxl_write_word(1, P_GOAL_POSITION_L, dxl_read_word( 1, P_PRESENT_POSITION_L));

        tilt = 1;
    }

    if(dxl_read_byte( 1, P_MOVING ))
        *head_move1 = true;  // verifica se a cabeça está em movimento
    else
        *head_move1 = false; // verifica se a cabeça está em movimento
//---------------------------------------------------------------------------------------------
if (pan == 1 && tilt == 1)
return 1;
else
return 0;

}
//=======================================================================================================================


void search_ball(bool inicio)
{
    // Posição inicial da cabeça {304, 594} //01 , 02, cabeça
static unsigned int varredura=0;

        dxl_write_word(2, MOVING_SPEED, 150);//Seta as velocidades da cabeça
        dxl_write_word(1, MOVING_SPEED, 400);




   //     int key = kbhit();
   //     if (key != 0)
   //     {
  //      cvReleaseCapture( &captura );
  //      cvReleaseMemStorage(&memoria);

  //      keyboard(key);
  //      }



    if(inicio)
        varredura--; // continua a varredura de onde parou

    if(varredura>9||varredura<1)
        varredura=0;

    if(dxl_read_byte( 2, P_MOVING )==0){
     varredura++;
     cont_search_ball++;
    }


    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==8)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, 1000);
        dxl_write_word(1, P_GOAL_POSITION_L, 304);
    }

    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==7)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, 250);
        dxl_write_word(1, P_GOAL_POSITION_L, 304);
    }


    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==6)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, 250);
        dxl_write_word(1, P_GOAL_POSITION_L, 435);
    }


    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==5)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, 1000);
        dxl_write_word(1, P_GOAL_POSITION_L, 435);
    }


    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==4)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, 1000);
        dxl_write_word(1, P_GOAL_POSITION_L, 550);
    }


    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==3)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, 250);
        dxl_write_word(1, P_GOAL_POSITION_L, 550);
    }

    if(dxl_read_byte( 1, P_MOVING )==0 && varredura==2)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, 250);
        dxl_write_word(1, P_GOAL_POSITION_L, 304);
    }

    if(dxl_read_byte( 1, P_MOVING )==0 && varredura==1)
    {
        dxl_write_word(2, MOVING_SPEED, 700);
        dxl_write_word(1, MOVING_SPEED, 700);
        dxl_write_word(2, P_GOAL_POSITION_L, 594);
        dxl_write_word(1, P_GOAL_POSITION_L, 304);
        //sleep(3);
    }


    //dxl_write_word(2, MOVING_SPEED, 20);//300
    //dxl_write_word(10, P_GOAL_POSITION_L, );


}

/*
void captura_pixel_hsv()
{

	CvSize tamanho = cvSize(320,240);


	    IplImage*  Quadro_hsv  = cvCreateImage(tamanho, IPL_DEPTH_8U, 3);

        if( !Quadro_hsv )
        	{
                fprintf( stderr, "ERRO Quadro vazio...\n" );
                getchar();
        	}

        //ponteiro para as cores da imagem
        unsigned char* colorData = (unsigned char*)Quadro_hsv->imageData;

        printf("%3d  %3d  %3d", colorData[0], colorData[1], colorData[2]);

}*/




//VISION_DIST_BALL = 9.99;
//VISION_DIST_GOAL = 9.88;
//VISION_DIST_OPP1 = 10.11;
//VISION_DIST_OPP2 = 10.22;
//VISION_DIST_OPP3 = 10.33;

//usleep(50);
//RECEIVED VARIABLES FROM CONTROL
//A = CONTROL_HEIGHT_A;
//B = CONTROL_HEIGHT_B;
//C = CONTROL_HEIGHT_C;



#ifdef DEBUG
cout << "vision received" << A <<" "<< B <<" "<< C << endl;
#endif

//}

//}
