#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "TestCOM.h"

void navigation() 

{
	
	/* variables locales */
    char buffer[256];
	char reponse[256];
	float old_position[2];
	float new_position[2];
	float diff_position[2];
	int tab[8];
	int iTabSize;
    int i=0,nId, nBytesWritten, nBytesRead;
	int xf=200,yf=200,xe,ye,d;
	int xo=0,yo=0;
	float x1,y1,l;
	float Theta = 0.785398163, dTheta;/*orientation initiale du robot*/
    int L = 300; /*distance entre les roues en mm*/
    double WHEEL_RADIUS = 7.81;
    int AXLE_LENGTH = 53;
    double  MM_PER_ENC = 3.14159 * 2 * WHEEL_RADIUS / 600;
    FILE *fichier;
	FILE *fichier_courbe;

	xe= xf - xo;
	ye= yf - yo;
    d =(int) (sqrt ( xe*xe + ye*ye)); /*distance qui sépare le robot du point objectif*/
    fichier=fopen("courbe1.dat","w");
	fichier_courbe=fopen("courbe1.gnu","w");





	/* reset encoders */
	WriteCOM("G,0,0\r", strlen("G,0,0\r"), &nBytesWritten); 
	ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead);
    
	/* calcule old position */
    WriteCOM("H\r", strlen("H\r"), &nBytesWritten);
	ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead);
	buffer[nBytesRead] = '\0';
	printf("%d octet(s) recu(s) :\n%s\n", nBytesRead, buffer);
    iTabSize =  sizeof(tab)/sizeof(tab[0]);	   		        	
	parseBuffer(tab,&iTabSize,buffer);
		for(i=0;i<2;i++)
		{
		old_position[i]= (tab[i] * MM_PER_ENC);
		printf("%f\t",old_position[i]);
		}
			
        WriteCOM("D,5,5\r", strlen("D,5,5\r"), &nBytesWritten);
	    ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead);

	while (d>10)
	{
		/* set speed */

	    printf("d=%d\n",d);
		/* calcule new position */
        WriteCOM("H\r", strlen("H\r"), &nBytesWritten);		
		ReadCOM(buffer, sizeof(buffer)-1, &nBytesRead);
	    buffer[nBytesRead] = '\0';
		printf("%d octet(s) recu(s) :\n%s\n", nBytesRead, buffer);
		iTabSize =  sizeof(tab)/sizeof(tab[0]);	   		        	
		parseBuffer(tab,&iTabSize,buffer);
		for(i=0;i<iTabSize;i++)
			{
			new_position[i]= (tab[i] * MM_PER_ENC);
		//	printf("new_position [%d]= %f\t",i,new_position[i]);
		    }
	 //   printf("\n");
		for(i=0;i<2;i++)
		{
		    diff_position[i]= new_position[i] - old_position[i];
  //           printf("diff_position [%d]= %f\t",i,diff_position[i]);
		}
		//printf("\n");

		 dTheta = ((diff_position[0] * (-1) + diff_position[1] * (1)) / AXLE_LENGTH);
		 printf("dTheta = %f\n",dTheta);
		 l = ((diff_position[0] * (1)) + (diff_position[1] * (1))) / 2;
          
		 x1 = xo + cos(Theta + dTheta/2)*l;
		 y1 = yo + sin(Theta + dTheta/2)*l;
		 Theta = Theta + dTheta;
		 
		if (fichier != NULL)
			 {
				 fprintf(fichier,"%f %f\n",x1,y1);
			 }
		 
		    /*distance entre le robot et le point objectif*/
		for(i=0;i<2;i++)
		{
		     old_position[i]= new_position[i];
		//	 printf("old_position [%d]= %f\t",i,old_position[i]);
		}
  //      printf("\n");


         xe = xf - x1;
         ye = yf - y1;
         d = (int)(sqrt(xe*xe+ye*ye));
         xo = x1;
         yo = y1;
		
		
	}
	if (d<=10)
	{
    WriteCOM("D,0,0\r", strlen("D,0,0\r"), &nBytesWritten);
    CloseCOM();
	}
    /* fermeture du port COM et retour */
	
    
	fclose(fichier);
	if (fichier_courbe != NULL)
		  {
			fprintf(fichier_courbe,"unset key\n");
			fprintf(fichier_courbe,"set title \"Trajectoire du Robot\"\n");
			fprintf(fichier_courbe,"set xlabel \"'x en pixels'\" \n");
			fprintf(fichier_courbe,"set ylabel \"y en pixels\" \n");
			fprintf(fichier_courbe,"set grid\n");
			fprintf(fichier_courbe,"plot [0:300] [0:300] 'courbe1.dat' lt 3\n");
			fprintf(fichier_courbe,"pause 100000 \n");
		}
		fclose(fichier_courbe);
		system("start gnuplot\\wgnuplot.exe courbe1.gnu");
		}