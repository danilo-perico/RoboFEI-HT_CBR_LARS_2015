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
#define XC .038 //distancia da camera em relação ao servo
#define YC .05 //Altura da camera em relação ao servo

using namespace cv;
using namespace std;

// Tamanho Padrão de captura WebCam Robo- 640x480
#define RESOLUCAO_X 640 //1280   1920
#define RESOLUCAO_Y 480  //720   1080

#define CENTERBALL 0.05 // Porcentagem de quanto centraliza a bola no quadro
#define AJUSTE 0.285 // para resolução de 640x480
//#define AJUSTE 0.285*0.49 // para resolução de 1280x720
#define AJUSTEDIST 1 // para resolução de 640x480
//#define AJUSTEDIST 0.5 //para resolução de 1280x720

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);
void captura_pixel_hsv();
int head_follow_ball(float posx, float posy, bool *head_move1, bool *head_move2);
void search_ball(bool inicio);
float media_raio_bola = 0;
float soma_raio_bola = 0;
uint8_t cont_bola = 0;

//803, 512,
int pos_servo1 = 803;
int pos_servo2 = 512;

int cont_search_ball;

CvCapture* captura;
CvMemStorage* memoria;

int head_follow_ball(float, float, bool*, bool*);

void search_ball(bool);

int main()
{

    CvSeq* lines = 0;
    int cont = 0;
    bool head_move2 = false;
    bool head_move1 = false;
    int i;
    int baudnum = DEFAULT_BAUDNUM; //velocidade de transmissao
    int index = 0;
    int deviceIndex = 0; //endereça a USB
    int Moving, PresentPos;
    unsigned int lost_ball = 0; // Conta quantos frames a bola está perdida
    int saida = 0;
    int CommStatus;

    using_shared_memory();
//IMU_STATE=0;
  system("echo | sudo chmod 777 /dev/ttyUSB*");//libera USB

// ---- Open USBDynamixel -----------------------------------------------{
bool servoComunica = false;
	while(deviceIndex<3)// laço que percorre o servo 0, 1 e 2.
	{
		if( dxl_initialize(deviceIndex, baudnum) == 0 )
		{
			printf( "Failed to open servo%d!\n", deviceIndex );
			if(deviceIndex==2)  // Não encontrou nenhum
			{
				if(servoComunica)
				    printf("Conectou-se a uma placa mas não conseguiu se comunicar com o servo\n");
				else
				    printf("Não encontrou nenhuma placa do servo conectada a porta USB\n");
			        return 0;
			}
			deviceIndex++;      // Não conecta na placa do servo e tenta a proxima porta.
		}
		else
		{
			servoComunica = true;
			printf( "Succeed to open Servo%d!\n", deviceIndex );
    			if(dxl_read_byte( 1, 3 ) == 1)
			{
       			 	printf("Servo%d okay - Connected and communicated!\n", deviceIndex);
			 	break;
			}
    			else
    			{
				printf("Servo wrong or not communicated!\n");
				if(deviceIndex==2)
				{
					printf("Conectou-se a uma placa mas não conseguiu se comunicar com o servo\n");
					return 0;
				}
				deviceIndex++;
			}
		}
	}
//-----------------------------------------------------------------------------}

    CvFont font;
    double hScale=1.0;
    double vScale=1.0;
    int    lineWidth=1;
    int posX = 1;
    int posY = 1;
    float raio_bola = 0;
    bool inicio=1;
    float alpha;
    float dist;
    bool print_tela = 0;

   cont_search_ball = 0;

        dxl_write_word(1, 34, 512); // Usando apenas 50% do torque
        dxl_write_word(2, 34, 512); // Usando apenas 50% do torque

cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);

	// Tamanho Padrão de captura WebCam Robo- 640x480
	//CvSize tamanho = cvSize(RESOLUCAO_X,RESOLUCAO_Y); //320 240

    	// Abre o dispositivo de captura "0" que é /dev/video0
 	captura = cvCaptureFromCAM( 0 );

//  	CvCapture* captura1 = cvCaptureFromCAM( 1 );

	if( !captura )
		{
	        fprintf( stderr, "ERRO Não há captura na Camera 0/n" );
	        getchar();
	        return -1;
        }

cvSetCaptureProperty( captura, CV_CAP_PROP_FRAME_WIDTH, RESOLUCAO_X); //1280   1920
cvSetCaptureProperty( captura, CV_CAP_PROP_FRAME_HEIGHT, RESOLUCAO_Y ); //720   1080

CvSize tamanho = cvSize(cvGetCaptureProperty(captura, CV_CAP_PROP_FRAME_WIDTH),cvGetCaptureProperty(captura, CV_CAP_PROP_FRAME_HEIGHT));


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

	if(DECISION_ACTION_VISION==0 && IMU_STATE==0) //Decide ver a bola se o robo estiver em pé
	{
        //-------------- Detecta a Bola------------------------------------------------------
	//***********************************************************************************
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
	CvSeq* circulos;

	//if(RESOLUCAO_X==640)
         // circulos = cvHoughCircles(segmentada, memoria, CV_HOUGH_GRADIENT, 2, segmentada->height/4, 100, 50, 2, 400);//200 100 10 400
	//if(RESOLUCAO_X==1280)
          circulos = cvHoughCircles(segmentada, memoria,CV_HOUGH_GRADIENT,2, segmentada->height/4, 100, 50, 5, 400);//200 100 10 400
                                                                                                                            //vilão 100 50 10 400
        std::string str;
        double area=0;

        for (int i = 0; i < circulos->total; i++)
        {
            float* p = (float*)cvGetSeqElem( circulos, i );

            CvMoments *moments = (CvMoments*)malloc(sizeof(CvMoments));
            cvMoments(segmentada, moments, 1);

            area = cvGetCentralMoment(moments, 0, 0);


            posX = p[0]; // variavel que contem a posição da bola em X da tela
            posY = p[1]; // variavel que contem a posição da bola em Y da tela
            raio_bola = p[2]; // variavel que contem o raio da bola

            cvCircle( Quadro, cvPoint(cvRound(posX),cvRound(posY)), 3, CV_RGB(0,255,0), -1, 8, 0 );
            cvCircle( Quadro, cvPoint(cvRound(posX),cvRound(posY)), cvRound(p[2]), CV_RGB(255,255,255), 2, 8, 0 );

            lost_ball=0;
            cont_search_ball = 0;
            print_tela=1;

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
       }

        if(print_tela>0) //imprimi os valores encontrado no circulo de Hough
        {
            soma_raio_bola = soma_raio_bola + raio_bola;
            cont_bola++;
            if(cont_bola>=20)
            {
                media_raio_bola = (soma_raio_bola/20)*(AJUSTEDIST);
                cont_bola = 0;
                soma_raio_bola=0;

/*
                if(media_raio_bola>=51)
                    dist = -media_raio_bola+98;
                if(media_raio_bola>=35 && media_raio_bola<51)
                    dist = -1.33*media_raio_bola+114.6;
                if(media_raio_bola>=26 && media_raio_bola<35)
                    dist = -2.55*media_raio_bola+158;
                if(media_raio_bola>=20 && media_raio_bola<26)
                    dist = -4*media_raio_bola+195;
                if(media_raio_bola>=15 && media_raio_bola<20)
                    dist = -5.6*media_raio_bola+227;
                if(media_raio_bola>=12 && media_raio_bola<15)
                    dist = -14*media_raio_bola+353;
                if(media_raio_bola<12)
                    dist = -10.5*media_raio_bola+311;
*/
                dist = 3.720745e-5*media_raio_bola*media_raio_bola*media_raio_bola*media_raio_bola
                -0.0079046753*media_raio_bola*media_raio_bola*media_raio_bola +
                0.62022205*media_raio_bola*media_raio_bola - 22.3316384*media_raio_bola + 372.603304;
                dist = dist*0.01;
                //VISION_DIST_BALL = dist;
                //if(media_raio_bola<73)
                    //VISION_DIST_BALL = sqrt(dist*dist - 0.0784);
                //else
                    //VISION_DIST_BALL = 0;
                //alpha = ((531-dxl_read_word( 1, P_PRESENT_POSITION_L))*0.005111)+0.0349;  //0.0349
                //tan(alpha)*(cos(3.1415-alpha)*YC+.448-sin(1.5708-alpha)*XC)+(sin(3.1415-alpha)*YC)+cos(1.5708-alpha)*XC;

            }
            //---- Escreve na Tela o raio de pixels da bola ------------------------------------
            char cr[10];
            sprintf(cr, "Raio %2.3f M %2.3f", raio_bola, media_raio_bola);
            cvPutText (Quadro, cr ,cvPoint(300,300), &font, cvScalar(255,255,0));

            //---- Escreve na tela a área da bola ----------------------------------------------
            char ca[15];
            sprintf(ca, "Area: %4.2f", area);
            cvPutText (Quadro, ca ,cvPoint(300,400), &font, cvScalar(255,255,0));
            print_tela = 0;
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




     if( (cvWaitKey(15) & 255) == 27 || saida > 22 || cont_search_ball > 9 )
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
        //alpha = ((531-dxl_read_word( 1, P_PRESENT_POSITION_L))*0.005111)+0.0349;  //0.0349
        //dist = tan(alpha)*(cos(3.1415-alpha)*YC+.448-sin(1.5708-alpha)*XC)+(sin(3.1415-alpha)*YC)+cos(1.5708-alpha)*XC;

	VISION_DIST_BALL = dist;
	std::cout << VISION_DIST_BALL << std::endl;

	VISION_MOTOR2_ANGLE = dxl_read_word( 2, P_PRESENT_POSITION_L);
	std::cout <<"Servo2 = "<< VISION_MOTOR2_ANGLE << std::endl;

	VISION_MOTOR1_ANGLE = dxl_read_word( 1, P_PRESENT_POSITION_L);
	std::cout <<"Servo1 = "<<VISION_MOTOR1_ANGLE << std::endl;

        }
        }
	//----------------------------------------------------------------------------------
	//**********************************************************************************
	}

	//-------------------------------- Vendo o Gol------------------------------------
	if(DECISION_ACTION_VISION==1 && IMU_STATE==0) //Decide ver o gol se o robo estiver em pé
	{

            IplImage* GoalFrame = cvQueryFrame( captura );

        if( !GoalFrame )
        {
                fprintf( stderr, "ERRO Quadro Gol Vazio...\n" );
                getchar();
//                break;
        }
	//pthread_mutex_unlock( &mutex3 );
	
	// Cria uma janela onde as imagens capturadas serão apresentadas
	cvNamedWindow( "Linhas Detectadas", CV_WINDOW_AUTOSIZE );
	//cvNamedWindow( "Saida Cinza", CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "Original com linhas de Hough", CV_WINDOW_AUTOSIZE );

/*cvNamedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"
*/
/*
int iLowH = 0;
int iHighH = 179;

int iLowS = 0;
int iHighS = 255;

int iLowV = 0;
int iHighV = 255;

//Create trackbars in "Control" window
cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
cvCreateTrackbar("HighH", "Control", &iHighH, 179);

cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
cvCreateTrackbar("HighS", "Control", &iHighS, 255);

cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
cvCreateTrackbar("HighV", "Control", &iHighV, 255);

*/

        //CvScalar min = cvScalar(20, 84, 130, 0);
        //CvScalar max = cvScalar(270, 256, 255, 0);

        CvScalar min = cvScalar(24, 110,   0, 0);//4 160 //160
        CvScalar max = cvScalar(47, 255, 255, 0);//11 //250


        //CvScalar min = cvScalar(iLowH, iLowS, iLowV);
        //CvScalar max = cvScalar(iHighH, iHighS, iHighV);

        CvSize tamanhoG = cvSize(cvGetCaptureProperty(captura, CV_CAP_PROP_FRAME_WIDTH),cvGetCaptureProperty(captura, 		CV_CAP_PROP_FRAME_HEIGHT));
        IplImage* CannyG        = cvCreateImage(tamanhoG, IPL_DEPTH_8U, 1 );
        IplImage* QuadroHsvG    = cvCreateImage(tamanhoG, IPL_DEPTH_8U, 3);
        IplImage* segmentadaG   = cvCreateImage(tamanhoG, IPL_DEPTH_8U, 1);

        cvCvtColor(GoalFrame, QuadroHsvG, CV_BGR2HSV);
        cvInRangeS(QuadroHsvG, min, max, segmentadaG);
        cvCanny( segmentadaG, CannyG, 50, 125, 3 );
         // Memoria para as linhas de Hough
        CvMemStorage* memoriaG = cvCreateMemStorage(0);


    Mat img (segmentadaG);
    cv::Point Coord;
    cv::Moments mm = cv::moments(img,false);
    double moment10 = mm.m10;
    double moment01 = mm.m01;
    double moment00 = mm.m00;
    Coord.x = int(moment10 / moment00);
    Coord.y = int(moment01 / moment00);

    //cvCircle( Quadro, Coord, 10, CV_RGB(0,255,0), -1, 8, 0 );

//	linhas = cvHoughlines2( Imagem de borda, memoria, método , Rho , Theta     , Threshould , parametro 1 = 0 , parametro 2 = 0)
	lines  = cvHoughLines2( CannyG, memoriaG, CV_HOUGH_STANDARD, 1   , CV_PI/180 , 80,   0,  0  );

      if (Coord.x<0)
      Coord.x = 0;
      if (Coord.y<0)
      Coord.y = 0;

        if ((Coord.x != 0) && (Coord.y != 0))
        {
        VISION_DIST_GOAL = 1;
        }
        else
        {
        VISION_DIST_GOAL = 0;
        }



        //cout << "Centro do Gol x = " << Coord.x << endl;
        //cout << "Centro do Gol y = " << Coord.y << endl;



   for( i = 0; i < MIN(lines->total,100); i++ )
        {
        //int d=0;
        //int c=0;

        float* line = (float*)cvGetSeqElem(lines,i);
        float rho = line[0];
        float theta = line[1];

        CvPoint pt1;
        CvPoint pt2;

        double a = cos(theta);
        double b = sin(theta);
        double x0 = a*rho;
        double y0 = b*rho;

            pt1.x = cvRound(x0 + 1000 * (-b));
            pt1.y = cvRound(y0 + 1000 * ( a));
            pt2.x = cvRound(x0 - 1000 * (-b));
            pt2.y = cvRound(y0 - 1000 * ( a));
            cvLine(GoalFrame, pt1, pt2, CV_RGB(100,100,200), 10, 8 );

        //cout << "Ponto 1 = " << pt1.x << endl;
        //cout << "Ponto 2 = " << pt2.x << endl;
        //cout << "----------------" << endl;
        if ((theta < 1.8 ) && (theta > 1.4))
        {
        cout << "TRAVESSAO encontrado (" << pt1.x << " , " << pt1.y << ") e (" << pt2.x << " , " << pt2.y << ") " << "Angulo Theta " << theta << " Radianos" <<endl;
        cout << "---------------------------------" << endl;
        }
        else
        {
        //cout << "TRAVE encontrada" << endl;
        cout << "TRAVE encontrada (" << pt1.x << " , " << pt1.y << ") e (" << pt2.x << " , " << pt2.y << ") " << "Angulo Theta " << theta << " Radianos" << endl;
        cout << "---------------------------------" << endl;
	    }

	}

	cvShowImage( "Linhas Detectadas", CannyG);
	//cvShowImage( "Saida Cinza", segmentada);
	//pthread_mutex_lock( &mutex3 );
	cvShowImage( "Original com linhas de Hough", GoalFrame );
//pthread_mutex_unlock( &mutex3 );

     if( cvWaitKey(10) & 255)
        {}
		cvReleaseMemStorage(&memoriaG);
    		cvReleaseImage(&CannyG);
    		cvReleaseImage(&QuadroHsvG);
    		cvReleaseImage(&segmentadaG);
	//-----------------------------------------------------------------------------------
	}

	if(IMU_STATE==1) //Caso o robô esteja caido
	{
		usleep(200000);
		cout<<"Robo Caido\n";
	}

	} //while(1)

return 0;
}

//=======================================================================================================================
//------------- Função que faz a cabeça centralizar a bola no vídeo------------------------------------------------------
int head_follow_ball(float posx, float posy, bool *head_move1, bool *head_move2)
{
    int pan = 0;
    int tilt = 0;
// Posição inicial da cabeça {304, 594} //01 , 02, cabeça

    dxl_write_word(1, MOVING_SPEED, 300);//300
    dxl_write_word(2, MOVING_SPEED, 300);//300

    //int key = kbhit();
    //if (key != 0)
    //{
       // cvReleaseCapture( &captura );
        //cvReleaseMemStorage(&memoria);

        //keyboard(key);
   // }


//------ Realiza o movimento do Pan -----------------------------------------------------------
    //------ Segue a bola para a esquerda do video -----------------------------------------
    if(posx<(RESOLUCAO_X/2)*(1-CENTERBALL) && *head_move2==false)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L)+( ((RESOLUCAO_X/2)-posx) * AJUSTE));

        //head_move = true;
    }

    //------ Segue a bola para a direita do video -----------------------------------------
    if(posx>(RESOLUCAO_X/2)*(CENTERBALL+1) && *head_move2==false)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L)-( (posx-(RESOLUCAO_X/2)) * AJUSTE));

        //dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L)-(((RESOLUCAO_X/2)-posx) * 0.30) );
        //head_move = true;
    }

    // Para a cabeça se chegou na posição desejada ----------------------------------------
    if(posx>=(RESOLUCAO_X/2)*(1-CENTERBALL) && posx<=(RESOLUCAO_X/2)*(CENTERBALL+1)){
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
    if(posy<(RESOLUCAO_Y/2)*(1-CENTERBALL) && *head_move1==false)
    {
        dxl_write_word(1, P_GOAL_POSITION_L, dxl_read_word( 1, P_PRESENT_POSITION_L)+( ((RESOLUCAO_Y/2)-posy) * AJUSTE));

        //head_move = true;
    }

    //------ Segue a bola para a direita do video -----------------------------------------
    if(posy>(RESOLUCAO_Y/2)*(CENTERBALL+1) && *head_move1==false)
    {
       dxl_write_word(1, P_GOAL_POSITION_L, dxl_read_word( 1, P_PRESENT_POSITION_L)-( (posy-(RESOLUCAO_Y/2)) * AJUSTE));

        //dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L)-(((RESOLUCAO_X/2)-posx) * 0.30) );
        //head_move = true;
    }

    // Para a cabeça se chegou na posição desejada ----------------------------------------
    if(posy>=(RESOLUCAO_Y/2)*(1-CENTERBALL) && posy<=(RESOLUCAO_Y/2)*(CENTERBALL+1)){
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
        dxl_write_word(2, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(1, P_GOAL_POSITION_L, pos_servo1);
    }

    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==7)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(1, P_GOAL_POSITION_L, pos_servo1);
    }


    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==6)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(1, P_GOAL_POSITION_L, pos_servo1-131);
    }


    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==5)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(1, P_GOAL_POSITION_L, pos_servo1-131);
    }


    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==4)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(1, P_GOAL_POSITION_L, pos_servo1-256);
    }


    if(dxl_read_byte( 2, P_MOVING )==0 && varredura==3)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(1, P_GOAL_POSITION_L, pos_servo1-256);
    }

    if(dxl_read_byte( 1, P_MOVING )==0 && varredura==2)
    {
        dxl_write_word(2, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(1, P_GOAL_POSITION_L, pos_servo1);
    }

    if(dxl_read_byte( 1, P_MOVING )==0 && varredura==1)
    {
        dxl_write_word(2, MOVING_SPEED, 700);
        dxl_write_word(1, MOVING_SPEED, 700);
        dxl_write_word(2, P_GOAL_POSITION_L, pos_servo2);
        dxl_write_word(1, P_GOAL_POSITION_L, pos_servo1);
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
