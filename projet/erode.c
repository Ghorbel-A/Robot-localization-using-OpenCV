#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>


#include "cxtypes.h"


/* ic_erode()

*/

void Erosion(int *Noyau,int KernelLenght,int  BordureColor,IplImage * pImage,int deb_ligne, int fin_ligne,int deb_colonne,int fin_colonne) 
{

 	  int i,j,k,l,n,Pos; 
 	  int Skip; 

	  uchar Tab[480][640];
	  uchar mat[480][640];


 	  n=(int)sqrt((double)KernelLenght);
	 	 
	  for(i=deb_ligne+n/2;i<fin_ligne-n/2;i++) 

 		 for(j=deb_colonne+n/2;j<fin_colonne-n/2;j++) 
 			{ 
 			  
 			  Tab[i][j]=0; 
 			  Skip=0; 
 			  for(l=-n/2;l<=n/2 && Skip==0 ;l++) 

 				for(k=-n/2; k<=n/2 && Skip==0 ;k++)

				  if(Noyau[(k+n/2)+(l+n/2)*n] && ( pImage->imageData + (i+k)*  pImage->widthStep)[(j+l)*  pImage->nChannels]==0) 
 					Skip=1; 
 			  if(!Skip) 
 				Tab[i][j]=1; 
 			}


	 for(i=deb_ligne;i<fin_ligne;i++) 
		   {
			   for(j=deb_colonne;j<fin_colonne;j++) 
			   {
				   if (Tab[i][j]==1)
					  (uchar)(pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels]=255;
				   else (uchar)(pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels]=0;
				   (uchar)(pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+1]=(pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels];
				   (uchar)(pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels+2]=(pImage->imageData + i*  pImage->widthStep)[j*  pImage->nChannels];

			  } 
	 }

	}
 
//void Dillatation(int *Noyau,int KernelLenght,int  BordureColor,IplImage * pImage) 
//{
//	int i=0,j=0,k=0;
//	
//	int ElementStructurant[5][5]={0,0,1,0,0, 0,0,1,0,0, 1,1,1,1,1, 0,0,1,0,0, 0,0,1,0,0};
//	
//		
//	for(i=0;i<pImage->height;i++) 
//		   {
//			   for(j=0;j<pImage->width;j++) 
//			   {
//				   for(k=0;k<pImage->nChannels;k++)
//				   {
//					    if ((uchar)pImage->imageData[i*pImage->widthStep+j*pImage->nChannels+k]==0)
//					   {
//						   (uchar)pImage->imageData[i*pImage->widthStep+j*pImage->nChannels+k]=255;
//					   }
//					   else
//					   {
//						   (uchar)pImage->imageData[i*pImage->widthStep+j*pImage->nChannels+k]=0;
//							   
//					   }
//					  
//					   
//				   }
//				  
//			   }
//		   }
//
//	Erosion(ElementStructurant,25,0,pImage);
//
//
//	for(i=0;i<pImage->height;i++) 
//		   {
//			   for(j=0;j<pImage->width;j++) 
//			   {
//				   for(k=0;k<pImage->nChannels;k++)
//				   {
//					   if ((uchar)pImage->imageData[i*pImage->widthStep+j*pImage->nChannels+k]==0)
//					   {
//						   (uchar)pImage->imageData[i*pImage->widthStep+j*pImage->nChannels+k]=255;
//					   }
//					   else
//					   {
//						   (uchar)pImage->imageData[i*pImage->widthStep+j*pImage->nChannels+k]=0;
//							   
//					   }
//					  
//					   
//				   }
//				  
//			   }
////		   }
//}
//
//void Ouverture(int *Noyau,int KernelLenght,int BordureColor,IplImage * pImage) 
// 	{ 
// 
// 
//          Erosion(Noyau,KernelLenght,BordureColor,pImage); 
//    	  Dillatation(Noyau,KernelLenght,BordureColor,pImage); 
// 
//
// 	} 