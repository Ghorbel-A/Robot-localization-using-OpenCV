#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "cxtypes.h"

void sobel(IplImage * pImage, int deb_ligne, int fin_ligne,int deb_colonne,int fin_colonne)
	{
    uchar mat[480][640];
		//uchar ** mat;
	int ConvY[3][3]={{1,2,1},{0,0,0},{-1,-2,-1}};
	int ConvX[3][3]={{1,0,-1},{2,0,-2},{1,0,-1}};
	int mX,mY;
	int i,j,k,l;
	int moy;


	// convert to gray et stocker la valeur des pixels dans une matrice mat
    
	for(i=deb_ligne;i<fin_ligne;i++) 
	  {
		   for(j=deb_colonne;j<fin_colonne;j++) 
		   {

			          mat[i][j]=   (uchar)( pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels]*0.114 
						          +(uchar)( pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+1]*0.587
								  +(uchar)( pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+2]*0.2989;
						          

		   }
		 
	   }

        // appliquer sur l'image

for(i=deb_ligne;i<fin_ligne;i++) 

	   {
		   for(j=deb_colonne;j<fin_colonne;j++) 
		   {
			   for(k=0;k<pImage->nChannels;k++)
			   {
				    
				   (uchar)( pImage->imageData + i* pImage->widthStep)[j*  pImage->nChannels+k]=(uchar)mat[i][j];

			   }
		   }
	   }
	   

for(i=deb_ligne;i<(fin_ligne)-2;i++) 
	   {
		   for(j=deb_colonne;j<(fin_colonne)-2;j++) 
		  

		     
				{
					mX=0;
					mY=0;
					for( k=-1;k<=1;k++)
					{
						for(l=-1;l<=1;l++)
							
						{ 
					
								mX+=( mat[i+k][j+l]*ConvX[k+1][l+1]);
								mY+=( mat[i+k][j+l]*ConvY[k+1][l+1]); 

							 
						}
					}

				moy=sqrt(mX*mX+mY*mY);

				    if(moy>255) 
				       {
					      moy=0;
				       }
				 (uchar)(  pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels]=(uchar)moy;
				 (uchar)(  pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+1]=(uchar)moy;
				 (uchar)(  pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+2]=(uchar)moy;
	    
		   
	   }
    }



}



