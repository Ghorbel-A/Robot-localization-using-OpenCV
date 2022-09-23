#include <math.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "cxtypes.h"



/* ic_threshold()

*/
void ic_threshold(IplImage * pImage,double value,int deb_ligne,int fin_ligne,int deb_colonne,int fin_colonne)

{
    int i=0,j=0,k=0;
	uchar cValue;
	cValue = (uchar)value;
		
	for(i=deb_ligne;i<fin_ligne;i++) 
		   {
			   for(j=deb_colonne;j<fin_colonne;j++) 
			   {
				   for(k=0;k<pImage->nChannels;k++)
				   {
					   if((uchar)pImage->imageData[i*pImage->widthStep+j*pImage->nChannels+k]> cValue)
					   {
						   (uchar)pImage->imageData[i*pImage->widthStep+j*pImage->nChannels+k]=255;
					   }
					   else
					   {
						   (uchar)pImage->imageData[i*pImage->widthStep+j*pImage->nChannels+k]=0;
							   
					   }
					  
					   
				   }
				  
			   }
		   }




}



			