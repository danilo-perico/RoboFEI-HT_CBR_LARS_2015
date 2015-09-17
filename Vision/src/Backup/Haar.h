#include <stdio.h>
#include <math.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <vector>


class Haar
{
  private:
    CvHaarClassifierCascade *cascade;
    CvMemStorage            *storage;

    int       key;
    char      *filename;    
    
    double  a,  raio_medio, Raio, Distancia;

//-------------------------------------------------------------------------------------------------

    double detect( IplImage *img, double *till, double *pan, int a ){
        int i;
        double     Radius, Raio;

        CvSeq *object = cvHaarDetectObjects(
                img,
                cascade,
                storage,
                1.1, //-------------------SCALE FACTOR 1,5
                8,//------------------MIN NEIGHBOURS 2
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
        till[0] = r->y + (r->height/2);
        pan[0] = r->x + (r->width/2);

    cvCircle( img, cvPoint((r->x + (r->width/2)),(r->y + (r->height/2))), cvRound(Radius), CV_RGB(64, 255, 64), 3, 8, 0 );

                //printf("%d,%d\nnumber =%d\n",r->x,r->y,object->total);
		
		    printf("Raio: %g\n", Radius);

            }

        if(a) cvShowImage( "video", img );

		    return(Radius);
    };
//-------------------------------------------------------------------------------------------------
  public:
    Haar (CvCapture *capture, int a){
    a=0;
    raio_medio=0;
    
    filename = ( char* )"Ball.xml";
    cascade = ( CvHaarClassifierCascade* )cvLoad( filename, 0, 0, 0 );
    storage = cvCreateMemStorage(0);

    assert( cascade && storage && capture );

    if(a) cvNamedWindow("video", 1);
    };
//-------------------------------------------------------------------------------------------------    
    ~Haar (){
      cvDestroyWindow("video");
      cvReleaseHaarClassifierCascade(&cascade);
      cvReleaseMemStorage(&storage);
    };
//-------------------------------------------------------------------------------------------------
    double localizar_bola(IplImage  *frame, double *pan, double *till, int a){
	if ((cvWaitKey(10) & 255) != 27){

		  Raio=detect(frame,  pan,  till, a);
		
		  if (Raio!=0){
			  if(a<10){
				  raio_medio=raio_medio+Raio;	
				  a++;
				  }
			  else{
				  a=0;
				  raio_medio=raio_medio/10;
  //				Distancia=(1e-05*pow(raio_medio,4))+(-0.0049*pow(raio_medio,3))+(0.8284*pow(raio_medio,2))+(-62.05*raio_medio)+1869.1;
				  Distancia=331.59-53.3*log(raio_medio-50);
				  printf("Raio Medio: %g\n", raio_medio);
				  printf("Distancia: %g\n", Distancia);
				  raio_medio=0;
			  }
        return(raio_medio);
		  }
      else
        return(-1);

    }
	return(-1);
	};


  
};
