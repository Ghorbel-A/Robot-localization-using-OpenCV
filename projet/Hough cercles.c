#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "cxtypes.h"

float tab_cos[41]={0,0.999248,0.996993,0.993238,0.987989,0.981255,0.973044,0.963370,0.952248,
             0.939692, 0.925723,0.910362,0.893632,0.875581,0.856166,0.835487,0.813552,
				 0.790392,0.766044,0.740543,0.713929,0.686241,0.657521,0.627812,0.597158,
				 0.565606, 0.533204,0.499999,0.466043,0.431385,0.396079,0.360177,0.323733,
				 0.286803,0.249440,0.211703,0.173647,0.135331,0.096810,0.058144,0.019391};
float tab_sin[41]={0,0.038775,0.077492,0.116092,0.154818,0.192715,0.230615,0.268172,0.305326,
				 0.342020,0.378200,0.413810,0.448799,0.483112,0.516699,0.549509,0.581492,
				 0.6126,0.642787,0.672,0.700217,0.727373,0.753435,0.778364,0.802123,
				 0.824675,0.845986,0.866025,0.884761,0.902167,0.918216,0.932883,0.946148,
				 0.957989,0.968390,0.977333,0.984807,0.990800,0.995302,0.998308,0.999811};



void SetPixelInc(IplImage * pImage,int i,int j)
{

	if(i<0 || i>=pImage->height || j<0 || j>=pImage->width)
		return;
	//Pos=i*pImage->widthStep+j*pImage->nChannels;
	if((pImage->imageData + i* pImage->widthStep)[j*pImage->nChannels]==255)
	{
	   (pImage->imageData + i* pImage->widthStep)[j*pImage->nChannels]=0;
	   //(pImage->imageData + i*  pImage->widthStep)[j*pImage->nChannels+1]++;
	}
	else 
		(pImage->imageData + i*  pImage->widthStep)[j*pImage->nChannels]++;
}


////Retourne la valeur du pixel i,j du canal type souhaité
//void SetPixel(IplImage * pImage,int i,int j,int R,int G,int B)
//{
//
//	if(i<0 || i>=pImage->height || j<0 || j>=pImage->width)
//		return;
//	(pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+2]=R;
//	(pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+1]=G;
//	(pImage->imageData + i*  pImage->widthStep)[j*pImage->nChannels]=B;
//}


//Incrementation des pixels appartennant au cercle =>Hough
void DrawCircle(IplImage * pImage,int Rayon,int Xoffset,int Yoffset)
{

	double i=0,x=1,y=0;
	double ytmp=0,xtmp=0;

	int k=0;
	
	double Pas=1.04719*(1/Rayon);
	for(i=0;i<1.5708;i+=Pas)
	{
		xtmp=(x*tab_cos[k]+y*tab_sin[k]);
		ytmp=(-x*tab_sin[k]+y*tab_cos[k]);
		
		SetPixelInc(pImage,(int)(xtmp*Rayon+ Xoffset),(int)(ytmp*Rayon+Yoffset));
		SetPixelInc(pImage,(int)(-xtmp*Rayon+ Xoffset),(int)(ytmp*Rayon+Yoffset));
		SetPixelInc(pImage,(int)(xtmp*Rayon+ Xoffset),(int)(-ytmp*Rayon+Yoffset));
		SetPixelInc(pImage,(int)(-xtmp*Rayon+ Xoffset),(int)(-ytmp*Rayon+Yoffset));
		k=k+1;
	}

	
}


int Count(IplImage * pImage,int *X,int *Y,int deb_ligne , int fin_ligne,int deb_col,int fin_col)
{
	int Max=0,i=0,j=0;

	for(i=deb_ligne;i<fin_ligne;i++)
		for(j=deb_col;j<fin_col;j++)
		{
		      
			if(( pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels]>Max)
			{
				Max=(pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels];//+256 *(pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+1];
				*X=j;
				*Y=i;
								
			}
			
		}

	return Max;
}


void HoughCircleCompute(int deb_ligne , int fin_ligne,int deb_col,int fin_col,int RayonMin,int RayonMax,IplImage * pImage,int *X,int *Y,int *Rayon)
{
	
	int MaxAcc=-1,R=0,Acc=0,NewX=0,NewY=0;
	int i=0,j=0,l,k;
	uchar imageValues[480][640];

	//recuperation des pixels
	for(R=RayonMin;R<=RayonMax;R++)
	{
		
    for(i=deb_ligne;i<fin_ligne;i++) 
	   {
		   for(j=deb_col;j<fin_col;j++) 
		   {

			          imageValues[i][j]= ((uchar)( pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels] 
						                 +(uchar)( pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+1]
								         +(uchar)( pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+2])/3;
                   
										
					  if(imageValues[i][j]==255)

					    DrawCircle(pImage,R,i,j);
					  
					  
		   }
		 
	   
	
	}

	    Acc=Count(pImage,&NewX,&NewY,deb_ligne , fin_ligne, deb_col, fin_col);
		if(Acc>MaxAcc)
		{	
			MaxAcc=Acc;
			*X=NewX;
			*Y=NewY;
			*Rayon=R;	

		}

	}
	  *X+=1;
	  *Y+=1;


	 // Dessiner une croix

(uchar)(pImage->imageData + (*Y-1) * pImage->widthStep)[(*X+1)*pImage->nChannels+2]=255;
(uchar)(pImage->imageData + (*Y-1) * pImage->widthStep)[(*X-1)*pImage->nChannels+2]=255;
(uchar)(pImage->imageData + (*Y+1) * pImage->widthStep)[(*X-1)*pImage->nChannels+2]=255;
(uchar)(pImage->imageData + (*Y+1) * pImage->widthStep)[(*X+1)*pImage->nChannels+2]=255;
(uchar)(pImage->imageData + (*Y) * pImage->widthStep)[(*X)*pImage->nChannels+2]=255;
	
          	
}

