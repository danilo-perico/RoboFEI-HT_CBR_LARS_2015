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

#include"vision.h"

#define INI_FILE_PATH       "../Control/Data/config.ini"



int main(int argc, char **argv)
{

	minIni* ini;
	ini = new minIni(INI_FILE_PATH);


	if((pos_servo2=ini->getd("Offset","ID_19",-1024))==-1024){
		cout<<"Erro na leitura do conf.ini";
		return(0);
	}

	if((pos_servo1=ini->getd("Offset","ID_20",-1024))==-1024){
		cout<<"Erro na leitura do conf.ini";
		return(0);
	}


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

    ("Far", "Capture open for histogram save from far (Use together with F1...F4 Argments)")
    ("Med", "Capture open for histogram save from medium (Use together with F1...F4 Argments)")
    ("Near", "Capture open for histogram save from Near (Use together with F1...F4 Argments)")

    ("F1", "Choose Soccer Field number 1 (Use together with Far or Med or Near Argments)")
    ("F2", "Choose Soccer Field number 2 (Use together with Far or Med or Near Argments)")
    ("F3", "Choose Soccer Field number 3 (Use together with Far or Med or Near Argments)")
    ("F4", "Choose Soccer Field number 4 (Use together with Far or Med or Near Argments)")

    ("dev0", "Abre Camera 0")
    ("dev1", "Abre Camera 1")
;
  
  po::variables_map variables;
  po::store(po::parse_command_line(argc, argv, desc), variables);
  po::notify(variables); 

	if (variables.count("Far")) 
		{
		    D = "Far";
		}

	if (variables.count("Med")) 
		{
		    D = "Med";
		}

	if (variables.count("Near")) 
		{
		    D = "Near";
		}

	if (variables.count("F1")) 
		{
		    N = "One";
		}

	if (variables.count("F2")) 
		{
		    N = "Two";
		}
	if (variables.count("F3")) 
		{
		    N = "Three";
		}
	if (variables.count("F4")) 
		{
		    N = "Four";
		}


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

		    //dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1);
		    //dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2);
			//sleep(10);

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

//			assert( cascade && storage && captura );
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

VideoWriter video("/home/fei/RoboFEI-HT/genfiles/SavedVideo/Bola.avi",CV_FOURCC('M','J','P','G'), 10, Size(RESOLUCAO_X,RESOLUCAO_Y),true);

    while( 1 )
 	{

		if(DECISION_ACTION_VISION==0 && IMU_STATE==0) //Decide ver a bola se o robo estiver em pé
		{
	
		//if(VISION_STATE==1)		
		//{
		//    dxl_write_word(1, P_GOAL_POSITION_L, dxl_read_word( 1, P_PRESENT_POSITION_L));
		//    dxl_write_word(2, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L));
		//}
        //-------------- Detecta a Bola------------------------------------------------------
	//***********************************************************************************

	//CvScalar minB;// = cvScalar(5, 160, 50, 0);//4 160 //160
	//CvScalar maxB;// = cvScalar(20, 250, 255, 0);//11 //250


        // Pegar um quadro
        IplImage* Quadro = cvQueryFrame( captura );
	
        if( !Quadro )
        {
                fprintf( stderr, "ERRO Quadro vazio...\n" );
                getchar();
                break;
        }
/*
	if (!variables.count("tb"))
	{
        	CvScalar minB = cvScalar(iLowHB, iLowSB, iLowVB);
        	CvScalar maxB = cvScalar(iHighHB, iHighSB, iHighVB);
	}
	else
	{
*/
		CvScalar minB = cvScalar(0, 159, 60, 0);//4 160 //160
		CvScalar maxB = cvScalar(28, 255, 255, 0);//11 //250
	//}

//*******************************************************************************
/*		IplImage*  Gray  = cvCreateImage(tamanho, IPL_DEPTH_8U, 1);
		cvCvtColor(Quadro, Gray, CV_BGR2GRAY);
		double brightness =cvMean(Gray);
		if (brightness < 30)
		{
		VISION_STATE = 1;
		cout << "Esta Escuro!!!"<< endl; 
		}
		else
		{
		VISION_STATE =0;
		}*/
//*******************************************************************************



	if(TransitionBall)
	{
		TransitionGoal = 1;
        	dxl_write_word(HEAD_PAN, MOVING_SPEED, 700);
        	dxl_write_word(HEAD_TILT, MOVING_SPEED, 700);
        	dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, BufferBallServo2);
        	dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, BufferBallServo1);
		sleep(1);
	}	
	TransitionBall = 0;
	TransitionGoal = 1;

       	// Converter o espaço de cores para o HSV para ficar mais fácil filtrar as cores.
        cvCvtColor(Quadro, Quadro_hsv, CV_BGR2HSV);

        // Filtrar cores que não interessam.
        cvInRangeS(Quadro_hsv, minB, maxB, segmentada);

        // Memoria para os circulos de Hough
        memoria = cvCreateMemStorage(0);

        cvDilate(segmentada,segmentada,NULL,5);
        cvErode(segmentada,segmentada,NULL,5);
        cvSmooth( segmentada, segmentada, CV_GAUSSIAN, 9, 9 );
        //cvSmooth( segmentada, segmentada, CV_GAUSSIAN, 9, 9 );
        CvSeq* circulos = cvHoughCircles(segmentada, memoria, CV_HOUGH_GRADIENT, 2, segmentada->height/4, 100, 50, 2, 500);// 200 100 10 400
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
            cont_BallSearch = 0;
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
                media_raio_bola = soma_raio_bola/20;
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

		if(VISION_STATE==0)
                    BallSearch(inicio);//Procura a bola pelo campo

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
                dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, dxl_read_word( 1, P_PRESENT_POSITION_L));
                dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, dxl_read_word( 2, P_PRESENT_POSITION_L));
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
        cvShowImage( "Filtragem das cores", segmentada ); // O stream depois da filtragem de cores
        cvShowImage( "Imagem", Quadro ); // Stream Original com a bola detectada
//	cvShowImage( "Video2", Quadro2 ); // Stream Original com a bola detectada
	}
		cvReleaseMemStorage(&memoria);




     if( (cvWaitKey(10) & 255) == 27 || saida > 22 || cont_BallSearch > 9 )
        {
     ///////////  cvReleaseCapture( &captura );
//      cvReleaseCapture( &captura1 );
     //   cvDestroyWindow( "Video1" );
    //    cvDestroyWindow( "Filtragem das cores" );
    //    cvDestroyWindow( "Sistema de cores RGB - HSV" );
        if (cont_BallSearch > 9){
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

	VISION_DIST_GOAL=0; //zera a variavel do gol

	VISION_MOTOR2_ANGLE = dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L);
	std::cout <<"Servo2 Bola = "<< VISION_MOTOR2_ANGLE << std::endl;

	VISION_MOTOR1_ANGLE = dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L);
	std::cout <<"Servo1 Bola= "<<VISION_MOTOR1_ANGLE << std::endl;

	BufferBallServo1 = VISION_MOTOR1_ANGLE; //Guarda a posição do servo1
	BufferBallServo2 = VISION_MOTOR2_ANGLE; //Guarda a posição do servo2

        }
        }
     		if( (cvWaitKey(10) & 255) == 27)
      		  {
			break;
		}


	//----------------------------------------------------------------------------------

		}

//******************************************************************************************************
	if(DECISION_ACTION_VISION==1 && IMU_STATE==0) //Decide ver o gol se o robo estiver em pé
	{       	
	}// endif DECISION_ACTION_VISION==1

//******************************************************************************************************
	// Vendo o Histograma---------------------------------------------------------------------------
	if(DECISION_ACTION_VISION==2 && IMU_STATE==0) //Decide ver o histograma se o robo estiver em pé
	{
	
	inicio = 0;
	HistogramPosition(inicio);
	usleep(1200);
	TransitionBall = 1;
	TransitionGoal = 1;

        // Pegar um quadro
        IplImage* Frame = cvQueryFrame( captura );

        if( !Frame )
        	{
                fprintf( stderr, "ERRO Quadro Histograma vazio...\n" );
                getchar();
                //break;
        	}


	/// Matrices for Histogram of Oriented Gradients calculation ( Image Test from WebCam )
        Mat MatOriginal(Frame, true); ///Convertion from iplimage(Frame) to mat (MatOriginal)

        cvtColor( MatOriginal, MatOriginal, CV_RGB2GRAY ); /// Convert Mat (MatOriginal) to gray

        hog.compute(MatOriginal, FeatureVectorTest, winStride, trainingPadding, locations);
        normalize( FeatureVectorTest, FeatureVectorTest, 1, 0, NORM_MINMAX, -1, Mat() );
	
	string FileHOG    = "/home/fei/RoboFEI-HT/genfiles/Actual/HOG.dat";
	saveHistogramVectorToFile(FeatureVectorTest, FileHOG);	

	/// Matrices for Color Histogram calculation

        IplImage* HSVFrame = cvCreateImage(cvSize(RESOLUCAO_X,RESOLUCAO_Y), IPL_DEPTH_8U, 3);

        cvCvtColor(Frame, HSVFrame, CV_BGR2HSV);

        Mat ImageHSV2(HSVFrame);

	calcHist( &ImageHSV2, 1, channels, Mat(), HistTest, 2, histSize, ranges, true, false );
	normalize( HistTest, HistTest, 0, 1, NORM_MINMAX, -1, Mat() );

	string FileColor  = "/home/fei/RoboFEI-HT/genfiles/Actual/Color.dat";  
	saveHistogramMatToFile(HistTest, FileColor);
	//usleep(1000);
	/// Computation of distances from Color Histogram vectors and Gradient Histogram vectors


		vector<float> HOGFieldFar, HOGFieldMed, HOGFieldNear, ColorFieldFar, ColorFieldMed, ColorFieldNear, HOG, Color;
				/// Bhattacharyya Distance For HOG-- Second parameter (3)


		double Prob = 0;

		double DistHOGFAR = 0;
 		double DistHOGMED = 0;
		double DistHOGNEAR = 0;
		double DistColorFAR = 0;
		double DistColorMED = 0;
		double DistColorNEAR = 0;
		bool flagfar  = false;
		bool flagmed  = false;
		bool flagnear = false;


		if (variables.count("F1")) ///Field One
		{
		HOGFieldFar    = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldOneFar.dat");
		HOGFieldMed    = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldOneMed.dat");
		HOGFieldNear   = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldOneNear.dat");
		ColorFieldFar  = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldOneFar.dat");
		ColorFieldMed  = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldOneMed.dat");
		ColorFieldNear = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldOneNear.dat");
		} ///Endif Variables.count ("F1")

		if (variables.count("F2")) ///Field Two
		{
		HOGFieldFar    = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldTwoFar.dat");
		HOGFieldMed    = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldTwoMed.dat");
		HOGFieldNear   = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldTwoNear.dat");
		ColorFieldFar  = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldTwoFar.dat");
		ColorFieldMed  = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldTwoMed.dat");
		ColorFieldNear = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldTwoNear.dat");
		} ///Endif Variables.count ("F2")

		if (variables.count("F3")) ///Field Three
		{
		HOGFieldFar    = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldThreeFar.dat");
		HOGFieldMed    = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldThreeMed.dat");
		HOGFieldNear   = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldThreeNear.dat");
		ColorFieldFar  = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldThreeFar.dat");
		ColorFieldMed  = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldThreeMed.dat");
		ColorFieldNear = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldThreeNear.dat");
		} ///Endif Variables.count ("F3")

		if (variables.count("F4")) ///Field Four
		{
		HOGFieldFar    = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldFourFar.dat");
		HOGFieldMed    = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldFourMed.dat");
		HOGFieldNear   = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/HOGFieldFourNear.dat");
		ColorFieldFar  = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldFourFar.dat");
		ColorFieldMed  = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldFourMed.dat");
		ColorFieldNear = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Fields/ColorFieldFourNear.dat");
		} ///Endif Variables.count ("F4")

		HOG   = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Actual/HOG.dat");
	        normalize( HOG, HOG, 1, 0, NORM_MINMAX, -1, Mat() );
		Color  = openFiletoGetHistogramVector("/home/fei/RoboFEI-HT/genfiles/Actual/Color.dat");
	        normalize( Color, Color, 1, 0, NORM_MINMAX, -1, Mat() );		
		usleep(2000);
		

		/// Bhattacharyya Distance For HOG-- Second parameter (3)
		DistHOGFAR    = 1 - compareHist( HOGFieldFar    , HOG , 3 );
		DistHOGMED    = 1 - compareHist( HOGFieldMed    , HOG , 3 );
		DistHOGNEAR   = 1 - compareHist( HOGFieldNear   , HOG , 3 );

		/// Bhattacharyya Distance For HSV Color-- Second parameter (3)
		DistColorFAR  = 1 - compareHist( ColorFieldFar  , Color , 3 );
		DistColorMED  = 1 - compareHist( ColorFieldMed  , Color , 3 );
		DistColorNEAR = 1 - compareHist( ColorFieldNear , Color , 3 );


	///Bhattacharyya
	/// - Low scores indicate good matches
	/// - High scores indicate bad matches
	
	/// Naiyve Bayes Rule P( Image Gradient | Image Color ) = P (Gradient) * P(Color | Gradient) \ P(Color)

	double ProbFAR = (1 + DistHOGFAR) * (1 + DistColorFAR) ;
	ProbFAR = ProbFAR/4;

	double ProbMED = (1 + DistHOGMED) * (1 + DistColorMED) ;
	ProbMED = ProbMED/4;

	double ProbNEAR = (1 + DistHOGNEAR) * (1 + DistColorNEAR) ;
	ProbNEAR = ProbNEAR/4;


	if ((ProbFAR > ProbMED) && (ProbFAR > ProbNEAR))   
		{Prob = ProbFAR; 
		    //cout << " Longe :" << ProbFAR << endl;
		flagfar = true;}

	if ((ProbMED > ProbFAR) && (ProbMED > ProbNEAR))   
		{Prob = ProbMED; 
    //cout << " Medio :" << ProbMED << endl;
flagmed = true;}

	if ((ProbNEAR > ProbFAR) && (ProbNEAR > ProbMED))  
		{Prob = ProbNEAR; 
	    //cout << " Perto :" << ProbNEAR << endl;
flagnear = true;}

inicio = 0;


	double Threshold = 0.65;
	double Margin = 0.05;

	//VISION_MOTOR1_ANGLE = dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L);
	//VISION_MOTOR2_ANGLE = dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L);

    if (Prob > Threshold)
    {
    cout << "-----------------------------------------------: " << endl;
    cout << "--------------Found!!!-------------------------: " << endl;
    cout << "||Servo 1: " << VISION_MOTOR1_ANGLE << "||Servo 2: " << VISION_MOTOR2_ANGLE << " || Probab: " << Prob <<" % ||"<< endl;
    cout << "---------------------------------------: " << endl;
    cout << " Perto :" << ProbNEAR << endl;
    cout << " Medio :" << ProbMED << endl;
    cout << " Longe :" << ProbFAR << endl;
    cout << "---------------------------------------: " << endl;

		LOCALIZATION_THETA = 1;
    }
    else
    {
        if ((Prob > Threshold*(1-Margin)) && (Prob < Threshold*(1+Margin)))
            {
            cout << "-----------------------------------------------: " << endl;
            cout << "--------------Close!!!-------------------------: " << endl;
          cout << "||Servo 1: " << VISION_MOTOR1_ANGLE << "||Servo 2: " << VISION_MOTOR2_ANGLE << " || Probab: " << Prob <<" % ||"<< endl;
            cout << "---------------------------------------: " << endl;
    cout << " Perto :" << ProbNEAR << endl;
    cout << " Medio :" << ProbMED << endl;
    cout << " Longe :" << ProbFAR << endl;
    cout << "---------------------------------------: " << endl;
            }
            else
            {
            cout << "-----------------------------------------------: " << endl;
            cout << "---------------Lost!!!-------------------------: " << endl;
   	    cout << "||Servo 1: " << VISION_MOTOR1_ANGLE << "||Servo 2: " << VISION_MOTOR2_ANGLE << " || Probab: " << Prob <<" % ||"<< endl;
            cout << "---------------------------------------: " << endl;
    cout << " Perto :" << ProbNEAR << endl;
    cout << " Medio :" << ProbMED << endl;
    cout << " Longe :" << ProbFAR << endl;
    cout << "---------------------------------------: " << endl;
            }
	//LocalizationSearch(inicio);
	LOCALIZATION_THETA = -1;


    }

	 dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, BufferBallServo1);
	 dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, BufferBallServo2);
	 sleep(2);

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
        IplImage* Frame = cvQueryFrame( captura );

        if( !Frame )
        	{
                fprintf( stderr, "ERRO Quadro Histograma vazio...\n" );
                getchar();
                //break;
        	}

	/// Matrices for Histogram of Oriented Gradients calculation ( Image Basis )
        Mat MatOriginal(Frame, true); ///Convertion from iplimage(Frame) to mat (MatOriginal)

        cvtColor( MatOriginal, MatOriginal, CV_RGB2GRAY ); /// Convert Mat (MatOriginal) to gray

        hog.compute(MatOriginal, FeatureVectorBase, winStride, trainingPadding, locations);
        normalize( FeatureVectorBase, FeatureVectorBase, 1, 0, NORM_MINMAX, -1, Mat() );

	string FileHOG  = "/home/fei/RoboFEI-HT/genfiles/Fields/HOGField"+ N + D +".dat";
	string FileColor  = "/home/fei/RoboFEI-HT/genfiles/Fields/ColorField"+ N + D +".dat";

	//cout << File << endl;

	saveHistogramVectorToFile(FeatureVectorBase, FileHOG);
	
	/// Matrices for Color Histogram calculation

        IplImage* HSVFrame = cvCreateImage(cvSize(RESOLUCAO_X,RESOLUCAO_Y), IPL_DEPTH_8U, 3);
        cvCvtColor(Frame, HSVFrame, CV_BGR2HSV);

        Mat ImageHSV(HSVFrame);

        /// Calculate the histograms for the HSV images
        calcHist( &ImageHSV, 1, channels, Mat(), HistBase, 2, histSize, ranges, true, false );
        normalize( HistBase, HistBase, 0, 1, NORM_MINMAX, -1, Mat() );

	saveHistogramMatToFile(HistBase, FileColor);

	usleep(1500);
	break;
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
    //------ Segue a bola para a cima do video -----------------------------------------
    if(posy<(RESOLUCAO_Y/2)*(1-CENTERBALL*(RESOLUCAO_X / RESOLUCAO_Y)) && *head_move1==false) // (RESOLUCAO_X / RESOLUCAO_Y) relação entre o tamanho da resolução
    {
        //if ((dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L)-( ((RESOLUCAO_Y/2)-posy) * AJUSTE))>512)
			dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L)-( ((RESOLUCAO_Y/2)-posy) * AJUSTE));
		//else
			//dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, 512);

        //head_move = true;
    }

    //------ Segue a bola para a baixo do video -----------------------------------------
    if(posy>(RESOLUCAO_Y/2)*(CENTERBALL*(RESOLUCAO_X / RESOLUCAO_Y)+1) && *head_move1==false) // (RESOLUCAO_X / RESOLUCAO_Y) relação entre o tamanho da resolução 
    {
       dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, dxl_read_word( HEAD_TILT, P_PRESENT_POSITION_L)+( (posy-(RESOLUCAO_Y/2)) * AJUSTE));

        //dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, dxl_read_word( HEAD_PAN, P_PRESENT_POSITION_L)-(((RESOLUCAO_X/2)-posx) * 0.30) );
        //head_move = true;
    }

    // Para a cabeça se chegou na posição desejada ----------------------------------------
    if(posy>=(RESOLUCAO_Y/2)*(1-CENTERBALL*(RESOLUCAO_X / RESOLUCAO_Y)) && posy<=(RESOLUCAO_Y/2)*(CENTERBALL*(RESOLUCAO_X / RESOLUCAO_Y)+1)){
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

void saveHistogramVectorToFile(vector<float> HistogramVector, string fileName)
{
    string separator = " "; // Use blank as default separator between single features
    fstream File;

    File.open(fileName.c_str(), ios::out);
    if (File.good() && File.is_open())
    {
 	for (int i = 0; i < HistogramVector.size(); ++i)
        {
         File << HistogramVector.at(i) << separator;
        }
        File << endl;
        File.flush();
        File.close();
    }
}


void saveHistogramMatToFile(Mat HistogramVector, string fileName)
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
                    File << HistogramVector.at<float>(i,j) << separator;
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
static unsigned int varredura2=0;

        dxl_write_word(HEAD_PAN, MOVING_SPEED, 150);//Seta as velocidades da cabeça
        dxl_write_word(HEAD_TILT, MOVING_SPEED, 200);

    if(inicio)
        varredura2--; // continua a varredura de onde parou

    if(varredura2>9||varredura2<1)
        varredura2=0;

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0)
	{
     varredura2++;
     //cont_BallSearch++;
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura2==8)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura2==7)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }
	

    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura2==6)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura2==5)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura2==4)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2+406);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }


    if(dxl_read_byte( HEAD_PAN, P_MOVING )==0 && varredura2==3)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }

    if(dxl_read_byte( HEAD_TILT, P_MOVING )==0 && varredura2==2)
    {
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2-344);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1+200);
    }

    if(dxl_read_byte( HEAD_TILT, P_MOVING )==0 && varredura2==1)
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
        dxl_write_word(HEAD_PAN, P_GOAL_POSITION_L, pos_servo2 + 20);
        dxl_write_word(HEAD_TILT, P_GOAL_POSITION_L, pos_servo1 - 110); // 240
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
            1.1, //-------------------SCALE FACTOR 1,5
            7,//------------------MIN NEIGHBOURS 2 7
            1,//---------------------- 1
                      // CV_HAAR_DO_CANNY_PRUNING,
            cvSize( 10,10), // ------MINSIZE
            cvSize(1000,1000) );//---------MAXSIZE

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
