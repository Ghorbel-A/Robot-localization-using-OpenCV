/***********************************************************************************************************************
*																													  *
*								        											                                  *
*	                                    Traitement sur l'image de réference 			     						  *
*																													  *
*																													  *
***********************************************************************************************************************/

#include <windows.h>
#include <highgui.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "binarisation.h"
#include "Hough cercles.h"
#include "erode.h"
#include "sobel.h"
#include "cxtypes.h"


int Ya=0;
int Y1=0;
int X=0,Y=0,X1=0,X2=0,Y2=0,X3=0,Y3=0,X4=0,Y4=0,Ra=0;

void pre_traitement() 
{

     IplImage* image_ref=0;	 
	 FILE *verif;
	 FILE *matrice_image;
	 FILE *resultat;
	 int i,j,k,comp=0;

     int ElementStructurant[5][5]={0,0,1,0,0, 0,1,1,1,0, 1,1,1,1,1, 0,1,1,1,0, 0,0,1,0,0
		};
	
	 // int X1=0,X2=0,Y2=0,X3=0,Y3=0,X4=0,Y4=0,Ra=0;
	 

	 //Chargement de l'image de reference 
	
	  
	 image_ref = cvLoadImage("frame10000.jpeg",CV_LOAD_IMAGE_COLOR);//GRAYSCALE
     if(!image_ref)
	  {
          printf("Could not load image file !!!! \n");
          exit(0);
      }

    // Detection de contours
	
	 sobel(image_ref,0,image_ref->height,0,image_ref->width);

	 // Binaristation
	
	 ic_threshold(image_ref,60,0,image_ref->height,0,image_ref->width);


     //Erosion(ElementStructurant,25,0,image_ref,0,image_ref->height,0,image_ref->width) ;

	 //Detection des 4 cercles de l'image de reference  


	//HoughCircleCompute(60,image_ref->height-40,126,image_ref->width-117,27,29,image_ref,&X,&Y,&Ra);
 //   printf("Cercle 1 de centre (%d,%d) et de Rayon=%d \n",X,480-Y,Ra);
   
  //   HoughCircleCompute(0,image_ref->height/2,0,image_ref->width/2,15,16,image_ref,&X4,&Y4,&Ra);
  //   printf("Cercle 4 de centre (%d,%d) et de Rayon=%d \n",X4,480-Y4,Ra);
  //

	 //HoughCircleCompute(0,image_ref->height/2,image_ref->width/2,image_ref->width,15,16,image_ref,&X3,&Y3,&Ra);
  //   printf("Cercle 3 de centre (%d,%d) et de Rayon=%d \n",X3,480-Y3,Ra);


	 //HoughCircleCompute(image_ref->height/2,image_ref->height,image_ref->width/2,image_ref->width,15,16,image_ref,&X2,&Y2,&Ra);
  //   printf("Cercle 2 de centre (%d,%d) et de Rayon=%d \n",X2,480-Y2,Ra);


	 //HoughCircleCompute(image_ref->height/2,image_ref->height,0,image_ref->width/2,15,16,image_ref,&X1,&Y1,&Ra);
  //   printf("Cercle 1 de centre (%d,%d) et de Rayon=%d \n",X1,480-Y1,Ra);

	 HoughCircleCompute(44,480-60,40,640-167,27,27,image_ref,&X,&Y,&Ra);
     printf("Cercle  de centre (%d,%d) et de Rayon=%d \n",X,480-Y,Ra);
 //    //Afichage de l'image
	   cvShowImage("image_ref", image_ref);
	// 
 //    //cvDestroyWindow( "image_ref" );
	// 
	 cvWaitKey(0);
	
	 
}