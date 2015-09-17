/*--------------------------------------------------------------------

******************************************************************************
  * @file       vision.cpp
  * @author     Claudio Vilao - ROBOFEI-HT - FEI
  * @version    V0.0.1
  * @created    24/04/2014
  * @Modified   18/07/2014
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
#include <string>
#include <dynamixel.h>
#include <unistd.h>
#include <boost/program_options.hpp> //tratamento de argumentos linha de comando

#include <iterator>
#include <fstream>
#include <vector>

#define HEAD_TILT 20
#define HEAD_PAN 19

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
//#define RESOLUCAO_X 1280 //1280   1920
//#define RESOLUCAO_Y 720  //720   1080

#define RESOLUCAO_X 1920 //1280   1920
#define RESOLUCAO_Y 1080  //720   1080

#define CENTERBALL 0.08 // Porcentagem de quanto centraliza a bola no quadro
//#define AJUSTE 0.285 // para resolução de 640x480
//#define AJUSTE 0.285*0.49 // para resolução de 1280x720
#define AJUSTE 0.285*0.33 // para resolução de 1920x1080
//#define AJUSTEDIST 1 // para resolução de 640x480
//#define AJUSTEDIST 0.5 //para resolução de 1280x720
#define AJUSTEDIST 0.33 //para resolução de 1920x1080

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);
void captura_pixel_hsv();
int HeadFollow(float posx, float posy, bool *head_move1, bool *head_move2);

void BallSearch(bool inicio);

void GoalSearch(bool inicio);

void LocalizationSearch(bool inicio);
void HistogramPosition(bool inicio);

float media_raio_bola = 0;
float soma_raio_bola = 0;
uint8_t cont_bola = 0;
static unsigned int varre;

CvHaarClassifierCascade *cascade;
CvMemStorage            *storage;

//803, 512,
int pos_servo1 = 250; //803
int pos_servo2 = 512;
int cont=0; //____________________________________________________________
int cont_BallSearch=0;
int cont_GoalSearch=0;
Mat b_hist, g_hist, r_hist;

CvCapture* captura;
CvMemStorage* memoria;

int HeadFollow(float, float, bool*, bool*);

void BallSearch(bool);

double detect( IplImage *img, double &posx, double &posy); // funcao

	void saveHistogramVectorToFile(Mat HistogramVector, string fileName);
	vector<float> openFiletoGetHistogramVector(string fileName);

        static string BlueFile  = "genfiles/BlueHistogram.dat";
        static string GreenFile = "genfiles/GreenHistogram.dat";
        static string RedFile   = "genfiles/RedHistogram.dat";



int countvector = 1;
bool TransitionBall = 1;
bool TransitionGoal = 1;
unsigned int BufferBallServo1; //Guarda a ultima posição do servo
unsigned int BufferBallServo2; //Guarda a ultima posição do servo
unsigned int BufferGoalServo1; //Guarda a ultima posição do servo do Gol
unsigned int BufferGoalServo2; //Guarda a ultima posição do servo do Gol



int main(int argc, char **argv)
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
    bool gui = false;

    using_shared_memory();


//*********************************************************
//-------------para entrada de argumentos-----------------------------------
 namespace po=boost::program_options;

  po::options_description desc("options");
  desc.add_options()
    ("help", "produce help message")
    ("vb", "apresenta o video da bola na tela")
    ("vg", "apresenta o video do gol na tela")
    ("vl", "apresenta o video e o histograma da localização na tela")
    //("baud,b", po::value<int32_t>(&baudnum),"baud rate")
    ("tg", "abre o trackbars para calibrar a detecção do gol")
    ("tb", "abre o trackbars para calibrar a detecção da bola")
    ("sh", "Abre captura para salvar o histograma")
    ("b", "Escolhe para iniciar com a bola")
    ("g", "Escolhe para iniciar com o gol")
    ("l", "Escolhe para iniciar com a localização")
	("ws", "Abre sem servo dynamixel")

    ("dev0", "Abre Camera 0")
    ("dev1", "Abre Camera 1")
;
  
  po::variables_map variables;
  po::store(po::parse_command_line(argc, argv, desc), variables);
  po::notify(variables); 

//------------------------------

if (variables.count("help")) 
{
    cout << desc << "\n";
    return 1;
}
/*
if (variables.count("vb"))
	cout<<"Chamou o video da bola\n";
if (variables.count("vg"))
	cout<<"Chamou o video do gol\n";
if (variables.count("vl"))
	cout<<"Chamou o video e o histograma da localização\n";
*/

//--------------------------------------------------------------------------
//*************************************************

  //system("echo | sudo chmod 777 /dev/ttyUSB*");//libera USB

// ---- Open USBDynamixel -----------------------------------------------{
if (!variables.count("ws"))
{
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
    			if(dxl_read_byte( HEAD_TILT, 3 ) == HEAD_TILT)
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
}
//-----------------------------------------------------------------------------}

    CvFont font;
    double hScale=1.0;
    double vScale=1.0;
    int    lineWidth=1;
    double posX = 1;
    double posY = 1;
    float raio_bola = 0;
    bool inicio=1;
    float alpha;
    float dist;
    bool print_tela = 0;

   cont_BallSearch = 0;

	BufferBallServo1 = dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L); 
	BufferBallServo2 = dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L); 
	BufferGoalServo1 = dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L); 
	BufferGoalServo2 = dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L); 
        dxl_write_word(HEAD_TILT, 34, 512); // Usando apenas 50% do torque
        dxl_write_word(HEAD_PAN, 34, 512); // Usando apenas 50% do torque

cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale,vScale,0,lineWidth);

	// Tamanho Padrão de captura WebCam Robo- 640x480
	//CvSize tamanho = cvSize(RESOLUCAO_X,RESOLUCAO_Y); //320 240

    	// Abre o dispositivo de captura "0" que é /dev/video0
	int dev = 0;
	if (variables.count("dev0")) dev = 0;
	if (variables.count("dev1")) dev = 1;

 	captura = cvCaptureFromCAM( dev );

		if( !captura )
		{
	        	fprintf( stderr, "ERRO Não há captura na Camera %i/n",dev  );
	        	getchar();
	        	return -1;
        }

cvSetCaptureProperty( captura, CV_CAP_PROP_FRAME_WIDTH, RESOLUCAO_X); //1280   1920
cvSetCaptureProperty( captura, CV_CAP_PROP_FRAME_HEIGHT, RESOLUCAO_Y ); //720   1080

CvSize tamanho = cvSize(cvGetCaptureProperty(captura, CV_CAP_PROP_FRAME_WIDTH),cvGetCaptureProperty(captura, CV_CAP_PROP_FRAME_HEIGHT));

	//Cria a janela do video da bola
	if (variables.count("vb")) cvNamedWindow( "Video Bola", CV_WINDOW_AUTOSIZE );

	// Cria uma janela onde as imagens capturadas serão apresentadas
	if (variables.count("vg")) cvNamedWindow( "Imagem", CV_WINDOW_AUTOSIZE );


        //CvScalar minG = cvScalar(20, 84, 130, 0);
        //CvScalar maxG = cvScalar(270, 256, 255, 0);

//r 80~110
//g 130~150
//b 40~70

			//============Haar ======================================
			double a=0,raio_medio=0, Raio, Distancia;
			IplImage  *frame;
			int       key;
			const char      *filename = "/home/fei/RoboFEI-HT/Vision/src/Ball.xml"; //Name of the classifier

			cascade = ( CvHaarClassifierCascade* )cvLoad( filename, 0, 0, 0 );
			//if(cascade) cout<<"cascade correto"<<endl;
			storage = cvCreateMemStorage(0);
			//if(storage) cout<<"storage correto"<<endl;

			assert( cascade && storage && captura );
			//=======================================================

    usleep(1000000);

//*****************************************************************************

	IplImage*  Quadro_hsv  = cvCreateImage(tamanho, IPL_DEPTH_8U, 3);
	IplImage*  segmentada  = cvCreateImage(tamanho, IPL_DEPTH_8U, 1);

DECISION_ACTION_VISION = 0;

//***********************************************************************************
	if (variables.count("b")) DECISION_ACTION_VISION = 0; // Detecta a Bola.				   //
	if (variables.count("g")) DECISION_ACTION_VISION = 1; // Acha o Gol.				   //
	if (variables.count("l")) DECISION_ACTION_VISION = 2; // Localiza Histograma igual ao armazenado.	   //
	if (variables.count("sh")) DECISION_ACTION_VISION = 3; // Salva o Histograma.			   //
//***********************************************************************************

VISION_STATE =0;


    while( 1 )
 	{

		if(DECISION_ACTION_VISION==0 && IMU_STATE==0) //Decide ver a bola se o robo estiver em pé
		{
	
			//if(VISION_STATE==1)		
			//{
			//    dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1-200);
			//    dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2);
			//}

			//while(1){}
			//cvNamedWindow("video", 1);

				//while((cvWaitKey(10) & 255) != 27) 
				//{
					frame = cvQueryFrame( captura );

					Raio=detect(frame, posX, posY);
		
					if (Raio>0.1){
						lost_ball=0;
						cont_BallSearch=0;
						if(a<10){
							raio_medio=raio_medio+Raio;	
							a++;
							}
						else{
							a=0;
							raio_medio=raio_medio/10;
			//				VISION_DIST_BALL=(1e-05*pow(raio_medio,4))+(-0.0049*pow(raio_medio,3))+(0.8284*pow(raio_medio,2))+(-62.05*raio_medio)+1869.1;
							VISION_DIST_BALL=331.59-53.3*log(raio_medio-50);
							printf("Raio Medio: %g\n", raio_medio);
							printf("Distancia: %g\n", VISION_DIST_BALL);
							raio_medio=0;
						}
					}

					if(lost_ball>20) //verifica se depois de 30 frames a bola está perdida no campo
					{
						//while(*comunica!=0);
						//if(*comunica)
						VISION_SEARCH_BALL = 1;

						if(VISION_STATE==0)
						      BallSearch(inicio);//Procura a bola pelo campo

						saida = 0;
						//char cD[10];
						//sprintf(cD, "procurando a bola");
						cvPutText (frame, "Procurando a bola" ,cvPoint(150,450), &font, cvScalar(255,255,0));
						//call_search = 1;
						inicio = 0;
					}
					else
					{
						VISION_SEARCH_BALL = 0;
						if(inicio==0)// Faz o robô parar a cabeça no instante que achou a bola que estava perdida
						{            // Sem esse comando o código não funciona porque ele não para a cabeça
						    dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L));
						    dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L));
						}

						inicio =1;

						//if(VISION_STATE==0)
						if (HeadFollow(posX, posY, &head_move1, &head_move2) == 1){// Move a cabeça para seguir a bola
						         saida++;
						}

					}
					if(lost_ball<100)
						lost_ball++;

					if (variables.count("vb"))
					{
						//cvShowImage( "Filtragem das cores", segmentada ); // O stream depois da filtragem de cores
						//cvShowImage( "Imagem", Quadro ); // Stream Original com a bola detectada
						//	cvShowImage( "Video2", Quadro2 ); // Stream Original com a bola detectada
						cvShowImage( "Video Bola", frame );
					}
					//cvReleaseMemStorage(&memoria);


					if( (cvWaitKey(10) & 255) == 27 || saida > 22 || cont_BallSearch > 9 )
					{
						 ///////////  cvReleaseCapture( &captura );
						//      cvReleaseCapture( &captura1 );
						 //   cvDestroyWindow( "Video1" );
						//    cvDestroyWindow( "Filtragem das cores" );
						//    cvDestroyWindow( "Sistema de cores RGB - HSV" );
						if (cont_BallSearch > 9){
						//dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, 594);
						//dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, 304);

						std::cout << "bola não encontrada" << std::endl;
						VISION_LOST_BALL = 1;
						//return 2;
						}
						else
						{
							//printf("servo 1: %d",dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L));
							VISION_LOST_BALL = 0;
							//alpha = ((531-dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L))*0.005111)+0.0349;  //0.0349
							//dist = tan(alpha)*(cos(3.1415-alpha)*YC+.448-sin(1.5708-alpha)*XC)+(sin(3.1415-alpha)*YC)+cos(1.5708-alpha)*XC;

							//VISION_DIST_BALL = dist;
							//std::cout << VISION_DIST_BALL << std::endl;

							VISION_DIST_GOAL=0; //zera a variavel do gol

							VISION_MOTOR2_ANGLE = dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L);
							//std::cout <<"Servo2 Bola = "<< VISION_MOTOR2_ANGLE << std::endl;

							VISION_MOTOR1_ANGLE = dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L);
							//std::cout <<"Servo1 Bola= "<<VISION_MOTOR1_ANGLE << std::endl;

							BufferBallServo1 = VISION_MOTOR1_ANGLE; //Guarda a posição do servo1
							BufferBallServo2 = VISION_MOTOR2_ANGLE; //Guarda a posição do servo2

					   	}
					}
				 	if( (cvWaitKey(10) & 255) == 27)
				  	{
						cvReleaseImage(&frame);
						cvReleaseCapture(&captura);
						cvDestroyWindow("video");
						cvReleaseHaarClassifierCascade(&cascade);
						cvReleaseMemStorage(&storage);
						break;
					}

				

				//key = cvWaitKey(50);
				//}





	}

//******************************************************************************************************
	if(DECISION_ACTION_VISION==1 && IMU_STATE==0) //Decide ver o gol se o robo estiver em pé
	{       	
	}// endif DECISION_ACTION_VISION==1

//******************************************************************************************************
	// Vendo o Histograma---------------------------------------------------------------------------
	if(DECISION_ACTION_VISION==2 && IMU_STATE==0) //Decide ver o histograma se o robo estiver em pé
	{

	TransitionBall = 1;
	TransitionGoal = 1;

        // Pegar um quadro
        IplImage* QuadroHist = cvQueryFrame( captura );

        if( !QuadroHist )
        	{
                fprintf( stderr, "ERRO Quadro Histograma vazio...\n" );
                getchar();
                //break;
        	}

  		/// Separa a imagem em 3 planos ( B, G e R )
		  vector<Mat> bgr_planes;
		  split( QuadroHist, bgr_planes );

  		/// Número de binários
  		int histSize = 256;

		  /// Set the ranges ( for B,G,R) )
		  float range[] = { 0, 256 } ;
		  const float* histRange = { range };

		  bool uniform = true;
		  bool accumulate = false;

		  /// Calcula os Histogramas:
		  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
		  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
		  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

		  /// Gráficos
		  int hist_w = 512;
		  int hist_h = 400;
		  int bin_w = cvRound( (double) hist_w/histSize );

		  Mat histImageB( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
		  Mat histImageG( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
		  Mat histImageR( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
		  Mat histImage ( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

		  /// Normaliza o resultado [ 0, histImage.rows ]
		  normalize(b_hist, b_hist, 0, histImageB.rows, NORM_MINMAX, -1, Mat() );
		  normalize(g_hist, g_hist, 0, histImageG.rows, NORM_MINMAX, -1, Mat() );
		  normalize(r_hist, r_hist, 0, histImageR.rows, NORM_MINMAX, -1, Mat() );

            /// Grafico para cada canal
	        for( int i = 1; i < histSize; i++ )
	            {

	            Point InitialBluePoint  = Point( bin_w*(i)  , hist_h - cvRound(b_hist.at<float>(i)  ) );
	            Point InitialGreenPoint = Point( bin_w*(i)  , hist_h - cvRound(g_hist.at<float>(i)  ) );
	            Point InitialRedPoint   = Point( bin_w*(i)  , hist_h - cvRound(r_hist.at<float>(i)  ) );

	            Point FinalBluePoint    = Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) );
	            Point FinalGreenPoint   = Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) );
	            Point FinalRedPoint     = Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) );

	            //line( histImageB, FinalBluePoint  , InitialBluePoint  , Scalar( 255, 0, 0), 2, 8, 0  );
	            //line( histImageG, FinalGreenPoint , InitialGreenPoint , Scalar( 0, 255, 0), 2, 8, 0  );
	            //line( histImageR, FinalRedPoint   , InitialRedPoint   , Scalar( 0, 0, 255), 2, 8, 0  );

	            line( histImage, FinalBluePoint  , InitialBluePoint   , Scalar( 255, 0, 0), 2, 8, 0  );
	            line( histImage, FinalGreenPoint , InitialGreenPoint  , Scalar( 0, 255, 0), 2, 8, 0  );
	            line( histImage, FinalRedPoint   , InitialRedPoint    , Scalar( 0, 0, 255), 2, 8, 0  );
	            }

/*
        if (countvector == 1)
        {
        
	inicio = 0;
	HistogramPosition(inicio);
	usleep(3500);
        saveHistogramVectorToFile(b_hist, BlueFile);
        saveHistogramVectorToFile(g_hist, GreenFile);
        saveHistogramVectorToFile(r_hist, RedFile);
        countvector++;
        }
*/
            vector<float> HistogramVectorBlue  ;
            vector<float> HistogramVectorGreen ;
            vector<float> HistogramVectorRed   ;

            HistogramVectorBlue  = openFiletoGetHistogramVector(BlueFile );
            HistogramVectorGreen = openFiletoGetHistogramVector(GreenFile);
            HistogramVectorRed   = openFiletoGetHistogramVector(RedFile  );

            vector<float> BhistVector;
            vector<float> GhistVector;
            vector<float> RhistVector;
            b_hist.copyTo(BhistVector);
            g_hist.copyTo(GhistVector);
            r_hist.copyTo(RhistVector);

	int CB = 0;
	int CG = 0;
	int CR = 0;
	bool blueflag  = 0;
	bool greenflag = 0;
	bool redflag   = 0;

	float RangeMin = 50;
	float RangeMax = 50;
	float sensibilidade = 35;


	double BlueComp = compareHist ( HistogramVectorBlue, BhistVector ,0 );
	double GreenComp = compareHist ( HistogramVectorGreen, GhistVector ,0 );
	double RedComp = compareHist ( HistogramVectorRed, RhistVector ,0 );


		if ((BlueComp > -0.02) && (BlueComp < 0.04 ))
			{
			cout << "Achei Azul" << BlueComp << endl;
			blueflag = 1;
			}

		if ((GreenComp > -0.02) && (GreenComp < 0.04 ))
			{
			cout << "Achei Verde" << GreenComp << endl;
			greenflag = 1;
			}
		if ((RedComp > -0.02) && (RedComp < 0.04 ))
			{
			cout << "Achei Vermelho" << RedComp << endl;
			redflag = 1;
			}



//	cout << " Comparacao Azul : " << BlueComp << endl;
//	cout << " Comparacao Verde : " << GreenComp << endl;
//	cout << " Comparacao Vermelho : " << RedComp << endl;
 
inicio = 0;

        if (blueflag && greenflag && redflag)
        {
       	 //cout << "Achei para qual lado devo chutar!!" << endl;
		cout << dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L) << endl;
		cont++;
		if (cont >5)
		{
		cout << " Achei" << endl;
		LOCALIZATION_THETA = 1;
		VISION_MOTOR2_ANGLE = dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L);
		std::cout <<"Servo2 Localizacao= "<< VISION_MOTOR2_ANGLE << std::endl;

		VISION_MOTOR1_ANGLE = dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L);
		std::cout <<"Servo1 Localizacao= "<<VISION_MOTOR1_ANGLE << std::endl;

		}

        }
        else
        {
	cont =0;
        cout << "Procurando!" << endl;
	LocalizationSearch(inicio);
	LOCALIZATION_THETA = 0;
        }


	CB = 0;
	CG = 0;
	CR = 0;
	blueflag  = 0;
	greenflag = 0;
	redflag   = 0;

        cvShowImage( "Imagem", QuadroHist );

        imshow("Histograma - 3 Canais", histImage );
        /*
        imshow("Histograma - Canal Azul", histImageB );
        imshow("Histograma - Canal Verde", histImageG );
        imshow("Histograma - Canal Vermelho", histImageR );
        */
     		if( (cvWaitKey(10) & 255) == 27)
      		  {
			break;
			}
		

	}// endif DECISION_ACTION_VISION==2
   //******************************************************************************************

	if(DECISION_ACTION_VISION==3) // Salvo o Histograma------------------------------------
	{
	inicio = 0;
	HistogramPosition(inicio);
	TransitionBall = 1;
	TransitionGoal = 1;

        // Pegar um quadro
        IplImage* QuadroHist = cvQueryFrame( captura );

        if( !QuadroHist )
        	{
                fprintf( stderr, "ERRO Quadro Histograma vazio...\n" );
                getchar();
                //break;
        	}

  		/// Separa a imagem em 3 planos ( B, G e R )
		  vector<Mat> bgr_planes;
		  split( QuadroHist, bgr_planes );

  		/// Número de binários
  		int histSize = 256;

		  /// Set the ranges ( for B,G,R) )
		  float range[] = { 0, 256 } ;
		  const float* histRange = { range };

		  bool uniform = true;
		  bool accumulate = false;

		  /// Calcula os Histogramas:
		  calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
		  calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
		  calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

		  /// Gráficos
		  int hist_w = 512;
		  int hist_h = 400;
		  int bin_w = cvRound( (double) hist_w/histSize );

		  Mat histImageB( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
		  Mat histImageG( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
		  Mat histImageR( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
		  Mat histImage ( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

		  /// Normaliza o resultado [ 0, histImage.rows ]
		  normalize(b_hist, b_hist, 0, histImageB.rows, NORM_MINMAX, -1, Mat() );
		  normalize(g_hist, g_hist, 0, histImageG.rows, NORM_MINMAX, -1, Mat() );
		  normalize(r_hist, r_hist, 0, histImageR.rows, NORM_MINMAX, -1, Mat() );


	usleep(3500);
        saveHistogramVectorToFile(b_hist, BlueFile);
        saveHistogramVectorToFile(g_hist, GreenFile);
        saveHistogramVectorToFile(r_hist, RedFile);
	usleep(1500);
     		if( (cvWaitKey(10) & 255) == 27)
      			break;
			

		} // endif DECISION_ACTION_VISION==3

	} //while(1)

return 0;
}



//=======================================================================================================================
//------------- Função que faz a cabeça centralizar a bola no vídeo------------------------------------------------------
int HeadFollow(float posx, float posy, bool *head_move1, bool *head_move2)
{
     int pan = 0;
    int tilt = 0;
// Posição inicial da cabeça {304, 594} //01 , 02, cabeça

    dxl_write_word(HEAD_TILT, MOVING_SPEED, 300);//300
    dxl_write_word(HEAD_PAN, MOVING_SPEED, 300);//300

//------ Realiza o movimento do Pan -----------------------------------------------------------
    //------ Segue a bola para a esquerda do video -----------------------------------------
    if(posx<(RESOLUCAO_X/2)*(1-CENTERBALL) && *head_move2==false)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L)+( ((RESOLUCAO_X/2)-posx) * AJUSTE));

        //head_move = true;
    }

    //------ Segue a bola para a direita do video -----------------------------------------
    if(posx>(RESOLUCAO_X/2)*(CENTERBALL+1) && *head_move2==false)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L)-( (posx-(RESOLUCAO_X/2)) * AJUSTE));

        //dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L)-(((RESOLUCAO_X/2)-posx) * 0.30) );
        //head_move = true;
    }

    // Para a cabeça se chegou na posição desejada ----------------------------------------
    if(posx>=(RESOLUCAO_X/2)*(1-CENTERBALL) && posx<=(RESOLUCAO_X/2)*(CENTERBALL+1)){
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L));
        pan = 1;

    }

    if(dxl_read_byte( HEAD_PAN, P_MOVING ))
        *head_move2 = true;  // verifica se a cabeça está em movimento
    else
        *head_move2 = false; // verifica se a cabeça está em movimento
//---------------------------------------------------------------------------------------------


//------ Realiza o movimento do Tilt -----------------------------------------------------------
    //------ Segue a bola para a esquerda do video -----------------------------------------
    if(posy<(RESOLUCAO_Y/2)*(1-CENTERBALL) && *head_move1==false)
    {
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L)-( ((RESOLUCAO_Y/2)-posy) * AJUSTE));

        //head_move = true;
    }

    //------ Segue a bola para a direita do video -----------------------------------------
    if(posy>(RESOLUCAO_Y/2)*(CENTERBALL+1) && *head_move1==false)
    {
       dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L)+( (posy-(RESOLUCAO_Y/2)) * AJUSTE));

        //dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L)-(((RESOLUCAO_X/2)-posx) * 0.30) );
        //head_move = true;
    }

    // Para a cabeça se chegou na posição desejada ----------------------------------------
    if(posy>=(RESOLUCAO_Y/2)*(1-CENTERBALL) && posy<=(RESOLUCAO_Y/2)*(CENTERBALL+1)){
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L));

        tilt = 1;
    }

    if(dxl_read_byte( HEAD_TILT, P_MOVING ))
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


void BallSearch(bool inicio)
{
    // Posição inicial da cabeça {304, 594} //01 , 02, cabeça
static unsigned int varredura=0;

        dxl_write_word(HEAD_PAN, MOVING_SPEED, 100);//Seta as velocidades da cabeça
        dxl_write_word(HEAD_TILT, MOVING_SPEED, 400);




   //     int key = kbhit();
   //     if (key != 0)
   //     {
  //      cvReleaseCapture( &captura );
  //      cvReleaseMemStorage(&memoria);

  //      keyboard(key);
  //      }



    if(inicio)
        varredura--; // continua a varredura de onde parou

    if(varredura>7||varredura<1)//9
        varredura=0;

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0){
     varredura++;
     cont_BallSearch++;
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==8)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+256);
    }

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==7)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+256);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==6)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+131);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==5)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+131);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==4)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+256);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==3)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+256);
    }

    if(dxl_read_byte( HEAD_TILT, P_MOVING )==0 && varredura==2)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+256);
    }

    if(dxl_read_byte( HEAD_TILT, P_MOVING )==0 && varredura==1)
    {
        dxl_write_word(HEAD_PAN, MOVING_SPEED, 700);
        dxl_write_word(HEAD_TILT, MOVING_SPEED, 700);
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+256);
        //sleep(3);
    }


    //dxl_write_word(HEAD_PAN, MOVING_SPEED, 20);//300
    //dxl_write_word(HEAD_TILT0, P_GOAL_POSITION_L, );


}

void saveHistogramVectorToFile(Mat HistogramVector, string fileName)
{
    string separator = " "; // Use blank as default separator between single features
    fstream File;

    File.open(fileName.c_str(), ios::out);
    if (File.good() && File.is_open())
    {
        for (int i = 0; i < HistogramVector.rows; ++i)
        {
            for (int j = 0; j < HistogramVector.cols; ++j)
                {
                    File << HistogramVector.at<double>(i,j) << separator;
                }
        }
        File << endl;
        File.flush();
        File.close();
    }
}

vector<float> openFiletoGetHistogramVector(string fileName)
{
    //string fileName="Pranav";
    const char *fileName1;
    fileName1 =fileName.c_str();

	ifstream File(fileName1);
	istream_iterator<float> start(File), end;
	vector<float> histogramVector(start, end);

return (histogramVector);
}



void GoalSearch(bool inicio)
{
    // Posição inicial da cabeça {304, 594} //01 , 02, cabeça
static unsigned int varredura=0;

        dxl_write_word(HEAD_PAN, MOVING_SPEED, 150);//Seta as velocidades da cabeça
        dxl_write_word(HEAD_TILT, MOVING_SPEED, 400);

    if(inicio)
        varredura--; // continua a varredura de onde parou

    if(varredura>9||varredura<1)
        varredura=0;

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0)
	{
     varredura++;
     cont_GoalSearch++;
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==8)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1);
    }

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==7)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1);
    }
	

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==6)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==5)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==4)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==3)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1);
    }

    if(dxl_read_byte( HEAD_TILT, P_MOVING )==0 && varredura==2)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1);
    }

    if(dxl_read_byte( HEAD_TILT, P_MOVING )==0 && varredura==1)
    {
        dxl_write_word(HEAD_PAN, MOVING_SPEED, 700);
        dxl_write_word(HEAD_TILT, MOVING_SPEED, 700);
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1);
      
    }

}

void LocalizationSearch(bool inicio)
{

    // Posição inicial da cabeça {304, 594} //01 , 02, cabeça
static unsigned int varredura=0;

        dxl_write_word(HEAD_PAN, MOVING_SPEED, 150);//Seta as velocidades da cabeça
        dxl_write_word(HEAD_TILT, MOVING_SPEED, 200);

    if(inicio)
        varredura--; // continua a varredura de onde parou

    if(varredura>9||varredura<1)
        varredura=0;

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0)
	{
     varredura++;
     //cont_BallSearch++;
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==8)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==7)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }
	

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==6)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==5)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==4)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura==3)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }

    if(dxl_read_byte( HEAD_TILT, P_MOVING )==0 && varredura==2)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }

    if(dxl_read_byte( HEAD_TILT, P_MOVING )==0 && varredura==1)
    {
        dxl_write_word(HEAD_PAN, MOVING_SPEED, 700);
        dxl_write_word(HEAD_TILT, MOVING_SPEED, 700);
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
      
    }


}


void HistogramPosition(bool inicio)
{

        dxl_write_word(HEAD_PAN, MOVING_SPEED, 353);//Seta as velocidades da cabeça
        dxl_write_word(HEAD_TILT, MOVING_SPEED, 612);

    //if((dxl_read_byte( HEAD_TILT, P_MOVING )==0) && (dxl_read_byte( HEAD_PAN, P_MOVING )==0 ))
    //{
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    //}
    
    while((dxl_read_byte( HEAD_TILT, P_MOVING )==1) || (dxl_read_byte( HEAD_PAN, P_MOVING )==1 ))
    {}

}

double detect(IplImage *img, double &posx, double &posy)
{
    int i;
    double     Radius, Raio;

    CvSeq *object = cvHaarDetectObjects(
            img,
            cascade,
            storage,
            1.11, //-------------------SCALE FACTOR 1,5
            8,//------------------MIN NEIGHBOURS 2
            1,//---------------------- 1
                      // CV_HAAR_DO_CANNY_PRUNING,
            cvSize( 100,100), // ------MINSIZE
            cvSize(640,480) );//---------MAXSIZE

    for( i = 0 ; i < ( object ? object->total : 0 ) ; i++ )
        {
            CvRect *r = ( CvRect* )cvGetSeqElem( object, i );
            //cvRectangle( img, cvPoint( r->x, r->y ), cvPoint( r->x + r->width, r->y + r->height ), CV_RGB( 64, 255, 64 ), 3, 8, 0 );

		if ( (r->height/2) >= (r->width/2) )
			{
				Radius = r->height/2;
			}
			else{
				Radius = r->width/2;
				}

cvCircle( img, cvPoint((r->x + (r->width/2)),(r->y + (r->height/2))), cvRound(Radius), CV_RGB(64, 255, 64), 3, 8, 0 );
			
			posx = r->x + (r->width/2);
			posy = r->y + (r->height/2);
            //printf("%d,%d\nnumber =%d\n",r->x,r->y,object->total);
		
		//printf("Raio: %g\n", Radius);

        }

		return(Radius);
}



#ifdef DEBUG
cout << "vision received" << A <<" "<< B <<" "<< C << endl;
#endif

//}

//}






