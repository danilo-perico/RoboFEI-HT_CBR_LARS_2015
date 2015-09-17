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
#include "dynamixel.h"
#include <unistd.h>
#include <boost/program_options.hpp> //tratamento de argumentos linha de comando


#include "minIni.h"

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

#define CENTERBALL 0.05 // Porcentagem de quanto centraliza a bola no quadro
//#define AJUSTE 0.285 // para resolução de 640x480
//#define AJUSTE 0.285*0.49 // para resolução de 1280x720
//#define AJUSTE 0.285*0.33 // para resolução de 1920x1080
#define AJUSTE 0.285*0.25 // para resolução de 1920x1080
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
int pos_servo1;
int pos_servo2;
int cont=0; //____________________________________________________________
int cont_BallSearch=0;
int cont_GoalSearch=0;
Mat b_hist, g_hist, r_hist;

CvCapture* captura;
CvMemStorage* memoria;

int HeadFollow(float, float, bool*, bool*);

void BallSearch(bool);

double detect( IplImage *img, double &posx, double &posy); // funcao

	void saveHistogramVectorToFile(vector<float> HistogramVector, string fileName);
	vector<float> openFiletoGetHistogramVector(string fileName);
	void saveHistogramMatToFile(Mat HistogramVector, string fileName);

	string D;
	string N;

int countvector = 1;
bool TransitionBall = 1;
bool TransitionGoal = 1;
unsigned int BufferBallServo1; //Guarda a ultima posição do servo
unsigned int BufferBallServo2; //Guarda a ultima posição do servo
unsigned int BufferGoalServo1; //Guarda a ultima posição do servo do Gol
unsigned int BufferGoalServo2; //Guarda a ultima posição do servo do Gol


vector<float> FeatureVectorBase;
vector<float> FeatureVectorTest;


    HOGDescriptor hog; // Use standard parameters here
    vector<Point> locations;
    Size trainingPadding(Size(256, 256)); //melhor ate agora 16,16   10,10 - 0.18 -1.08
    Size winStride(Size(128, 128));

Mat HistTest;
Mat HistBase;

/// Using 50 bins for hue and 60 for saturation
	int HBins = 20;
	int SBins = 5;
	int VBins = 5;
    int histSize[] = { HBins, SBins, VBins  };

    // hue varies from 0 to 179, saturation from 0 to 255
    float HRanges[] = { 0, 180 };
    float SRanges[] = { 0, 256 };
    float VRanges[] = { 0, 256 };
    const float* ranges[] = { HRanges, SRanges, VRanges };

    // Use the o-th and 1-st channels
    int channels[] = { 0, 1 , 2}; ///Checar a quantidade de canais para incluir o V do HSV ( 18,3,3)




